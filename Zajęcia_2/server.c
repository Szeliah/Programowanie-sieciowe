// Szkielet serwera TCP/IPv4.
//
// Po podmienieniu SOCK_STREAM na SOCK_DGRAM staje się on szkieletem serwera
// UDP/IPv4 korzystającego z gniazdek działających w trybie połączeniowym.

#define _POSIX_C_SOURCE 200809L
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

int main(int argc, char** argv) {

    if (argc != 2){
        perror("Niewlasciwa ilosc argumentow");
        exit(EXIT_FAILURE);
    }

    int lst_sock;   // gniazdko nasłuchujące
    int clnt_sock;  // gniazdko połączone z bieżącym klientem
    int rc;         // "rc" to skrót słów "result code"
    ssize_t cnt;    // wyniki zwracane przez read() i write() są tego typu
    int PORT;       // numer portu

    PORT = atoi(argv[1]); 

    lst_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (lst_sock == -1) {
        perror("socket");
        return 1;
    }

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_addr = { .s_addr = htonl(INADDR_ANY) },
        .sin_port = htons(PORT)
    };

    rc = bind(lst_sock, (struct sockaddr *) & addr, sizeof(addr));
    if (rc == -1) {
        perror("bind");
        return 1;
    }

    rc = listen(lst_sock, 10);
    if (rc == -1) {
        perror("listen");
        return 1;
    }

    bool keep_on_handling_clients = true;
    
    while (keep_on_handling_clients) {

        clnt_sock = accept(lst_sock, NULL, NULL);
        if (clnt_sock == -1) {
            perror("accept");
            return 1;
        }

        char write_buffer[] = "Hello, world!\r\n";

        //unsigned char buf[16];
        // cnt = read(clnt_sock, buf, 16);
        // if (cnt == -1) {
        //     perror("read");
        //     return 1;
        // }
        // printf("read %zi bytes\n", cnt);
        // memcpy(buf, "pong", 4);

        cnt = write(clnt_sock, write_buffer, sizeof(write_buffer));
        if (cnt == -1) {
            perror("write");
            return 1;
        }

        printf("wrote %zi bytes\n", cnt);

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