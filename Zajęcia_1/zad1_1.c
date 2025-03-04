#include <stdio.h>

#define MAX_SIZE 50

// 1.1
void drukuj(int tablica[], int liczba_elementow){
    for(int i = 0; i < liczba_elementow; ++i){
        int liczba = tablica[i];
        if(liczba > 10 && liczba < 100){
            printf("%d\n", liczba);
        }
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

    drukuj(liczby, counter);

    return 0;
}