#include "../utils.h"  /* TCP echo server includes */

void ctrlCHandler(int nsig) {
    // int shmid;
    // if ((shmid = shm_open(QUEUE_REGION, O_CREAT | O_RDWR, S_IRWXU)) == -1) {
    //     perror("shm_open");
    //     exit(-1);
    // }
    // int *forming_queue = mmap(0, sizeof(client) * ???, PROT_WRITE | PROT_READ, MAP_SHARED, shmid, 0);
    // for (int i = 0; i < SELLERS_AMOUNT; ++i) {
    //     forming_queue[i] = 2;
    // }

    // close(shmid);
    shm_unlink(QUEUE_REGION);
    exit(0);
}

void CreateClients(int clientsAmount) {
    int shmid;
    if ((shmid = shm_open(QUEUE_REGION, O_CREAT | O_RDWR, S_IRWXU)) == -1) {
        perror("shm_open");
        exit(-1);
    }

    if (ftruncate(shmid, sizeof(client) * clientsAmount) == -1) {
        perror("ftruncate");
        exit(-1);
    }

    client* clients = mmap(0, sizeof(client) * clientsAmount, PROT_WRITE | PROT_READ, MAP_SHARED, shmid, 0);

    for (int i = 0; i < clientsAmount; ++i) {
        srand(time(NULL));
        int busy[4] = {0, 0, 0, 0};

        for (int j = 0; j < SELLERS_AMOUNT; ++j) {
            while (1) {
                int potentialSeller = (rand() % 4);

                if (!busy[potentialSeller]) {
                    clients[i].sellersToVisit[j] = potentialSeller;
                    busy[potentialSeller] = 1;
                    break;
                }
            }
        }

        for (int j = 0; j < SELLERS_AMOUNT; ++j) {
            if (clients[i].sellersToVisit[j]) {
                clients[i].currentSeller = j;
                break;
            }
        }

        sleep(1);
    }
}

void PrintClients(int clientsAmount) {
    int shmid;
    if ((shmid = shm_open(QUEUE_REGION, O_RDWR, S_IRWXU)) == -1) {
        perror("shm_open");
        exit(-1);
    }

    client* clients = mmap(0, sizeof(client) * clientsAmount, PROT_WRITE | PROT_READ, MAP_SHARED, shmid, 0);

    for (int i = 0; i < clientsAmount; ++i) {
        printf("%d ", clients[i].currentSeller);
        for (int j = 0; j < SELLERS_AMOUNT; ++j) {
            if (j == clients[i].currentSeller) {
                PRINT_GREEN(clients[i].sellersToVisit[j]);
            } else {
                printf("%d ", clients[i].sellersToVisit[j]);
            }
        }
        printf("\n");
    }
}

int UnservedClients(int clientsAmount) {
    int shmid;
    if ((shmid = shm_open(QUEUE_REGION, O_RDWR, S_IRWXU)) == -1) {
        perror("shm_open");
        exit(-1);
    }

    client* clients = mmap(0, sizeof(client) * clientsAmount, PROT_WRITE | PROT_READ, MAP_SHARED, shmid, 0);

    for (int i = 0; i < clientsAmount; ++i) {
        if (clients[i].currentSeller != 3) {
            return 1;
        }
    }

    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage:  %s <Server Port> <Amount of Clients>\n", argv[0]);
        exit(1);
    }
    prevHandler = signal(SIGINT, ctrlCHandler);

    int clntSock;                    /* Socket descriptor for client */
    pid_t processID;                 /* Process ID from fork() */
    unsigned int childProcCount = 0; /* Number of child processes */ 

    int echoServPort = atoi(argv[1]);  /* First arg:  local port */
    int servSock = CreateTCPServerSocket(echoServPort);
    int clntAmount = atoi(argv[2]);

    CreateClients(clntAmount);
    PrintClients(clntAmount);


    for (;;) {
        clntSock = AcceptTCPConnection(servSock);
        // printf("%d\n", clntSock);

        if ((processID = fork()) < 0) {
            DieWithError("fork() failed");
        } else if (processID == 0) {
            signal(SIGINT, prevHandler);

            close(servSock);   /* Child closes parent socket */
            HandleTCPClient(clntSock, clntAmount);
            exit(0);           /* Child process terminates */
        }

        printf("with child process: %d\n", processID);
        close(clntSock);       /* Parent closes child socket descriptor */
        childProcCount++;      /* Increment number of outstanding child processes */

        while (childProcCount) {
            processID = waitpid((pid_t) -1, NULL, WNOHANG);  /* Non-blocking wait */
            if (processID < 0)  /* waitpid() error? */ {
                DieWithError("waitpid() failed");
            } else if (processID == 0)  /* No zombie to wait on */ {
                break;
            } else {
                childProcCount--;  /* Cleaned up after a child */
            }
        }
    }
    
    shm_unlink(QUEUE_REGION);
    exit(0);
}
