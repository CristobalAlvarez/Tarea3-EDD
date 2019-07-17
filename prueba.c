#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void prueba(int HT[]){
    HT[0]=5;
}

int main(){
    int ar[5];
    prueba(ar);
    printf("%d",ar[0]);
}
