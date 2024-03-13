#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define BUFFER_SIZE 4096

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Sposób użycia: %s <plik_wejsciowy> <plik_wyjsciowy>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int input_fd = open(argv[1], O_RDONLY);
    if (input_fd == -1) {
        perror("Błąd otwarcia pliku wejściowego");
        return EXIT_FAILURE;
    }

    int output_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (output_fd == -1) {
        perror("Błąd otwarcia pliku wyjściowego");
        close(input_fd);
        return EXIT_FAILURE;
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytes_read, bytes_written;

    while ((bytes_read = read(input_fd, buffer, BUFFER_SIZE)) > 0) {
        bytes_written = write(output_fd, buffer, bytes_read);
        if (bytes_written == -1) {
            perror("Błąd zapisu do pliku wyjściowego");
            close(input_fd);
            close(output_fd);
            return EXIT_FAILURE;
        }
    }

    if (bytes_read == -1) {
        perror("Błąd odczytu z pliku wejściowego");
        close(input_fd);
        close(output_fd);
        return EXIT_FAILURE;
    }

    close(input_fd);
    close(output_fd);

    printf("Kopiowanie zakończone sukcesem.\n");
    return EXIT_SUCCESS;
}
