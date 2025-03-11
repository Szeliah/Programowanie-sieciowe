// Szkielet klienta UDP/IPv4 używającego gniazdka bezpołączeniowego.

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdbool.h>

int convert_string_IPv4_to_int(const char* addr);
bool is_printable_buf(const void * buf, int len);

int main(int argc, char** argv)
{
    int sock;
    int rc;         // "rc" to skrót słów "result code"
    ssize_t cnt;    // na wyniki zwracane przez recvfrom() i sendto()
    int PORT;       // numer portu

    PORT = atoi(argv[2]);

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        perror("socket");
        return 1;
    }

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        // .sin_addr = { .s_addr = htonl(0x7F000001) },   
        .sin_port = htons(PORT)
    };


    if (inet_pton(AF_INET, argv[1], &(addr.sin_addr)) <= 0) {
        perror("inet_pton failed");
        exit(EXIT_FAILURE);
    }


    unsigned char buf[16];
    buf[15] = '\0';
    // memcpy(buf, "ping", 4);


    cnt = connect(sock, (struct sockaddr *) & addr, sizeof(addr));
    if (rc == -1) {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    cnt = send(sock, buf, 0, 0);
    if (cnt == -1){
        perror("send");
        exit(EXIT_FAILURE);
    }

    // cnt = sendto(sock, buf, 0, 0, (struct sockaddr *) & addr, sizeof(addr));
    // if (cnt == -1) {
    //     perror("sendto");
    //     return 1;
    // }

    printf("sent %zi bytes\n", cnt);

    // cnt = recvfrom(sock, buf, 15, 0, NULL, NULL);
    // if (cnt == -1) {
    //     perror("recvfrom");
    //     return 1;
    // }

    cnt = recv(sock, buf, 15, 0);
    if (cnt == -1) {
        perror("recv");
        exit(EXIT_FAILURE);
    }

    printf("received %zi bytes\n", cnt);
    
    if (is_printable_buf(buf, 15)) printf("%s", buf);

    rc = close(sock);
    if (rc == -1) {
        perror("close");
        return 1;
    }

    return 0;
}



bool is_printable_buf(const void* buf, int len){
    const char* ptr = (const char*)buf;
    const char* end = ptr + len;
    while(ptr != end){
        if((*ptr < 32 || *ptr > 126) && (*ptr != '\n' && *ptr != '\r' && *ptr != '\t')){
            return false;
        }
        ++ptr;
    }
    return true;
}



// int convert_string_IPv4_to_int(const char* addr){
    
//     const char* ptr = addr;
//     int first_octet, second_octet, third_octet, fourth_octet;   
//     int dot_counter = 0;
//     int i = 0;
//     char buf[4];

//     while (*ptr != '\0'){
//         while (*ptr != '.' && *ptr != '\0'){
//             buf[i] = *ptr;
//             ++i;
//             ++ptr;
//         }
    
//         buf[i] = '\0'; 
        
//         switch (dot_counter)
//         {
//         case 0:
//             first_octet = atoi(buf);
//             break;
        
//         case 1:
//             second_octet = atoi(buf);
//             break;

//         case 2:
//             third_octet = atoi(buf);
//             break;

//         case 3:
//             fourth_octet = atoi(buf);
//             break;
//         }

//         if (*ptr == '\0'){
//             break;
//         }

//         ++dot_counter;
//         i = 0;
//         ++ptr;
//     }

//     return (int)(first_octet << 24 | second_octet << 16 | third_octet << 8 | fourth_octet);
// }