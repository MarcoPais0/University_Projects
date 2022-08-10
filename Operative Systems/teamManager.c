//Marco da Cruz Pais Nº2019218981
//Simão Carvalho Monteiro Nº2019215412

#include "raceSimulator.h"
#include "malfunctionManager.h"
#include "raceManager.h"

malfunction received_malfunction;
int team;
pthread_mutex_t box_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t shm_mutex = PTHREAD_MUTEX_INITIALIZER;
int fix_time;
int shmid;
int *box_full; //0 - Livre, 1 - Reservada, 2 - Ocupada
char send[512];

void *car_state(void* t){
  int* car_id = (int*)t;
  if(new_race->car_list[team][*car_id].team[0] == '\0'){
    pthread_exit(NULL);
  }
  car *this_car = &(new_race->car_list[team][*car_id]);
  snprintf(send, sizeof(send),"CAR %d OF TEAM %s ENTERING RUNNING MODE", this_car->n_car, this_car->team);
  close(unnamed_pipe[team][0]);
  write(unnamed_pipe[team][1], &send, sizeof(send));
  while(1){
    //Modo de Corrida
    if(this_car->state == 0){
      this_car->distance += (this_car->speed * new_race->time_unit);
      this_car->deposit -= this_car->consumption;
    }

    //Modo de Segurança
    else if(this_car->state == 1){
      this_car->distance += (0.3 * this_car->speed * new_race->time_unit);
      this_car->deposit -= (0.4 * this_car->consumption);
    }

    if((int)this_car->distance % new_race->lap_size < 25 && new_race->end == 1){
      pthread_exit(NULL);
    }

    if(this_car->state == 0){
      //Abastecimento 4 Voltas
      if(this_car->deposit < this_car->consumption * ((new_race->lap_size * 4)/(this_car->speed * new_race->time_unit))){
        if((int)this_car->distance % new_race->lap_size < 25){
          pthread_mutex_lock(&box_mutex);
          if(*box_full == 0){
            *box_full = 2;
            pthread_mutex_unlock(&box_mutex);
            pthread_mutex_lock(&shm_mutex);
            new_race->n_fills++;
            pthread_mutex_unlock(&shm_mutex);
            this_car->deposit = new_race->deposit_size;
            this_car->box_stops++;
            snprintf(send, sizeof(send),"CAR %d OF TEAM %s FILLED DEPOSIT", this_car->n_car, this_car->team);
            close(unnamed_pipe[team][0]);
            write(unnamed_pipe[team][1], &send, sizeof(send));
            sleep(2);
            snprintf(send, sizeof(send),"CAR %d OF TEAM %s ENTERING RUNNING MODE", this_car->n_car, this_car->team);
            close(unnamed_pipe[team][0]);
            write(unnamed_pipe[team][1], &send, sizeof(send));
            pthread_mutex_lock(&box_mutex);
            if(*box_full != 1){
              *box_full = 0;
            }
            pthread_mutex_unlock(&box_mutex);
          }
          else{
            pthread_mutex_unlock(&box_mutex);
          }
        }
      }

      //Abastecimento 2 Voltas
      else if(this_car->deposit < this_car->consumption * ((new_race->lap_size * 2)/(this_car->speed * new_race->time_unit))){
        this_car->state = 2;
        snprintf(send, sizeof(send),"CAR %d OF TEAM %s ENTERING SAFETY MODE", this_car->n_car, this_car->team);
        close(unnamed_pipe[team][0]);
        write(unnamed_pipe[team][1], &send, sizeof(send));
        pthread_mutex_lock(&box_mutex);
        *box_full = 1;
        pthread_mutex_unlock(&box_mutex);
      }

      //Segurança
      if (this_car->state == 2){
        pthread_mutex_lock(&box_mutex);
        *box_full = 1;
        pthread_mutex_unlock(&box_mutex);
        if((int)this_car->distance % new_race->lap_size < 25){
          pthread_mutex_lock(&box_mutex);
          if(*box_full != 2){
            *box_full = 2;
            pthread_mutex_unlock(&box_mutex);
            pthread_mutex_lock(&shm_mutex);
            new_race->n_fills++;
            pthread_mutex_unlock(&shm_mutex);
            this_car->deposit = new_race->deposit_size;
            this_car->box_stops++;
            fix_time = (rand()%(new_race->T_box_max + 1 - new_race->T_box_min)) + new_race->T_box_min;
            sleep(fix_time);
            sleep(2);
            this_car->state = 0;
            snprintf(send, sizeof(send),"CAR %d OF TEAM %s FIXED AND FILLED DEPOSIT", this_car->n_car, this_car->team);
            close(unnamed_pipe[team][0]);
            write(unnamed_pipe[team][1], &send, sizeof(send));
            snprintf(send, sizeof(send),"CAR %d OF TEAM %s ENTERING RUNNING MODE", this_car->n_car, this_car->team);
            close(unnamed_pipe[team][0]);
            write(unnamed_pipe[team][1], &send, sizeof(send));
            pthread_mutex_lock(&box_mutex);
            if(*box_full != 1){
              *box_full = 0;
            }
            pthread_mutex_unlock(&box_mutex);
          }
        }
        else{
          pthread_mutex_unlock(&box_mutex);
        }
      }
    }

    //Desistência
    if(this_car->deposit == 0 && this_car->distance != 0){
      this_car->state = 3;
      pthread_mutex_lock(&shm_mutex);
      new_race->n_not_finish--;
      if(new_race->n_not_finish == 0){
        new_race->end = 1;
      }
      pthread_mutex_unlock(&shm_mutex);
      snprintf(send, sizeof(send),"CAR %d OF TEAM %s GIVES UP", this_car->n_car, this_car->team);
      close(unnamed_pipe[team][0]);
      write(unnamed_pipe[team][1], &send, sizeof(send));
      pthread_exit(NULL);
    }

    //Terminado
    if(this_car->distance >= (new_race->lap_number * new_race->lap_size)){
      this_car->state = 4;
      pthread_mutex_lock(&shm_mutex);
      new_race->n_not_finish--;
      if(new_race->n_not_finish == 0){
        new_race->end = 1;
      }
      pthread_mutex_unlock(&shm_mutex);
      snprintf(send, sizeof(send),"CAR %d OF TEAM %s FINISHED THE RACE", this_car->n_car, this_car->team);
      close(unnamed_pipe[team][0]);
      write(unnamed_pipe[team][1], &send, sizeof(send));
      pthread_exit(NULL);
    }
    sleep(new_race->time_unit);
  }
}

