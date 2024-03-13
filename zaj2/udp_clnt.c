#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Sposób użycia: %s <adres_IPv4> <port>\n", argv[0]);
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

    struct sockaddr_in server_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(atoi(argv[2]))
    };

    if (inet_pton(AF_INET, argv[1], &server_addr.sin_addr) <= 0) {
        perror("inet_pton");
        return 1;
    }

    const char *message = "";
    size_t message_len = strlen(message);

    cnt = sendto(sock, message, message_len, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (cnt != (ssize_t)message_len) {
        perror("sendto");
        return 1;
    }

    printf("Sent request to %s:%s\n", argv[1], argv[2]);

    unsigned char buf[16];
    cnt = recvfrom(sock, buf, sizeof(buf), 0, NULL, NULL);
    if (cnt == -1) {
        perror("recvfrom");
        return 1;
    }

    printf("Received business card: %.*s\n", (int)cnt, buf);

    return 0;
}
