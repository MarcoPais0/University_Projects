//Marco da Cruz Pais Nº2019218981
//Simão Carvalho Monteiro Nº2019215412

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUF_SIZE 1024
#define USER_SIZE 128
#define CLIENTS_PORT 10000

void process_client(int socket_udp, struct sockaddr_in client_addr, char *message);

void server_control(int client_socket_tcp);

int authentication(char *message);

void read_fich();

void write_fich();

void erro(char *msg);

void sigint(int signum);

void sigint1(int signum);

typedef struct {
    char user[USER_SIZE];
    struct sockaddr_in address;
    char password[USER_SIZE];
    char client_server[4];
    char p2p[4];
    char group[4];
} user_credentials;

int admin_fd;
user_credentials users[BUF_SIZE];
user_credentials *users_p = &users[0];
int count_p = 0;
int *count = &count_p;
int udp_port;
int tcp_port;
char *fich_name;
int shmid;
void *shm;
struct sockaddr_in udp_server, udp_client;
socklen_t udp_len;

int main(int argc, char *argv[]) {
    if (argc != 4) {
        erro("server <porto de clientes> <porto config> <ficheiro de registos>");
    }

    udp_port = atoi(argv[1]);
    tcp_port = atoi(argv[2]);
    fich_name = argv[3];

    //Sinal para interrupção do programa
    signal(SIGINT, sigint);

    //Cria memória partilhada para todos os processos terem acesso ao array users
    if ((shmid = shmget(IPC_PRIVATE, sizeof(users) + sizeof(int), IPC_CREAT | 0766)) < 0) {
        perror("Error in shmget with IPC_CREAT\n");
        exit(1);
    }
    shm = shmat(shmid, NULL, 0);
    count = (int *) shm;
    users_p = (user_credentials *) shm + sizeof(int);
    memset(users_p, 0, sizeof(users));

    //Leitura do ficheiro credentials
    read_fich();

    // Cria um socket UDP
    int socket_udp, recv_len;
    struct sockaddr_in udp_server, udp_client;
    udp_len = sizeof(udp_server);
    udp_server.sin_family = AF_INET;
    udp_server.sin_port = htons(udp_port);
    udp_server.sin_addr.s_addr = htonl(INADDR_ANY);
    if ((socket_udp = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
        erro("Erro na criação do socket");
    // Associa o socket à informação de endereço
    if (bind(socket_udp, (struct sockaddr *) &udp_server, sizeof(udp_server)) == -1)
        erro("Erro no bind");

    //Cria um socket TCP
    char ip_address[INET_ADDRSTRLEN];
    int socket_tcp;
    struct sockaddr_in addr, tcp_addr;
    int tcp_addr_size;
    bzero((void *) &addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(tcp_port);
    tcp_addr_size = sizeof(tcp_addr);
    if ((socket_tcp = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        erro("na funcao socket");
    if (bind(socket_tcp, (struct sockaddr *) &addr, sizeof(addr)) != 0)
        erro("na funcao bind");
    if (listen(socket_tcp, 5) != 0)
        erro("na funcao listen");
    if (inet_ntop(AF_INET, &(addr.sin_addr), ip_address, INET_ADDRSTRLEN) == NULL)
        erro("nao foi possivel converter endereco de IP");
    char message[BUF_SIZE];
    if (fork() == 0) {
        signal(SIGINT, sigint1);
        //Espera mensagens udp
        while (1) {
            //wait for new packet
            if ((recv_len = recvfrom(socket_udp, message, BUF_SIZE, 0, (struct sockaddr *) &udp_client,
                                     (socklen_t * ) & udp_len)) == -1) {
                printf("Erro no recvfrom\n");
            }
            message[recv_len] = '\0';
            process_client(socket_udp, udp_client, message);
        }
    } else {
        //Espera mensagens tcp
        while (1) {
            //clean finished child processes, avoiding zombies
            //must use WNOHANG or would block whenever a child process was working
            while (waitpid(-1, NULL, WNOHANG) > 0);
            //wait for new connection
            admin_fd = accept(socket_tcp, (struct sockaddr *) &tcp_addr, (socklen_t * ) & tcp_addr_size);
            if (admin_fd > 0) {
                if (fork() == 0) {
                    close(socket_tcp);
                    server_control(admin_fd);
                    exit(0);
                }
                close(admin_fd);
            }
        }
    }
    //Atualiza o ficheiro antes de fechar e limpa recursos
    write_fich();
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}

void process_client(int socket_udp, struct sockaddr_in client_addr, char *message) {
    socklen_t udp_len = sizeof(client_addr);
    int i;
    int recv_len;
    if (message[0] == '0') {
        //Autenticar
        if ((recv_len = recvfrom(socket_udp, message, BUF_SIZE, 0, (struct sockaddr *) &client_addr,
                                 (socklen_t * ) & udp_len)) == -1) {
            printf("Erro 2 no recvfrom\n");
        }
        message[recv_len] = '\0';
        if ((i = authentication(message)) >= 0) {
            sleep(0.5);
            snprintf(message, BUF_SIZE,
                     "#### - Authentication Successfull - ####\nUser Priviligies\nCliente-Servidor: %s\nP2P: %s\nGrupo: %s",
                     users_p[i].client_server, users_p[i].p2p, users_p[i].group);
            if ((sendto(socket_udp, message, strlen(message), 0, (struct sockaddr *) &client_addr, udp_len)) == -1)
                printf("Erro no sendto");
            snprintf(message, BUF_SIZE, "%s - %s - %s", users_p[i].client_server, users_p[i].p2p, users_p[i].group);
            if ((sendto(socket_udp, message, strlen(message), 0, (struct sockaddr *) &client_addr, udp_len)) == -1)
                printf("Erro no sendto");
        } else {
            strcpy(message, "!!!! - Authentication Failed - !!!!");
            if ((sendto(socket_udp, message, strlen(message), 0, (struct sockaddr *) &client_addr, udp_len)) == -1)
                printf("Erro no send to");
        }
    } else if (message[0] == '1') {
        //Client-Server
        char send[BUF_SIZE];
        char receiver[BUF_SIZE];
        char ip_receiver[INET_ADDRSTRLEN];
        char *token;
        if ((recv_len = recvfrom(socket_udp, message, BUF_SIZE, 0, (struct sockaddr *) &client_addr,
                                 (socklen_t * ) & udp_len)) == -1) {
            printf("Erro no recvfrom");
        }
        message[recv_len] = '\0';
        token = strtok(message, " - ");
        strcpy(send, token);
        token = strtok(NULL, " - ");
        strcpy(receiver, token);
        if (send[0] == '0' || receiver[0] == '0') {
            strcpy(message, "Invalid Data!");
            sleep(0.5);
            if ((sendto(socket_udp, message, strlen(message), 0, (struct sockaddr *) &client_addr, udp_len)) == -1)
                printf("Erro no sendto");
        } else {
            int x = 0;
            for (int i = 0; i < *count; i++) {
                if (strcmp(receiver, users_p[i].user) == 0) {
                    if (inet_ntop(AF_INET, &(users_p[i].address.sin_addr), ip_receiver, INET_ADDRSTRLEN) == NULL)
                        printf("nao foi possivel converter endereco de IP");
                    x = 1;
                    break;
                }
            }
            if (x == 0) {
                strcpy(message, "Utilizador não existente!");
                sleep(0.5);
                if ((sendto(socket_udp, message, strlen(message), 0, (struct sockaddr *) &client_addr, udp_len)) == -1)
                    printf("Erro no sendto");
            } else {
                struct hostent *hostPtr;
                if ((hostPtr = gethostbyname(ip_receiver)) == 0) {
                    printf("Couldn't obtain IP Address");
                }
                struct sockaddr_in addr_cs;
                socklen_t udp_len_cs = sizeof(addr_cs);
                int fd_cs;
                bzero((void *) &addr_cs, sizeof(addr_cs));
                addr_cs.sin_family = AF_INET;
                addr_cs.sin_port = htons((short) udp_port);
                addr_cs.sin_addr.s_addr = ((struct in_addr *) (hostPtr->h_addr))->s_addr;
                if ((fd_cs = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
                    printf("socket");
                }
                sleep(0.5);
                if ((sendto(fd_cs, send, strlen(send), 0, (struct sockaddr *) &addr_cs, udp_len_cs)) == -1)
                    printf("Erro no sendto");
                strcpy(message, "Mensagem enviada!");
                if ((sendto(socket_udp, message, strlen(message), 0, (struct sockaddr *) &client_addr, udp_len)) == -1)
                    printf("Erro no sendto");
            }
        }
    } else if (message[0] == '2') {
        //Mensagens P2P
        char ip_receiver[INET_ADDRSTRLEN];
        if ((recv_len = recvfrom(socket_udp, message, BUF_SIZE, 0, (struct sockaddr *) &client_addr,
                                 (socklen_t * ) & udp_len)) == -1) {
            printf("Erro no recvfrom");
        }
        message[recv_len] = '\0';
        int x = 0;
        for (int i = 0; i < *count; i++) {
            if (strcmp(message, users_p[i].user) == 0) {
                if (inet_ntop(AF_INET, &(users_p[i].address.sin_addr), ip_receiver, INET_ADDRSTRLEN) == NULL)
                    printf("nao foi possivel converter endereco de IP");
                x = 1;
                break;
            }
        }
        if (x == 0) {
            sleep(0.5);
            strcpy(message, "Utilizador não existente!");
            if ((sendto(socket_udp, message, strlen(message), 0, (struct sockaddr *) &client_addr, udp_len)) == -1)
                printf("Erro no sendto");
        } else {
            sleep(0.5);
            snprintf(message, BUF_SIZE, "%s:%d", ip_receiver, CLIENTS_PORT);
            if ((sendto(socket_udp, message, strlen(message), 0, (struct sockaddr *) &client_addr, udp_len)) == -1)
                printf("Erro no sendto");
        }
    } else if (message[0] == '3') {
        //Implementar Mensagens de Grupo
    }
}

int authentication(char *message) {
    char *token;
    char nome[USER_SIZE];
    char pass[USER_SIZE];
    token = strtok(message, " - ");
    strcpy(nome, token);
    token = strtok(NULL, " - ");
    strcpy(pass, token);
    for (int i = 0; i < 1024; i++) {
        if (strcmp(nome, users_p[i].user) == 0) {
            if (strcmp(pass, users_p[i].password) == 0) {
                return i;
            }
        }
    }
    return -1;
}

void server_control(int admin_fd) {
    int nread = 0;
    char message[BUF_SIZE];
    char escolha[2];
    while (1) {
        memset(message, 0, sizeof(message));
        memset(escolha, 0, sizeof(escolha));
        strcpy(message,
               "\n####### - Server Command Line Interface - ########\n1 - List\n2 - Add\n3 - Del\n4 - Quit\n\nSelect => ");
        write(admin_fd, message, strlen(message) + 1);
        nread = read(admin_fd, escolha, 2);
        escolha[nread - 1] = '\0';
        if (escolha[0] == '1') {
            //Lista Utilizadores
            for (int i = 0; i < *count; i++) {
                if (users_p[i].user[0] != '\0') {
                    char ip_address[INET_ADDRSTRLEN];
                    if (inet_ntop(AF_INET, &(users_p[i].address.sin_addr), ip_address, INET_ADDRSTRLEN) == NULL)
                        printf("nao foi possivel converter endereco de IP");
                    snprintf(message, BUF_SIZE,
                             "User: %s - IP: %s - Password: %s - Client-Server: %s - P2P: %s - Group: %s\n",
                             users_p[i].user, ip_address, users_p[i].password, users_p[i].client_server, users_p[i].p2p,
                             users_p[i].group);
                    write(admin_fd, message, strlen(message) + 1);
                }
            }
            strcpy(message, "Fim\n");
            write(admin_fd, message, strlen(message) + 1);
        } else if (escolha[0] == '2') {
            //Adiciona Utilizadores
            strcpy(message, "\nInsert New User (<user> - <ip> - <password> - <client-server> - <p2p> - <group>): ");
            write(admin_fd, message, strlen(message) + 1);
            nread = read(admin_fd, message, BUF_SIZE);
            message[nread - 1] = '\0';
            char *token;
            int error = 0;
            struct hostent *hostPtr;
            token = strtok(message, " - ");
            for (int i = 0; i < *count; i++) {
                if (strcmp(users_p[i].user, token) == 0) {
                    error = 1;
                }
            }
            strcpy(users_p[*count].user, token);
            if (strlen(users_p[*count].user) == 0) {
                error = 1;
            }
            token = strtok(NULL, " - ");
            if ((hostPtr = gethostbyname(token)) == 0) {
                error = 1;
            }
            users_p[*count].address.sin_addr.s_addr = ((struct in_addr *) (hostPtr->h_addr))->s_addr;
            token = strtok(NULL, " - ");
            strcpy(users_p[*count].password, token);
            if (strlen(users_p[*count].password) == 0) {
                error = 1;
            }
            token = strtok(NULL, " - ");
            strcpy(users_p[*count].client_server, token);
            if ((strcmp(users_p[*count].client_server, "yes") != 0) &&
                (strcmp(users_p[*count].client_server, "no") != 0)) {
                error = 1;
            }
            token = strtok(NULL, " - ");
            strcpy(users_p[*count].p2p, token);
            if ((strcmp(users_p[*count].p2p, "yes") != 0) && (strcmp(users_p[*count].p2p, "no") != 0)) {
                error = 1;
            }
            token = strtok(NULL, " - ");
            strcpy(users_p[*count].group, token);
            if ((strcmp(users_p[*count].group, "yes") != 0) && (strcmp(users_p[*count].group, "no") != 0)) {
                error = 1;
            }
            if (error == 0) {
                strcpy(message, "User added successfully\n");
                write(admin_fd, message, strlen(message) + 1);
                (*count)++;
            } else {
                strcpy(message, "User added unsuccessfully\n");
                write(admin_fd, message, strlen(message) + 1);
                memset(&users_p[*count], 0, sizeof(user_credentials));
            }
        } else if (escolha[0] == '3') {
            //Remove Utilizadores
            int removido = 0;
            strcpy(message, "\nInsert User ID to delete: ");
            write(admin_fd, message, strlen(message) + 1);
            nread = read(admin_fd, message, BUF_SIZE);
            message[nread - 1] = '\0';
            for (int i = 0; i < *count; i++) {
                if (strcmp(message, users_p[i].user) == 0) {
                    users_p[i].user[0] = '\0';
                    strcpy(message, "User deleted successfully\n");
                    write(admin_fd, message, strlen(message) + 1);
                    removido = 1;
                }
            }
            if (removido == 0) {
                strcpy(message, "User deleted unsuccessfully\n");
                write(admin_fd, message, strlen(message) + 1);
            }
        } else if (escolha[0] == '4') {
            //Sai da Interface da Linha de Comandos
            close(admin_fd);
            break;
        } else {
            //Comando Inexistente
            strcpy(message, "Option not valid\n");
            write(admin_fd, message, strlen(message) + 1);
        }
    }
}

void read_fich() {
    FILE *fp;
    char line[BUF_SIZE];
    fp = fopen(fich_name, "r");
    if (fp == NULL) {
        printf("Ficheiro '%s' não existe.", fich_name);
    }
    while (fgets(line, BUF_SIZE, fp) != NULL) {
        line[strlen(line) - 1] = '\0';
        char *token;
        struct hostent *hostPtr;
        token = strtok(line, " - ");
        strcpy(users_p[*count].user, token);
        token = strtok(NULL, " - ");
        if ((hostPtr = gethostbyname(token)) == 0) {
            erro("Não consegui obter endereço");
        }
        users_p[*count].address.sin_addr.s_addr = ((struct in_addr *) (hostPtr->h_addr))->s_addr;
        token = strtok(NULL, " - ");
        strcpy(users_p[*count].password, token);
        token = strtok(NULL, " - ");
        strcpy(users_p[*count].client_server, token);
        token = strtok(NULL, " - ");
        strcpy(users_p[*count].p2p, token);
        token = strtok(NULL, " - ");
        strcpy(users_p[*count].group, token);
        memset(line, 0, BUF_SIZE);
        (*count)++;
    }
    fclose(fp);
}

void write_fich() {
    FILE *fp;
    fp = fopen(fich_name, "w");
    if (fp == NULL) {
        printf("Ficheiro 'credentials.txt' não existe.");
    }
    for (int i = 0; i < *count; i++) {
        if (users_p[i].user[0] != '\0') {
            char ip_address[INET_ADDRSTRLEN];
            if (inet_ntop(AF_INET, &(users_p[i].address.sin_addr), ip_address, INET_ADDRSTRLEN) == NULL) {
                printf("nao foi possivel converter endereco de IP");
            } else {
                fprintf(fp, "%s - %s - %s - %s - %s - %s\n", users_p[i].user, ip_address,
                        users_p[i].password, users_p[i].client_server, users_p[i].p2p, users_p[i].group);
            }
        }
    }
    fclose(fp);
}

void erro(char *msg) {
    printf("Erro: %s\n", msg);
    exit(-1);
}

void sigint(int signum) {
    signal(SIGINT, SIG_IGN);
    write_fich();
    shmctl(shmid, IPC_RMID, NULL);
    exit(0);
}

void sigint1(int signum) {
    signal(SIGINT, SIG_IGN);
    exit(0);
}
