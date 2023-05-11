#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main(int argc, char **argv) {
    int sockfd, port;
    struct sockaddr_in broadcastAddr, clientAddr;
    socklen_t clientLen;
    char buffer[BUFFER_SIZE];

    if (argc < 3) {
        fprintf(stderr, "usage: %s port", argv[0]);
        return 1;
    }

    port = atol(argv[1]);

    /* Create UDP socket */
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        return 1;
    }

    /* Set up address interface */
    memset(&broadcastAddr, 0, sizeof(broadcastAddr));
    broadcastAddr.sin_family = AF_INET;
    broadcastAddr.sin_addr.s_addr = inet_addr("255.255.255.255");
    broadcastAddr.sin_port = htons(port);

    /* Bind socket to the server address */
    if (bind(sockfd, (struct sockaddr *)&broadcastAddr, sizeof(broadcastAddr)) < 0) {
        perror("Binding failed");
        return 3;
    }

    printf("Broadcast client started. Receiving packets...\n");
    while (1) {
        /* Receive broadcast packets */
        clientLen = sizeof(clientAddr);
        ssize_t recvBytes = recvfrom(sockfd, buffer, BUFFER_SIZE - 1, 0, (struct sockaddr *)&clientAddr, &clientLen);
        if (recvBytes < 0) {
            perror("Broadcast packet received failed");
            return 4;
        }

        buffer[recvBytes] = '\0';
        printf("Received message from (%s, %d): %s\n", inet_ntoa(senderAddr.sin_addr), senderAddr.sin_port, buffer);
    }

    /* Close the socket */
    close(sockfd);

    return 0;
}