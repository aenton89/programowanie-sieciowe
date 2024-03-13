#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <ctype.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Sposób użycia: %s <port>\n", argv[0]);
        return 1;
    }

    int sock;
    int rc;         // "rc" to skrót słów "result code"
    ssize_t cnt;    // wyniki zwracane przez recvfrom() i sendto() są tego typu

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        perror("socket");
        return 1;
    }

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_addr = { .s_addr = htonl(INADDR_ANY) },
        .sin_port = htons(atoi(argv[1]))
    };

    rc = bind(sock, (struct sockaddr *)&addr, sizeof(addr));
    if (rc == -1) {
        perror("bind");
        return 1;
    }

    unsigned char buf[16];
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    while (1) {
        cnt = recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr *)&client_addr, &client_addr_len);
        if (cnt == -1) {
            perror("recvfrom");
            return 1;
        }

        printf("Received request from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        const char *message = "Hello, world!\r\n";
        size_t message_len = strlen(message);

        cnt = sendto(sock, message, message_len, 0, (struct sockaddr *)&client_addr, client_addr_len);
        if (cnt != (ssize_t)message_len) {
            perror("sendto");
            return 1;
        }

        printf("Sent business card to %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    }

    return 0;
}
