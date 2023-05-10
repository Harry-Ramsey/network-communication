#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main(int argc, char **argv) {
    struct sockaddr_in multicastAddr, senderAddr;
    int sockfd;
    char *multicast_group; 
    int port;

    if (argc < 3) {
        fprintf(stderr, "usage: %s multicast_group port", argv[0]);
        return 1;
    }

    multicast_group = argv[1];
    port = atol(argv[2]);

    /* Create UDP socket */
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        return 2;
    }

    /* Setup address interface */
    memset(&multicastAddr, 0, sizeof(multicastAddr));
    multicastAddr.sin_family = AF_INET;
    multicastAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    multicastAddr.sin_port = htons(port);

    /* Bind socket to address interface */ 
    if (bind(sockfd, (struct sockaddr *)&multicastAddr, sizeof(multicastAddr)) < 0) {
        perror("Binding failed");
        return 3;
    }

    /* Join multicast group */
    struct ip_mreq multicastRequest;
    multicastRequest.imr_multiaddr.s_addr = inet_addr(multicast_group);
    multicastRequest.imr_interface.s_addr = htonl(INADDR_ANY);
    if (setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void *)&multicastRequest, sizeof(multicastRequest)) < 0) {
        perror("Joining multicast group failed");
        return 4;
    }


    printf("Multicast client started. Receiving messages...\n");
    while (1) {
        char buffer[BUFFER_SIZE];
        socklen_t senderLen = sizeof(senderAddr);

        // Receive multicast messages
        ssize_t recvBytes = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&senderAddr, &senderLen);
        if (recvBytes < 0) {
            perror("Receive error");
            return 5;
        }

        buffer[recvBytes] = '\0';
        printf("Received message from (%s, %d): %s\n", inet_ntoa(senderAddr.sin_addr), senderAddr.sin_port, buffer);

        sleep(5);
    }

    /* Close the socket */
    close(sockfd);

    return 0;
}
