#include <stdio.h>
#include <stdbool.h>


//1.3
bool is_printable_buf(const void * buf, int len){
    const unsigned char* ptr = (const unsigned char*)buf;
    const unsigned char* end = ptr + len;
    while(ptr != end){
        if(*ptr < 32 || *ptr > 126){
            return false;
        }
        ++ptr;
    }
    return true;
}


int main(int argc, char** argv){
    char buffer_1[] = "Test pierwszy, tutaj powinno wypisac prawde";
    char buffer_2[] = "A tutaj nie tutaj powinno wypisac fałsz\n";
    char buffer_3[] = "\t";

    printf("%d\n", is_printable_buf(buffer_1, sizeof(buffer_1) - 1));
    printf("%d\n", is_printable_buf(buffer_2, sizeof(buffer_2) - 1));\
    printf("%d\n", is_printable_buf(buffer_3, sizeof(buffer_3) - 1));

    return 0;
}