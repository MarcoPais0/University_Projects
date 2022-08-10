//Marco da Cruz Pais Nº2019218981
//Simão Carvalho Monteiro Nº2019215412

#include "raceSimulator.h"
#include "malfunctionManager.h"
#include "raceManager.h"

malfunction new_malfunction;
time_t t;

void malfunctionManager(){
  srand((unsigned) time(&t));
  pthread_mutex_lock(start_race_mutex);
  while(new_race->start != 1){
    pthread_cond_wait(start_race,start_race_mutex);
  }
  pthread_mutex_unlock(start_race_mutex);

  int random = 0;
  while(1){
    sleep(new_race->T_breakdown);
    if(new_race->end == 1){
      for(int i = 0; i<new_race->team_number; i++){
        new_malfunction.mtype = i + 1;
        new_malfunction.car_number = -1;
        if(msgsnd(mqid, &new_malfunction, sizeof(malfunction), 0) < 0){
          perror("Error sending message through message queue");
          exit(0);
        }
      }
      exit(0);
    }
    for(int i = 0; i < new_race->team_number; i++){
      for(int j = 0; j < new_race->max_car; j++){
        random = rand() % 101;
        new_malfunction.mtype = i + 1;
        new_malfunction.car_number = j;
        if(random > new_race->car_list[i][j].reliability && new_race->car_list[i][j].state != 3 && new_race->car_list[i][j].state != 4){
          if(msgsnd(mqid, &new_malfunction, sizeof(malfunction), 0) < 0){
            perror("Error sending message through message queue");
            exit(0);
          }
          new_race->n_malfunction++;
        }
      }
    }
  }

}
