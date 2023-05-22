#include "utils.h"

void HandleTCPClient1(int clntSocket, int clientsAmount) {
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
    printf("I received %d seller number\n", sellerNumber);

    for (int i = 0; i < clientsAmount; ++i) {
        if (clients[i].currentSeller <= 2 && clients[i].sellersToVisit[clients[i].currentSeller] == sellerNumber) {
            sleep(rand() % 3 + 1);
            while (clients[i].currentSeller <= 2 && clients[i].sellersToVisit[++clients[i].currentSeller] == 0);
        }
    }

    if (send(clntSocket, &sellerNumber, sizeof(sellerNumber), 0) != sizeof(sellerNumber)) {
        DieWithError("send() failed");
    }
    printf("I sent %d seller number\n", sellerNumber);

    close(clntSocket);
}

void HandleTCPClient2(int clntSocket, int clientsAmount, int obsrvSock) {
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

    for (int i = 0; i < clientsAmount; ++i) {
        if (clients[i].currentSeller <= 2 && clients[i].sellersToVisit[clients[i].currentSeller] == sellerNumber) {
            sleep(rand() % 3 + 1);
            while (clients[i].currentSeller <= 2 && clients[i].sellersToVisit[++clients[i].currentSeller] == 0);
        }
    }

    if (send(clntSocket, &sellerNumber, sizeof(sellerNumber), 0) != sizeof(sellerNumber)) {
        DieWithError("send1() failed");
    }

    if (send(obsrvSock, &sellerNumber, sizeof(sellerNumber), 0) != sizeof(sellerNumber)) {
        DieWithError("send2() failed");
    }

    close(clntSocket);
}
