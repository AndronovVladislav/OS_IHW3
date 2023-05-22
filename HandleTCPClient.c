#include "utils.h"

void HandleTCPClient(int clntSocket, int clientsAmount) {
    // client clientBuffer;
    int sellerNumber;
    int shmid;
    if ((shmid = shm_open(QUEUE_REGION, O_RDWR, S_IRWXU)) == -1) {
        perror("shm_open");
        exit(-1);
    }

    client* clients = mmap(0, sizeof(client) * clientsAmount, PROT_WRITE | PROT_READ, MAP_SHARED, shmid, 0);

    if (recv(clntSocket, &sellerNumber, sizeof(sellerNumber), 0) < 0) {
        DieWithError("recv() failed");
    }

    printf("sellerNumber = %d\n", sellerNumber);
    for (int i = 0; i < clientsAmount; ++i) {
        if (clients[i].currentSeller <= 2 && clients[i].sellersToVisit[clients[i].currentSeller] == sellerNumber) {
            sleep(3);
            while (clients[i].currentSeller <= 2 && clients[i].sellersToVisit[++clients[i].currentSeller] == 0);
        }
    }

    if (send(clntSocket, &sellerNumber, sizeof(sellerNumber), 0) != sizeof(sellerNumber)) {
        DieWithError("send() failed");
    }

    PrintClients(clientsAmount);
    close(clntSocket);
}
