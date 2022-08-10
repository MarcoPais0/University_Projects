//Marco da Cruz Pais Nº2019218981
//Simão Carvalho Monteiro Nº2019215412

#ifndef SO_PROJETO_RACEMANAGER_H
#define SO_PROJETO_RACEMANAGER_H
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

void raceManager();

int unnamed_pipe[100][2];


#endif
