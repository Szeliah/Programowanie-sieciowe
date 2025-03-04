#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX 512

//1.7
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
        perror("Nieudana proba otwarcia pliku src_path");
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

    int line_counter = 1;

    while ((bytes_read = read(src_fd, buffer, MAX)) > 0){

        int i = 0;
        while (i < bytes_read){
            
            int start_read = i;
            
            while (i < bytes_read && buffer[i] != '\n') {
                i++;
            }

            if (line_counter % 2 == 1){
                int line_length = i - start_read + 1;

                bytes_written = write(dest_fd, buffer + start_read, line_length);
                
                if (bytes_written < 0) {
                    perror("Nieudane zapisywanie do pliku");
                    close(src_fd);
                    close(dest_fd);
                    exit(EXIT_FAILURE);
                }
            }

            if (i < bytes_read && buffer[i] == '\n') {
                ++line_counter;
            }

            ++i;
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
