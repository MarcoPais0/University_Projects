//Marco da Cruz Pais Nº2019218981
//Simão Carvalho Monteiro Nº2019215412
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
#ifndef SO_PROJETO_TEAMMANAGER_H
#define SO_PROJETO_TEAMMANAGER_H

typedef struct car {
    char team[250];
    int n_car;
    int speed;
    float consumption;
    int reliability;
    int deposit;
    float distance;
    int box_stops;
    int state; //0 - Corrida, 1 - Segurança, 2 - Desistência, 3 - Terminado
} car;

void teamManager(int team_id);

#endif
