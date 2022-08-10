//Marco da Cruz Pais Nº2019218981
//Simão Carvalho Monteiro Nº2019215412

#ifndef SO_PROJETO_RACESIMULATOR_H
#define SO_PROJETO_RACESIMULATOR_H
#define PIPE_NAME "corrida"
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/msg.h>
#include "teamManager.h"

pthread_mutex_t *start_race_mutex;
pthread_cond_t * start_race;


typedef struct race {
  int time_unit;
  int lap_size;
  int lap_number;
  int team_number;
  int max_car;
  int T_breakdown;
  int T_box_min;
  int T_box_max;
  int deposit_size;
  int start;
  int n_malfunction;
  int n_fills;
  int n_not_finish;
  int end;
  car car_list[100][100];
} race;

void raceSimulator(FILE *config);

void log_out(char *out);

void sigtstp(int signum);

void sigint(int signum);

race * new_race;

#endif
