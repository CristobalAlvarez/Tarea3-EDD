#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(){
    char buffer[100];

    FILE *input;
    input = fopen("compras.txt","r");
    while(feof(input)==0){
        fgets(buffer,100,input);
        strtok(buffer, "\n");
        int p = atoi(buffer);
        printf("%d\n",p);
    }
    return 0;
}
