#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
    FILE *input;
    int i,j,compras;
    char buffer[100];
    input = fopen("compras.txt","r");

    fgets(buffer,100,input);
    strtok(buffer, "\n");
    int cantCompras = atoi(buffer);
    printf("%s\n",buffer);

    for(i=0;i<cantCompras;i++){
        fgets(buffer,100,input);
        printf("%s",buffer);
        strtok(buffer, "\n");
        int compras = atoi(buffer);
        for(j=0;j<compras;j++){
            fgets(buffer,100,input);
            printf("%s",buffer);
            strtok(buffer,"\n");
        }
    }
    fclose(input);
}
