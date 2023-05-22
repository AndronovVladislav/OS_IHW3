#ifndef IHW3_UTILS_H
#define IHW3_UTILS_H

#define SELLERS_AMOUNT 3
#define QUEUE_REGION "queue"
#define SEMAPHORES_REGION "semaphores"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define PRINT_GREEN(X) printf("%s%d%s ", ANSI_COLOR_GREEN, X, ANSI_COLOR_RESET)

#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/socket.h> /* for socket(), bind(), and connect() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */

typedef struct client {
    int currentSeller;
    int sellersToVisit[3];
} client;

#define RCVBUFSIZE sizeof(client)
#define MAXPENDING 5    /* Maximum outstanding connection requests */

void (*prevHandler)(int);

// const char *sellers_names[] = {"/seller0", "/seller1", "/seller2"};
// char pathname[] = "main.c";
// char pathname_separated_files[] = "sellers.c";
// sem_t *sellers[3] = {NULL, NULL, NULL};
// sem_t *sellers_shm;


void DieWithError(char *errorMessage);  /* Error handling function */
void HandleTCPClient(int clntSocket, int clientsAmount);   /* TCP client handling function */
int CreateTCPServerSocket(unsigned short port); /* Create TCP server socket */
int AcceptTCPConnection(int servSock);  /* Accept TCP connection request */
void PrintClients(int clientsAmount);

#endif //IHW3_UTILS_H