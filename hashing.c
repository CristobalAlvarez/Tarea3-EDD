#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define VACIA -1

typedef struct {
    int cod_producto;
    char nbre_producto[31];
    int precio;
}producto;

typedef struct {
    int cod_producto;
    int cantidad_descuento;
    int descuento;
}oferta;

int h2(int k){
    srand(time(NULL));
    int r = rand();
    return k*r;
}

int h(int k, int i){
    return i;
}

void hashInitProducto(producto HT[],int M){
    int i;
    for (i = 0; i < M; i++){
        HT[i].cod_producto=VACIA;
    }
}

void hashInitOferta(oferta HO[], int M){
    int i;
    for (i = 0; i < M; i++){
        HO[i].cod_producto=VACIA;
    }
}

int hashInsertOferta(oferta HT[], int k, int cant_desc, int desc,int M) {
    int i;
    int pos = h(k,0);
    int inicio = pos;
    for (i = 1; HT[pos].cod_producto != VACIA && HT[pos].cod_producto != k; i++)
        pos = (inicio + h(k, i)) % M; // próxima ranura en la secuencia
        if (HT[pos].cod_producto == k){
            return 0; // inserción no exitosa: cod_producto repetida
        }else{
            HT[pos].cod_producto = k;
            HT[pos].cantidad_descuento = cant_desc;
            HT[pos].descuento = desc;
            return 1; // inserción exitosa
        }
}

int hashInsertProducto(producto HT[], int k, char nombre[31], int pre, int M) {
    int i;
    int pos = h(k,0);
    int inicio = pos;
    for (i = 1; HT[pos].cod_producto != VACIA && HT[pos].cod_producto != k; i++)
        pos = (inicio + h(k, i)) % M; // próxima ranura en la secuencia
        if (HT[pos].cod_producto == k){
            return 0; // inserción no exitosa: cod_producto repetida
        }else{
            HT[pos].cod_producto = k;
            HT[pos].precio = pre;
            strcpy(HT[pos].nbre_producto,nombre);
            return 1; // inserción exitosa
        }
}

producto searchProducto(producto HP[],int k, int M){
    int inicio, i;
    int pos = h(k,0);
    inicio=pos;
    producto output;
    for (i = 1; HP[pos].cod_producto != VACIA && HP[pos].cod_producto != k; i++){
        pos = (inicio + h(k, i)) % M; // próxima ranura en la secuencia
        if (HP[pos].cod_producto == k){
            output = HP[pos]; // registro encontrado, búsqueda exitosa
        }
    }

    return output;
}

oferta searchOferta(oferta HP[],int k, int M){
    int inicio, i;
    int pos = h(k,0);
    inicio=pos;
    oferta output;
    for (i = 1; HP[pos].cod_producto != VACIA && HP[pos].cod_producto != k; i++){
        pos = (inicio + h(k, i)) % M; // próxima ranura en la secuencia
        if (HP[pos].cod_producto == k){
            output = HP[pos]; // registro encontrado, búsqueda exitosa
        }
    }

    return output;
}

void hashDisplayOferta(oferta HT[], int M){
   int i = 0;
   for(i = 0; i<M; i++) {
      if(HT[i].cod_producto != VACIA)
         printf(" (%d,%d)",HT[i].cod_producto,HT[i].descuento);
      else
         printf(" ~~ ");
   }
   printf("\n");
}

void hashDisplayProducto(producto HT[], int M){
   int i = 0;
   for(i = 0; i<M; i++) {
      if(HT[i].cod_producto != VACIA)
         printf(" (%d,%s)",HT[i].cod_producto,HT[i].nbre_producto);
      else
         printf(" ~~ ");
   }
   printf("\n");
}
 

int main(){
    FILE *product,*ofert,*input;
    int productCant,ofertCant,i,j;
    char buffer[100];

    producto inProducto;
    oferta inOferta;

    product = fopen("productos.dat", "r");
    ofert = fopen("ofertas.dat","r");

    fread(&productCant, sizeof(int),1,product);
    fread(&ofertCant, sizeof(int),1,ofert);

    int  largeProduct = (int) ceil( ((double)10/(double)7)*productCant);
    producto HP[largeProduct];

    int  largeOfert = (int) ceil( ((double)10/(double)7)*ofertCant);
    oferta HO[largeOfert];

    hashInitProducto(HP,largeProduct);
    hashInitOferta(HO,largeOfert);

    for(i=0;i<largeProduct;i++){
        fread(&inProducto, sizeof(producto), 1, product);
        hashInsertProducto(HP, inProducto.cod_producto, inProducto.nbre_producto, inProducto.precio,largeProduct);
    }

    for(i=0;i<largeOfert;i++){
        fread(&inOferta, sizeof(oferta), 1, ofert);
        hashInsertOferta(HO,inOferta.cod_producto, inOferta.cantidad_descuento, inOferta.descuento,largeOfert);
    }

    fclose(product);
    fclose(ofert);

    hashDisplayProducto(HP,largeProduct);
    
    input = fopen("compras.txt","r");
    fgets(buffer,100,input);
    strtok(buffer, "\n");
    int cantCompras = atoi(buffer);

    for(i=0;i<cantCompras;i++){
        fgets(buffer,100,input);
        strtok(buffer, "\n");
        int compras = atoi(buffer);
        for(j=0;j<compras;j++){

        }
    }
    
}