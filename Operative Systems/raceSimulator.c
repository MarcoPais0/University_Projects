//Marco da Cruz Pais Nº2019218981
//Simão Carvalho Monteiro Nº2019215412


#include "raceSimulator.h"
#include "raceManager.h"
#include "malfunctionManager.h"

#define size 25
#define PIPE_NAME "corrida"

sem_t *mutex;

void init();

void end();

void log_out(char *out);

void sigint(int signum);

void sigtstp(int signum);

void show_stats();

int shmid;
void*shm;
pthread_mutexattr_t attrmutex;
pthread_condattr_t attrcond;

void raceSimulator(FILE *config) {
  //Lê e valida o ficheiro config
  int time_unit;
  int lap_size;
  int lap_number;
  int team_number;
  int max_car;
  int T_breakdown;
  int T_box_min;
  int T_box_max;
  int deposit_size;
  int n = 1;
  char line[size];
  signal(SIGINT, SIG_IGN);
  signal(SIGTSTP, SIG_IGN);
  signal(SIGUSR1, SIG_IGN);
  while (fgets(line, size, config)!=NULL) {
    char *remaining;
    long first = 0;
    long second = 0;
    first = strtol(line, &remaining, 10);
    second =  strtol(remaining + 1, &remaining, 10);
    if (first > 0 && second == 0) {
      if (n == 1)
      time_unit = first;
      else if (n == 3) {
        team_number = first;
        if (team_number < 3) {
          fprintf(stderr, "Simulator has to have at least 3 teams.\n");
          exit(EXIT_FAILURE);
        }
      } else if (n == 4) {
        max_car = first;
      } else if (n == 5)
      T_breakdown = first;
      else if (n == 7)
      deposit_size = first;
    } else if (first > 0 && second > 0) {
      if (n == 2) {
        lap_size = first;
        lap_number = second;
      } else if (n == 6) {
        T_box_min = first;
        T_box_max = second;
      }
    } else {
      fprintf(stderr, "Something wrong with the Config file.\n");
      exit(EXIT_FAILURE);
    }
    n++;
  }

  fclose(config);


  init();

  log_out("SIMULATOR STARTING");



  if ((shmid = shmget(IPC_PRIVATE, sizeof(race)+sizeof(pthread_mutex_t)+sizeof(pthread_cond_t), IPC_CREAT | 0766)) < 0) {
    perror("Error in shmget with IPC_CREAT\n");
    exit(1);
  }

  //Attach memória partilhada
  shm = shmat(shmid, NULL, 0);
  new_race = (race*) shm;
  (new_race->time_unit) = time_unit;
  (new_race->lap_size) = lap_size;
  (new_race->lap_number) = lap_number;
  (new_race->team_number) = team_number;
  (new_race->max_car) = max_car;
  (new_race->T_breakdown) = T_breakdown;
  (new_race->T_box_min) = T_box_min;
  (new_race->T_box_max) = T_box_max;
  (new_race->deposit_size) = deposit_size;
  (new_race->start) = 0;
  (new_race->n_malfunction) = 0;
  (new_race->n_fills) = 0;
  (new_race->n_not_finish) = 0;
  memset(new_race->car_list, 0, sizeof(new_race->car_list));
  start_race_mutex = (pthread_mutex_t *)((void*)shm + sizeof(race));
  start_race = (pthread_cond_t *)((void*)shm + sizeof(race) + sizeof(pthread_mutex_t));
  pthread_mutexattr_init(&attrmutex);
  pthread_mutexattr_setpshared(&attrmutex, PTHREAD_PROCESS_SHARED);
  pthread_condattr_init(&attrcond);
  pthread_condattr_setpshared(&attrcond, PTHREAD_PROCESS_SHARED);
  pthread_mutex_init(start_race_mutex, &attrmutex);
  pthread_cond_init(start_race, &attrcond);

  if ((mkfifo(PIPE_NAME, O_CREAT|O_EXCL|0766)<0) && (errno != EEXIST)) {
    perror("Cannot create pipe: ");
    exit(0);
  }

  if((mqid = msgget(IPC_PRIVATE,IPC_CREAT|0777)) < 0){
    perror("Error creating message queue");
    exit(0);
  }



  //Criar os outros dois processos
  if (fork() == 0) {
    raceManager();
    exit(0);
  }

  if  (fork() == 0){
    malfunctionManager();
    exit(0);
  }

  //Handler dos Sinais
  signal(SIGINT, sigint);
  signal(SIGTSTP, sigtstp);


  pause();

}

