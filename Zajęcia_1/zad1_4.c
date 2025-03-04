#include <stdio.h>
#include <stdbool.h>


//1.4
bool is_printable_str(const char * str){
    while(*str != '\0'){
        if (*str < 32 || *str > 126){
            return false;
        }
        ++str;
    }
    return true;
}


int main(int argc, char** argv){
    const char* str_1 = "Test pierwszy, tutaj powinno wypisac prawde";
    const char* str_2 = "A tutaj nie tutaj powinno wypisac fałsz\n";
    const char* str_3 = "\t";

    printf("%d\n", is_printable_str(str_1));
    printf("%d\n", is_printable_str(str_2));
    printf("%d\n", is_printable_str(str_3));

    return 0;
}