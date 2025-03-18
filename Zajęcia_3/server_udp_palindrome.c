// Szkielet serwera UDP/IPv4 używającego gniazdka bezpołączeniowego.

// Pierwsza wersja serwera, ktory oblicza ilosc palindromow i zwraca wynik jako sotsunek ilosci palindromow do ilosci wyrazow

#define _POSIX_C_SOURCE 200809L
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdbool.h>
#include <ctype.h>
#include <sys/time.h>

#define MAX_SIZE 65507

enum State{
    Q0,
    Q1,
    Q2,
    Q3,
    Q4
};

enum Alphabet{
    Start,
    C,
    S,
    R,
    N
};

struct Automat{
    enum State state;
    enum Alphabet current_sign;
    enum Alphabet previous_sign;    
};


struct Data_type{
    int words_count;
    int palindorme_count;
};   


bool is_palindrome(const char* string, int size);
struct Data_type palindrome_count(const char* string, int size); 

bool is_valid_string(const char* buffer, int length);
bool recognize(struct Automat* au, const char* ptr);

bool is_space(const char* sign);
bool is_character(const char* sign);
bool is_LF(const char* sign);
bool is_CR(const char* sign);


int main(int argc, char** argv){

    int sock;                                                       // gniazdko
    int rc;                                                         // "rc" to skrót słów "result code"
    ssize_t cnt;                                                    // na wyniki zwracane przez recvfrom() i sendto()
    const int PORT = 2020;                                          // numer portu
    const char ERROR_MESSAGE[5] = {'E', 'R', 'R', 'O', 'R'};        // wiadomosc wysylana w razie niepowodzenia

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        perror("socket error");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_addr = { .s_addr = htonl(INADDR_ANY) },
        .sin_port = htons(PORT)
    };

    struct timeval timeout = {
        .tv_sec = 10
    };

    rc = bind(sock, (struct sockaddr *) & addr, sizeof(addr));
    if (rc == -1) {
        perror("bind error");
        exit(EXIT_FAILURE);
    }
 
    bool keep_on_handling_clients = true;

    while (keep_on_handling_clients) {

        char read_buffer[MAX_SIZE];
        char write_buffer[3];
        
        struct sockaddr_in clnt_addr;
        socklen_t clnt_addr_len;

        clnt_addr_len = sizeof(clnt_addr);

        setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

        cnt = recvfrom(sock, read_buffer, MAX_SIZE, 0,
                (struct sockaddr *) & clnt_addr, & clnt_addr_len);
        printf("%ld\n", cnt);
        if (cnt == -1) {
            perror("recvfrom error");
            exit(EXIT_FAILURE);
        }

  
        if (is_valid_string(read_buffer, cnt) || cnt == 0){
            struct Data_type data = palindrome_count(read_buffer, cnt);
            char temp_buf[4];

            snprintf(temp_buf, sizeof(temp_buf), "%d/%d", data.palindorme_count, data.words_count);

            cnt = sendto(sock, temp_buf, 3, 0,
                (struct sockaddr *) & clnt_addr, clnt_addr_len);
                if (cnt == -1) {
                    perror("sendto error");
                    exit(EXIT_FAILURE);
                }

            continue;
        }

        cnt = sendto(sock, ERROR_MESSAGE, 5, 0,
            (struct sockaddr *) & clnt_addr, clnt_addr_len);
           if (cnt == -1) {
            perror("sendto error");
            exit(EXIT_FAILURE);
        }

    }

    rc = close(sock);
    if (rc == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }

    return 0;
}


bool is_palindrome(const char* string, int size){
    int left = 0; 
    int right = size - 1;
    while (left < right){
        if (toupper(string[left]) != toupper(string[right])){
            return false;
        }
        ++left;
        --right;
    }
    return true;
}

struct Data_type palindrome_count(const char* string, int size) {
    const char* it = string;
    const char* end = string + size;