void init() {
  sem_unlink("MUTEX");
  mutex = sem_open("MUTEX", O_CREAT | O_EXCL, 0700, 1);
}

void end() {
  new_race->end = 1;
  show_stats();
  for(int i = 0; i < 2; i++){
    wait(NULL);
  }
  pthread_mutex_destroy(start_race_mutex);
  pthread_mutex_destroy(start_race_mutex);
  unlink(PIPE_NAME);
  sem_close(mutex);
  sem_unlink("MUTEX");
  shmdt(shm);
  shmctl(shmid, IPC_RMID, NULL);
}

void log_out(char *out) {
  sem_wait(mutex);
  FILE *f;
  time_t T= time(NULL);
  struct  tm tm = *localtime(&T);
  f = fopen("log.txt", "a");
  //fprintf(f, "%02d:%02d:%02d %s\n",tm.tm_hour, tm.tm_min, tm.tm_sec , out);
  fprintf(stdout, "%02d:%02d:%02d %s\n",tm.tm_hour, tm.tm_min, tm.tm_sec, out);
  fclose(f);
  sem_post(mutex);
}


void sigint(int signum) {
  signal(SIGINT, SIG_IGN);
  signal(SIGTSTP, SIG_IGN);
  signal(SIGUSR1, SIG_IGN);
  if(new_race->start == 1){
    log_out("SIGNAL SIGINT RECEIVED");
    log_out("SIMULATOR CLOSING");
    end();
    exit(0);
  }
  signal(SIGINT, sigint);
  signal(SIGTSTP, show_stats);
}
void sigtstp(int signum){
  signal(SIGINT, SIG_IGN);
  signal(SIGTSTP, SIG_IGN);
  signal(SIGUSR1, SIG_IGN);
  show_stats();
  signal(SIGINT, sigint);
  signal(SIGTSTP, show_stats);
}
void show_stats(){
  char send[512];
  car tabela[new_race->team_number*new_race->max_car];
  memset(tabela, 0, sizeof(tabela));
  int x = 0;
  for(int i = 0; i < new_race->team_number; i++){
    for(int j = 0; j < new_race->max_car; j++){
      if(new_race->car_list[i][j].team[0] != '\0'){
        tabela[x++] = new_race->car_list[i][j];
      }
    }
  }
  car aux;
  for(int i = 0; i < (sizeof(tabela)/sizeof(tabela[0])); i++){
    for(int j = i; j < (sizeof(tabela)/sizeof(tabela[0])) - i; j++){
      if(tabela[j].distance < tabela[j+1].distance){
        aux = tabela[j];
        tabela[j] = tabela[j + 1];
        tabela[j + 1] = aux;
      }
    }
  }
  log_out("TOP 5:");
  for(int place = 0; place < 5; place++){
    snprintf(send, sizeof(send),"%d PLACE: CAR: %d TEAM: %s LAPS: %d BOX STOPS: %d", place + 1, tabela[place].n_car, tabela[place].team, (int)tabela[place].distance/new_race->lap_size, tabela[place].box_stops);
    log_out(send);
  }
  x = 0;
  while(tabela[x].team[0] != '\0'){
    x++;
  }
  snprintf(send, sizeof(send),"%d PLACE: CAR: %d TEAM: %s LAPS: %d BOX STOPS: %d", x, tabela[x].n_car, tabela[x].team, (int)tabela[x].distance/new_race->lap_size, tabela[x].box_stops);
  log_out(send);
  snprintf(send, sizeof(send),"TOTAL MALFUNCTIONS: %d", new_race->n_malfunction);
  log_out(send);
  snprintf(send, sizeof(send),"TOTAL DEPOSIT FILLS: %d", new_race->n_fills);
  log_out(send);
  int give_up;
  for(int i = 0; i < (sizeof(tabela)/sizeof(tabela[0])); i++){
    if(tabela[i].state == 3){
      give_up++;
    }
  }
  snprintf(send, sizeof(send),"TOTAL CARS STILL IN TRACKS: %d", new_race->n_not_finish);
  log_out(send);

}
