#include <stdio.h>

void drukuj(int tablica[], int liczba_elementow){
    printf("normal: \n");
    for(int i = 0; i < liczba_elementow; i++){
        if(tablica[i] > 10 && tablica[i] < 100)
            printf("%d ", tablica[i]);
    }
    printf("\n");
}

void drukuj_alt(int * tablica, int liczba_elementow){
    // using tablica++ to iterate through the array
    printf("alt: \n");
    for(int i = 0; i < liczba_elementow; i++){
        if(*tablica > 10 && *tablica < 100)
            printf("%d ", *tablica);
        tablica++;
    }
    printf("\n");
}

int main(void){
    int liczby[50];
    int i = 0;
    int read_num;

    printf("podaj liczby: \n");
    scanf("%d", &read_num);
    while(i < 50 && read_num != 0){
        liczby[i] = read_num;
        i++;
        scanf("%d", &read_num);
    }

    drukuj(liczby, i);
    drukuj_alt(liczby, i);
}
