#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char **argv) {
    int sockfd, port;
    struct sockaddr_in multicastAddr;
    const char *buffer = "Hello, multicast!";
    char *multicast_group;

    if (argc < 3) {
        fprintf(stderr, "usage: %s multicast_group port\n", argv[0]);
        return 1;
    }

    multicast_group = argv[1];
    port = atol(argv[2]);

    /* Create UDP socket */
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        return 2;
    }

    /* Setup multicast address interface */
    memset(&multicastAddr, 0, sizeof(multicastAddr));
    multicastAddr.sin_family = AF_INET;
    multicastAddr.sin_addr.s_addr = inet_addr(multicast_group);
    multicastAddr.sin_port = htons(port);

    /* Join multicast group */
    struct ip_mreq multicastRequest;
    multicastRequest.imr_multiaddr.s_addr = inet_addr(multicast_group);
    multicastRequest.imr_interface.s_addr = htonl(INADDR_ANY);
    if (setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void *)&multicastRequest, sizeof(multicastRequest)) < 0) {
        perror("Joining multicast group failed");
        return 3;
    }

    printf("Multicast server started. Sending packets...\n");
    while (1) {
        /* Send multicast buffer */
        if (sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&multicastAddr, sizeof(multicastAddr)) < 0) {
            perror("Send multicast packet failed");
            return 4;
        }
        printf("Sent multicast packet.\n");

        sleep(5);
    }

    /* Close the socket */
    close(sockfd);

    return 0;
}