void teamManager(int team_id) {
  int car_id[new_race->max_car];
  pthread_t cars[new_race->max_car];
  team = team_id;

  if ((shmid = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0766)) < 0) {
    perror("Error in shmget with IPC_CREAT");
    exit(1);
  }
  box_full = (int*) shmat(shmid, NULL, 0);
  *box_full = 0;
  pthread_mutex_lock(start_race_mutex);
  while(new_race->start != 1){
    pthread_cond_wait(start_race,start_race_mutex);
  }
  pthread_mutex_unlock(start_race_mutex);
  for (int i = 0; i < (new_race->max_car); i++) {
    car_id[i] = i;
    pthread_create(&cars[i], NULL, car_state, &car_id[i]);
  }

  while(1){
    if(msgrcv(mqid, &received_malfunction, sizeof(malfunction),(long)(team + 1), 0) < 0){
      perror("Error receiving message through message queue.");
      exit(0);
    }
    if(received_malfunction.car_number == -1){
      break;
    }
    snprintf(send, sizeof(send),"NEW PROBLEM IN CAR %d OF TEAM %s", new_race->car_list[team][received_malfunction.car_number].n_car, new_race->car_list[team][received_malfunction.car_number].team);
    close(unnamed_pipe[team][0]);
    write(unnamed_pipe[team][1], &send, sizeof(send));
    new_race->car_list[team][received_malfunction.car_number].state = 1;
    snprintf(send, sizeof(send),"CAR %d OF TEAM %s ENTERING SAFETY MODE", new_race->car_list[team][received_malfunction.car_number].n_car, new_race->car_list[team][received_malfunction.car_number].team);
    close(unnamed_pipe[team][0]);
    write(unnamed_pipe[team][1], &send, sizeof(send));
    pthread_mutex_lock(&box_mutex);
    *box_full = 1;
    pthread_mutex_unlock(&box_mutex);
  }
  pthread_mutex_destroy(&box_mutex);
  pthread_mutex_destroy(&shm_mutex);
  shmctl(shmid, IPC_RMID, NULL);
  for (int i = 0; i < new_race->max_car; i++) {
    pthread_join(cars[i], NULL);
  }
}
