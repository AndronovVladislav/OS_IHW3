#include "../Other/utils.h"

int main(int argc, char *argv[]) {
    if ((argc < 3) || (argc > 4)) {
       fprintf(stderr, "Usage: %s <Server IP> <Number of Seller> [<Echo Port>]\n",
               argv[0]);
       exit(1);
    }

    int sock;                        /* Socket descriptor */
    struct sockaddr_in echoServAddr; /* Echo server address */
    client clientBuffer;             /* Buffer for echo string */
    int bytesRcvd, totalBytesRcvd;   /* Bytes read in single recv()
                                        and total bytes read */

    char *servIP = argv[1];
    int sellerNumber = atoi(argv[2]);
    int echoServPort = argc == 4 ? atoi(argv[3]) : 7;

    /* Create a reliable, stream socket using TCP */
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        DieWithError("socket() failed");
    }

    /* Construct the server address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));     /* Zero out structure */
    echoServAddr.sin_family      = AF_INET;             /* Internet address family */
    echoServAddr.sin_addr.s_addr = inet_addr(servIP);   /* Server IP address */
    echoServAddr.sin_port        = htons(echoServPort); /* Server port */

    /* Establish the connection to the echo server */
    if (connect(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0) {
        DieWithError("connect() failed");
    }

    /* Receive the same string back from the server */
    totalBytesRcvd = 0;
    while (totalBytesRcvd < sizeof(sellerNumber)) {
        if (send(sock, &sellerNumber, sizeof(sellerNumber), 0) != sizeof(sellerNumber)) {
            DieWithError("send() failed");
        }
        printf("I sent %d seller number\n", sellerNumber);
        

        if ((bytesRcvd = recv(sock, &sellerNumber, sizeof(sellerNumber), 0)) <= 0) {
            DieWithError("recv() failed or connection closed prematurely");
        }
        printf("I received %d seller number\n", sellerNumber);

        totalBytesRcvd += bytesRcvd; /* Keep tally of total bytes */
    }

    close(sock);
    exit(0);
}
