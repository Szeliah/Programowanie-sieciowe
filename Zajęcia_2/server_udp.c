// Szkielet serwera UDP/IPv4 używającego gniazdka bezpołączeniowego.

#define _POSIX_C_SOURCE 200809L
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

int main(int argc, char** argv)
{
    int sock;
    int rc;         // "rc" to skrót słów "result code"
    ssize_t cnt;    // na wyniki zwracane przez recvfrom() i sendto()
    int PORT;       // numer portu

    PORT = atoi(argv[1]); 
    
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        perror("socket");
        return 1;
    }

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_addr = { .s_addr = htonl(INADDR_ANY) },
        .sin_port = htons(PORT)
    };

    rc = bind(sock, (struct sockaddr *) & addr, sizeof(addr));
    if (rc == -1) {
        perror("bind");
        return 1;
    }

    bool keep_on_handling_clients = true;

    while (keep_on_handling_clients) {

        char write_buffer[] = "Hello, world!\r\n";

        unsigned char buf[16];
        struct sockaddr_in clnt_addr;
        socklen_t clnt_addr_len;

        clnt_addr_len = sizeof(clnt_addr);

        cnt = recvfrom(sock, buf, 16, 0,
                (struct sockaddr *) & clnt_addr, & clnt_addr_len);
        if (cnt == -1) {
            perror("recvfrom");
            return 1;
        }

        printf("received %zi bytes\n", cnt);

        // memcpy(buf, "pong", 4);

        // rc = connect(sock, (struct sockaddr *) &clnt_addr, clnt_addr_len);
        // if (rc == -1) {
        //     perror("connect");
        //     exit(EXIT_FAILURE);
        // }

        // cnt = send(sock, write_buffer, sizeof(write_buffer), 0);
        // if (cnt == -1){
        //     perror("send");
        //     exit(EXIT_FAILURE);
        // }

        
        cnt = sendto(sock, write_buffer, 16, 0,
                (struct sockaddr *) & clnt_addr, clnt_addr_len);
        if (cnt == -1) {
            perror("sendto");
            return 1;
        }

        printf("sent %zi bytes\n", cnt);


    }

    rc = close(sock);
    if (rc == -1) {
        perror("close");
        return 1;
    }

    return 0;
}