    struct Data_type data = {
        .palindorme_count = 0,
        .words_count = 0
    };

    int word_size = 0;  
    const char* word_start = string;  

    while (it != end) {

        if (*it != '\n' && *it != '\r'){

            if (is_space(it) || it == end - 1) {
     
                if (it == end - 1) {
                    ++word_size; 
                }
    
                if (word_size > 0) {
                
                    if (is_palindrome(word_start, word_size)) {
                        data.palindorme_count++;
                    }
                    data.words_count++;  
                    word_size = 0;  
                }
            } 
            else {
              
                if (word_size == 0) {
                    word_start = it;
                }
                ++word_size;
    
            }
        }

        it++;

    }

    if (word_size > 0) {
        if (is_palindrome(word_start, word_size)){
            data.palindorme_count++;
        }
        data.words_count++;
    }

    return data;
}


bool is_valid_string(const char* buffer, int length){
    
    struct Automat au = {
        .state = Q0,
        .current_sign = Start,
        .previous_sign = Start,
    };  
    
    const char* it = buffer;
    const char* end = buffer + length;

    while(it != end){
        if (!recognize(&au, it)){
            return false;
        }
        ++it;
    }

    if (au.state == Q1 || au.state == Q3){
        return true;
    }

    return false;
}


bool is_character(const char* sign){
    return ((*sign > 64 && *sign < 91) || (*sign > 96 && *sign < 123)) ? true : false;
}

bool is_space(const char* sign){
    return (*sign == 32) ? true : false;
}


bool is_LF(const char* sign){
    return (*sign == 10) ? true : false;
}


bool is_CR(const char* sign){
    return (*sign == 13) ? true : false;
}


bool recognize(struct Automat* au, const char* ptr){
    
    if (is_character(ptr)){
        au->previous_sign = au->current_sign;
        au->current_sign = C;
        au->state = Q1;
        if (au->previous_sign == R || au->previous_sign == N){
            return false;
        }
        return true;
    }

    if (is_space(ptr)){
        au->previous_sign = au->current_sign;
        au->current_sign = S;
        au->state = Q2;
        if (au->previous_sign != C){
            return false;
        }
        return true;
    }
    
    if (is_LF(ptr)){
        au->previous_sign = au->current_sign;
        au->current_sign = N;
        au->state = Q3;
        if ( au->previous_sign != R && au->previous_sign != C ){
            return false;
        }
        return true;
    }

    if (is_CR(ptr)){
        au->previous_sign = au->current_sign;
        au->current_sign = R;
        au->state = Q4;
        if (au->previous_sign != C){
            return false;
        }
        return true;
    }

    return false;
}




// bool recognize(struct Automat* au, const char* ptr){
    
//     if (is_character(ptr)){
//         if (au->current_sign == N || au->current_sign == R){
//             au->state = Q1;
//             return false;
//         }
//         au->previous_sign = au->current_sign;
//         au->current_sign = C;
//         au->state = Q1;
//         return true;
//     }

//     if (is_space(ptr)){
//         if (au->current_sign == N || au->current_sign == R){
//             au->state = Q2;
//             return false;
//         }
//         au->previous_sign = au->current_sign;
//         au->current_sign = S;
//         au->state = Q2;
//         if (au->previous_sign == S){
//             au->state = Q2;
//             return false;
//         }
//         return true;
//     }
    
//     if (is_LF(ptr)){
//         if (au->current_sign == S || au->current_sign == N){
//             au->state = Q3;
//             return false;
//         }
//         au->previous_sign = au->current_sign;
//         au->current_sign = N;
//         au->state = Q3;
//         return true;
//     }

//     if (is_CR(ptr)){
//         if (au->current_sign == S || au->current_sign == N || au->current_sign == R){
//             au->state = Q4;
//             return false;
//         }
//         au->previous_sign = au->current_sign;
//         au->current_sign = R;
//         au->state = Q4;
//         return true;
//     }

//     return false;
// }

