//Marco da Cruz Pais Nº2019218981
//Simão Carvalho Monteiro Nº2019215412

#include "raceSimulator.h"
#include "raceManager.h"

#define BUFFLEN 512

void add_car(char *comando);
int fd;

void raceManager(){
  char send[BUFFLEN];
  char comando[256];
  char aux[256];
  int nread;


  for (int i = 0; i < (new_race->team_number); i++) {
    if (pipe(&unnamed_pipe[i][0]) == -1){
      perror("Create unnamed pipe error");
      exit(1);
    }
    if (fork() == 0) {
      teamManager(i);
      exit(0);
    }
  }

  if ((fd = open(PIPE_NAME, O_RDONLY)) < 0) {
    perror("Cannot open named pipe for reading: ");
    exit(0);
  }
  fd_set read_set;
  struct timeval tv = {5, 0};
  while(1){
    if(new_race->end == 1){
      for(int i = 0; i<new_race->team_number; i++){
        wait(NULL);
      }
      kill(0, SIGINT);
      break;
    }
    if(new_race->start == 1){
      FD_ZERO(&read_set);
      for (int i = 0; i < new_race->team_number; i++){
        FD_SET(unnamed_pipe[i][0],&read_set);
      }
      if(select(unnamed_pipe[new_race->team_number-1][0] + 1, &read_set, NULL, NULL, &tv) > 0){
        for(int i = 0; i < new_race->team_number; i++){
          if(FD_ISSET(unnamed_pipe[i][0],&read_set)){
            close(unnamed_pipe[i][1]);
            nread = read(unnamed_pipe[i][0], &send, sizeof(send));
            send[nread - 1] = '\0';
            log_out(send);

          }
        }
      }
    }
    memset(aux, 0, sizeof(aux));
    memset(comando, 0, sizeof(comando));
    if((nread = read(fd, comando, sizeof(comando))) != 0){
      comando[nread - 1] = '\0';
      if(new_race->start == 0){
        strcpy(aux, comando);

        if (strcmp(comando, "START RACE!") == 0){
          int all_teams = 0;
          log_out("NEW COMMAND RECEIVED: START RACE!");
          for(int i = 0; i < new_race->team_number; i++){
            if(new_race->car_list[i][0].team[0] == 0){
              all_teams = 1;
            }
          }
          if(all_teams == 0){
            pthread_mutex_lock(start_race_mutex);
            new_race->start = 1;
            pthread_cond_broadcast(start_race);
            pthread_mutex_unlock(start_race_mutex);
            log_out("RACE STARTED.");
          }
          else{
            log_out("COMMAND REJECTED, NOT ENOUGH CARS IN EACH TEAM TO START THE RACE.");
          }
        }

        else if(strcmp(strtok(aux, " "), "ADDCAR") == 0){
          add_car(comando);
        }

        else {
          char send[BUFFLEN];
          snprintf(send, sizeof(send),"WRONG COMMAND => %s", comando);
          log_out(send);
        }
      }
      else{
        char send[BUFFLEN];
        snprintf(send, sizeof(send),"REJECTED RACE ALREADY STARTED => %s", comando);
        log_out(send);
      }
    }
  }
}

void add_car(char *comando){
  char aux[256];
  char send[BUFFLEN];
  char *remaining;
  int aux_number = 0, aux_speed = 0, aux_reliability = 0;
  float aux_consumption = 0;
  char aux_team[128];
  strcpy(aux, comando);
  remaining = strtok(comando, " ");
  remaining = strtok(NULL, " ");
  if(remaining != NULL){
    strcpy(aux_team, remaining);
  }
  remaining = strtok(NULL, " ");
  if(remaining != NULL){
    aux_number = atoi(remaining);
  }
  remaining = strtok(NULL, " ");
  if(remaining != NULL){
    aux_speed = atoi(remaining);
  }
  remaining = strtok(NULL, " ");
  if(remaining != NULL){
    aux_consumption = atof(remaining);
  }
  remaining = strtok(NULL, " ");
  if(remaining != NULL){
    aux_reliability = atoi(remaining);
  }
  if(aux_number == 0 || aux_speed == 0 || aux_consumption == 0 || aux_reliability == 0){
    snprintf(send, sizeof(send), "WRONG COMMAND => %s", aux);
    log_out(send);
  }
  else{
    for(int i = 0; i < new_race->team_number; i++){
      if(new_race->car_list[i][0].team[0] == '\0' || (strcmp(new_race->car_list[i][0].team, aux_team) == 0)){
        for(int j = 0; j < new_race->max_car; j++){
          if(new_race->car_list[i][j].n_car != aux_number){
            if(new_race->car_list[i][j].team[0] == '\0'){
              strcpy(new_race->car_list[i][j].team, aux_team);
              new_race->car_list[i][j].n_car = aux_number;
              new_race->car_list[i][j].reliability = aux_reliability;
              new_race->car_list[i][j].speed = aux_speed;
              new_race->car_list[i][j].consumption = aux_consumption;
              new_race->car_list[i][j].deposit = new_race->deposit_size;
              new_race->n_not_finish++;
              snprintf(send, sizeof(send), "NEW CAR LOADED => TEAM: %s, CAR: %d, SPEED: %d, CONSUMPTION: %f, RELIABILITY: %d", aux_team, aux_number, aux_speed, aux_consumption, aux_reliability);
              log_out(send);
              return;
            }
          }
        }
        break;
      }
    }
    snprintf(send, sizeof(send), "WRONG TEAM INSERTED OR TEAM FULL => %s", aux);
    log_out(send);
  }
}
