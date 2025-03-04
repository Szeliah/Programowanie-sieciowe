#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX 512

//1.6
int main(int argc, char** argv){

    if (argc != 3){
        perror("Nieprawidlowa liczba argumentow");
        exit(EXIT_FAILURE);
    }

    const char* src_path = argv[1];
    const char* dest_path = argv[2];

    unsigned int src_fd;
    unsigned int dest_fd;

    ssize_t bytes_read;
    ssize_t bytes_written;

    char buffer[MAX] = {0};

    src_fd = open(src_path, O_RDONLY);

    if (src_fd < 0){
        perror("Nieudana proba otwarcia pliku");
        exit(EXIT_FAILURE);
    }

    dest_fd = open(dest_path, O_WRONLY | O_APPEND, 0666);

    if (dest_fd < 0){
        perror("Nieudana proba otwarcia pliku");
        if (close(src_fd) < 0){
            perror("Nieudana proba zamkniecia pliku");
            exit(EXIT_FAILURE);
        }
        exit(EXIT_FAILURE);
    }

    while ((bytes_read = read(src_fd, buffer, MAX)) > 0){
        bytes_written = write(dest_fd, buffer, bytes_read);
        if (bytes_written < 0) {
            perror("Nieudane zapisywanie do pliku");
            close(src_fd);
            close(dest_fd);
            exit(EXIT_FAILURE);
        }
    }

    if (bytes_read < 0){
        perror("Nieudane czytanie z pliku");
        close(src_fd);
        close(dest_fd);
        exit(EXIT_FAILURE);
    }

    if (close(src_fd) < 0){
        perror("Nieudana proba zamkniecia pliku");
        if (close(dest_fd) < 0){
            perror("Nieudana proba zamkniecia pliku");
            exit(EXIT_FAILURE);
        }
        exit(EXIT_FAILURE);
    }

    if (close(dest_fd) < 0){
        perror("Nieudana proba zamkniecia pliku");
        exit(EXIT_FAILURE);
    }
    
    return 0;
}
