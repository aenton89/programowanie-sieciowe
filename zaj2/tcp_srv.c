#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Sposób użycia: %s <port>\n", argv[0]);
        return 1;
    }

    int lst_sock;   // gniazdko nasłuchujące
    int clnt_sock;  // gniazdko połączone z bieżącym klientem
    int rc;         // "rc" to skrót słów "result code"
    ssize_t cnt;    // wyniki zwracane przez read() i write() są tego typu

    lst_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (lst_sock == -1) {
        perror("socket");
        return 1;
    }

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_addr = { .s_addr = htonl(INADDR_ANY) },
        .sin_port = htons(atoi(argv[1]))
    };

    rc = bind(lst_sock, (struct sockaddr *) &addr, sizeof(addr));
    if (rc == -1) {
        perror("bind");
        return 1;
    }

    rc = listen(lst_sock, 10);
    if (rc == -1) {
        perror("listen");
        return 1;
    }

    while (1) { // Pętla działa w nieskończoność
        clnt_sock = accept(lst_sock, NULL, NULL);
        if (clnt_sock == -1) {
            perror("accept");
            return 1;
        }

        const char *message = "Hello, world!\r\n";
        size_t message_len = strlen(message);

        cnt = write(clnt_sock, message, message_len);
        if (cnt != (ssize_t)message_len) {
            perror("write");
            return 1;
        }

        rc = close(clnt_sock);
        if (rc == -1) {
            perror("close");
            return 1;
        }
    }

    rc = close(lst_sock);
    if (rc == -1) {
        perror("close");
        return 1;
    }

    return 0;
}