#include "../Other/utils.h"

void ctrlCHandler(int nsig) {
    shm_unlink(QUEUE_REGION);
    exit(0);
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

    for (;;) {
        clntSock = AcceptTCPConnection(servSock);

        if ((processID = fork()) < 0) {
            DieWithError("fork() failed");
        } else if (processID == 0) {
            signal(SIGINT, prevHandler);

            close(servSock);   /* Child closes parent socket */
            HandleTCPClient1(clntSock, clntAmount);
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
