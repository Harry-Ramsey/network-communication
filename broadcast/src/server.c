#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char **argv) {
    int sockfd, port;
    struct sockaddr_in broadcastAddr;
    const char *buffer = "Hello, broadcast!";
    int broadcastEnable = 1;

    if (argc < 2) {
        fprintf(stderr, "usage: %s port\n", argv[0]);
        return 1;
    }

    port = atol(argv[1]);

    /* Create UDP socket */
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        return 2;
    }

    /* Set socket options to enable broadcast */
    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable)) < 0) {
        perror("Setting socket options failed");
        return 3;
    }

    /* Set up broadcast address */
    memset(&broadcastAddr, 0, sizeof(broadcastAddr));
    broadcastAddr.sin_family = AF_INET;
    broadcastAddr.sin_addr.s_addr = inet_addr("255.255.255.255");
    broadcastAddr.sin_port = htons(port);

    while (1) {
        /* Send the broadcast packet */
        if (sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&broadcastAddr, sizeof(broadcastAddr)) < 0) {
            perror("Send broadcast packet failed");
            return 4;
        }
        printf("Sent broadcast packet.\n");

        sleep(5);
    }

    /* Close the socket */
    close(sockfd);

    return 0;
}