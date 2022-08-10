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
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>


#define BUF_SIZE 1024
#define USER_SIZE 128
#define P2P 2000

void erro(char *msg);

void listen_p2p(int fd_p2p);

void listen_normal(int fd_normal);

void sigint(int signum);

pid_t pid1, pid2;

int port;
struct sockaddr_in addr_normal, addr_p2p;
socklen_t udp_len_normal, udp_len_p2p;
int recv_len;
char p2p[4];
char client_server[4];
char groups[4];

int main(int argc, char *argv[]) {
    signal(SIGINT, sigint);
    if (argc != 3) {
        erro("cliente <endereço do servidor> <porto>\n");
    }
    port = atoi(argv[2]);
    //Modo Cliente
    //Criação do socket UDP Server
    struct hostent *hostPtr;
    if ((hostPtr = gethostbyname(argv[1])) == 0) {
        erro("Não consegui obter endereço");
    }
    struct sockaddr_in addr;
    socklen_t udp_len = sizeof(addr);
    int fd;
    bzero((void *) &addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons((short) port);
    addr.sin_addr.s_addr = ((struct in_addr *) (hostPtr->h_addr))->s_addr;
    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        erro("socket");
    }
    //User Authentication
    char message[BUF_SIZE];
    char username[BUF_SIZE];
    char password[BUF_SIZE];
    strcpy(message, "0");
    if ((sendto(fd, message, strlen(message), 0, (struct sockaddr *) &addr, udp_len)) == -1) {
        erro("Erro no sendto");
    }
    printf("Username: ");
    scanf(" %1023[^\n]", username);
    printf("Password: ");
    scanf(" %1023[^\n]", password);
    snprintf(message, BUF_SIZE * 2 + 2, "%s - %s", username, password);
    if ((sendto(fd, message, strlen(message), 0, (struct sockaddr *) &addr, udp_len)) == -1) {
        erro("Erro no sendto");
    }

    //Receives Information about User Priviliges
    if ((recv_len = recvfrom(fd, message, BUF_SIZE, 0, (struct sockaddr *) &addr, (socklen_t * ) & udp_len)) == -1) {
        erro("Erro no recvfrom\n");
    }
    message[recv_len] = '\0';
    if (strcmp(message, "!!!! - Authentication Failed - !!!!") == 0) {
        printf("%s\n", message);
        exit(0);
    }
    printf("%s\n\n", message);
    if ((recv_len = recvfrom(fd, message, BUF_SIZE, 0, (struct sockaddr *) &addr, (socklen_t * ) & udp_len)) == -1) {
        erro("Erro no recvfrom\n");
    }
    message[recv_len] = '\0';
    char *token;
    token = strtok(message, " - ");
    strcpy(client_server, token);
    token = strtok(NULL, " - ");
    strcpy(p2p, token);
    token = strtok(NULL, " - ");
    strcpy(groups, token);


    //Modo Cliente
    /*//Criação do socket UDP multicast e client-server
    udp_len_normal = sizeof(addr_normal);
    int fd_normal;
    bzero((void *) &addr_normal, sizeof(addr_normal));
    addr_normal.sin_family = AF_INET;
    addr_normal.sin_port = htons((short) port);
    addr_normal.sin_addr.s_addr = htonl(INADDR_ANY);
    if ((fd_normal = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
      erro("socket");
    }
    if (bind(fd_normal, (struct sockaddr *) &addr_normal, sizeof(addr_normal)) == -1) {
      erro("Erro no bind");
    }
    //Escuta Portos
    if ((pid1 = fork()) == 0) {
      listen_normal(fd_normal);
      exit(0);
    }
    //Criação do socket UDP P2P
    udp_len_p2p = sizeof(addr_p2p);
    int fd_p2p;
    bzero((void *) &addr_p2p, sizeof(addr_p2p));
    addr_p2p.sin_family = AF_INET;
    addr_p2p.sin_port = htons((short) P2P);
    addr_p2p.sin_addr.s_addr = htonl(INADDR_ANY);
    if ((fd_p2p = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        erro("socket");
    }
    if (bind(fd_p2p, (struct sockaddr *) &addr_p2p, sizeof(addr_p2p)) == -1) {
        erro("Erro no bind");
    }
    //Escuta P2P
    if ((pid2 = fork()) == 0) {
        listen_p2p(fd_p2p);
        exit(0);
    }*/

    //Interface cliente
    char escolha[2];
    while (1) {
        memset(message, 0, sizeof(message));
        printf("#### - Messaging System - ####\n1 - Cient-Server\n2 - P2P\n3 - Group\n4 - Quit\nSelect => ");
        scanf(" %2[^\n]", escolha);
        strcpy(message, escolha);
        if (escolha[0] == '1') {
            //Cliente-Server
            if (strcmp(client_server, "yes") == 0) {
                sleep(0.5);
                if ((sendto(fd, message, strlen(message), 0, (struct sockaddr *) &addr, udp_len)) == -1) {
                    printf("Erro no sendto");
                }
                char user[USER_SIZE];
                char text[BUF_SIZE];
                printf("Which User: ");
                fflush(stdout);
                scanf(" %127[^\n]", user);
                printf("Message to send: ");
                scanf(" %1023[^\n]", text);
                snprintf(message, BUF_SIZE * 2, "%s - %s", text, user);
                sleep(0.5);
                if ((sendto(fd, message, strlen(message), 0, (struct sockaddr *) &addr, udp_len)) == -1) {
                    printf("Erro no sendto");
                }
                if ((recv_len = recvfrom(fd, message, BUF_SIZE, 0, (struct sockaddr *) &addr,
                                         (socklen_t * ) & udp_len)) == -1) {
                    printf("Erro no recvfrom");
                }
                message[recv_len] = '\0';
                printf("%s\n", message);
            } else {
                printf("Opção não permitida.\n");
            }
        } else if (escolha[0] == '2') {
            //P2P
            if (strcmp(p2p, "yes") == 0) {
                sleep(0.5);
                if ((sendto(fd, message, strlen(message), 0, (struct sockaddr *) &addr, udp_len)) == -1) {
                    printf("Erro no sendto");
                }
                char user[USER_SIZE];
                char text[BUF_SIZE];
                printf("Which User: ");
                scanf(" %127[^\n]", user);
                snprintf(message, USER_SIZE, "%s", user);
                sleep(0.5);
                if ((sendto(fd, message, strlen(message), 0, (struct sockaddr *) &addr, udp_len)) == -1) {
                    printf("Erro no sendto");
                }
                if ((recv_len = recvfrom(fd, message, BUF_SIZE, 0, (struct sockaddr *) &addr,
                                         (socklen_t * ) & udp_len)) == -1) {
                    printf("Erro no recvfrom");
                }
                message[recv_len] = '\0';
                if (strcmp(message, "Utilizador não existente!") == 0) {
                    printf("%s\n", message);
                } else {
                    char *token;
                    token = strtok(message, ":");
                    struct hostent *hostPtr;
                    if ((hostPtr = gethostbyname(token)) == 0) {
                        printf("Couldn't obtain IP Address");
                    }
                    struct sockaddr_in addr_p2p;
                    socklen_t udp_len_p2p = sizeof(addr_p2p);
                    int fd_p2p1;
                    bzero((void *) &addr_p2p, sizeof(addr_p2p));
                    addr_p2p.sin_family = AF_INET;
                    addr_p2p.sin_port = htons((short) atoi(message));
                    addr_p2p.sin_addr.s_addr = ((struct in_addr *) (hostPtr->h_addr))->s_addr;
                    if ((fd_p2p1 = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
                        printf("Erro no socket");
                    }
                    printf("Obtained IP of %s...\n", user);
                    printf("Message to send: ");
                    scanf(" %1023[^\n]", text);
                    strcpy(message, text);
                    sleep(0.5);
                    if ((sendto(fd_p2p1, message, strlen(message), 0, (struct sockaddr *) &addr_p2p, udp_len_p2p)) ==
                        -1) {
                        printf("Message not sent!");
                    } else {
                        printf("Message sent!");
                    }
                    close(fd_p2p1);
                }
            } else {
                printf("Opção não permitida.\n");
            }
        } else if (escolha[0] == '3') {
            //Grupos
            if (strcmp(groups, "yes") == 0) {
                sleep(0.5);
                if ((sendto(fd, message, strlen(message), 0, (struct sockaddr *) &addr, udp_len)) == -1) {
                    printf("Erro no sendto\n");
                }
                int multicastTTL = 255;
                if (setsockopt(fd, IPPROTO_IP, IP_MULTICAST_TTL, (void *) &multicastTTL,
                               sizeof(multicastTTL)) < 0) {
                    perror("socket opt");
                    return 1;
                }
            } else {
                printf("Opção não permitida.\n");
            }
        } else if (escolha[0] == '4') {
            //Quit
            close(fd);
            break;
        } else {
            printf("Escolha inválida!");
        }
    }
}

void listen_p2p(int fd_p2p) {
    char message[BUF_SIZE];
    while (1) {
        if ((recv_len = recvfrom(fd_p2p, message, BUF_SIZE, 0, (struct sockaddr *) &addr_p2p,
                                 (socklen_t * ) & udp_len_p2p)) == -1) {
            printf("Erro no recvfrom");
        }
        message[recv_len] = '\0';
        printf("%s\n", message);
    }
}

void listen_normal(int fd_normal) {
    char message[BUF_SIZE];
    while (1) {
        if ((recv_len = recvfrom(fd_normal, message, BUF_SIZE, 0, (struct sockaddr *) &addr_normal,
                                 (socklen_t * ) & udp_len_normal)) == -1) {
            printf("Erro no recvfrom");
        }
        message[recv_len] = '\0';
        printf("%s\n", message);
    }
}

void sigint(int signum) {
    kill(pid1, SIGKILL);
    kill(pid2, SIGKILL);
    close(0);
}

void erro(char *msg) {
    printf("Erro: %s\n", msg);
    exit(-1);
}
