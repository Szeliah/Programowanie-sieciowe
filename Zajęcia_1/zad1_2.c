#include <stdio.h>

#define MAX_SIZE 50

//1.2
void drukuj_alt(int* tablica, int liczba_elementow){
    int* end = tablica + liczba_elementow;
    while(tablica != end){
        if(*tablica > 10 && *tablica < 100){
            printf("%d\n", *tablica);
        }
        ++tablica;
    }
}


int main(int argc, char** argv){
    int liczby[MAX_SIZE];
    int temp;
    int counter = 0;

    for(int i = 0; i < MAX_SIZE; ++i){
        scanf("%d", &temp);
        if(temp == 0){
            break;
        }
        liczby[i] = temp;
        ++counter;
    }

    drukuj_alt(liczby, counter);
    return 0;
}