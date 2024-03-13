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
    if (argc != 3) {
        fprintf(stderr, "Sposób użycia: %s <adres_IPv4> <port>\n", argv[0]);
        return 1;
    }

    int sock;
    int rc;         // "rc" to skrót słów "result code"
    ssize_t cnt;    // wyniki zwracane przez read() i write() są tego typu

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("socket");
        return 1;
    }

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(atoi(argv[2]))
    };

    if (inet_pton(AF_INET, argv[1], &addr.sin_addr) <= 0) {
        perror("inet_pton");
        return 1;
    }

    rc = connect(sock, (struct sockaddr *) &addr, sizeof(addr));
    if (rc == -1) {
        perror("connect");
        return 1;
    }

    unsigned char buf[16];
    cnt = read(sock, buf, sizeof(buf));
    if (cnt == -1) {
        perror("read");
        return 1;
    }

    printf("Received business card: ");
    for (ssize_t i = 0; i < cnt; i++) {
        if (isprint(buf[i]) || isspace(buf[i])) {
            printf("%c", buf[i]);
        }
    }
    printf("\n");

    rc = close(sock);
    if (rc == -1) {
        perror("close");
        return 1;
    }

    return 0;
}
