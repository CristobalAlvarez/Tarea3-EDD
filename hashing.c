#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define VACIA -1
int CANT_OFERTA;
int CANT_PRODUCTO;

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

int hashInsertOferta(oferta HT[], int k, int cant_desc, int desc);
int hashInsertProducto(producto HT[], int k, char nombre[31], int pre);

producto *hashInitProducto(){
    int i,M;
    producto inProducto;

    FILE *product;
    product = fopen("productos.dat", "r");

    fread(&M, sizeof(int),1,product);
    
    int  largeProduct = (int) ceil( ((double)10/(double)7)*M);
    producto *HP = malloc(sizeof(producto)*largeProduct);

    for (i = 0; i < largeProduct; i++){
        HP[i].cod_producto=VACIA;
    }

    CANT_PRODUCTO = largeProduct;

    for(i=0;i<largeProduct;i++){
        fread(&inProducto, sizeof(producto), 1, product);
        hashInsertProducto(HP, inProducto.cod_producto, inProducto.nbre_producto, inProducto.precio);
    }

    fclose(product);
    return HP;
}

oferta *hashInitOferta(){
    int i,M;
    oferta inOferta;

    FILE *ofert;
    ofert = fopen("ofertas.dat", "r");

    fread(&M, sizeof(int),1,ofert);
    
    int  largeOfert = (int) ceil( ((double)10/(double)7)*M);
    CANT_OFERTA = largeOfert;
    oferta *HO = malloc(sizeof(oferta)*largeOfert);

    for (i = 0; i < largeOfert; i++){
        HO[i].cod_producto=VACIA;
    }

    for(i=0;i<largeOfert;i++){
        fread(&inOferta, sizeof(oferta), 1, ofert);
        hashInsertOferta(HO,inOferta.cod_producto, inOferta.cantidad_descuento, inOferta.descuento);
    }

    fclose(ofert);
    return HO;
}

int hashInsertOferta(oferta HT[], int k, int cant_desc, int desc) {
    int i;
    int pos = h(k,0);
    int inicio = pos;

    for (i = 1; HT[pos].cod_producto != VACIA && HT[pos].cod_producto != k; i++){
        pos = (inicio + h(k, i)) % CANT_OFERTA;
    }

    if (HT[pos].cod_producto == k){
        return 0;
    }else{
        HT[pos].cod_producto = k;
        HT[pos].cantidad_descuento = cant_desc;
        HT[pos].descuento = desc;
        return 1;
    }
}

int hashInsertProducto(producto HT[], int k, char nombre[31], int pre) {
    int i;
    int pos = h(k,0);
    int inicio = pos;
    for (i = 1; HT[pos].cod_producto != VACIA && HT[pos].cod_producto != k; i++)
        pos = (inicio + h(k, i)) % CANT_PRODUCTO;

    if (HT[pos].cod_producto == k){
        return 0;
    }else{
        HT[pos].cod_producto = k;
        HT[pos].precio = pre;
        strcpy(HT[pos].nbre_producto,nombre);
        return 1;
    }
}

producto searchProducto(producto HP[],int k){
    int inicio, i;
    int pos = h(k,0);
    inicio=pos;

    producto output;
    output.cod_producto = 0;
    output.precio = 0;
    strcpy(output.nbre_producto,"nada");

    for (i = 1; HP[pos].cod_producto != VACIA && HP[pos].cod_producto != k; i++){
        pos = (inicio + h(k, i)) % CANT_PRODUCTO;
    }
    
    if (HP[pos].cod_producto == k){
        output = HP[pos];
    }

    return output;
}

oferta searchOferta(oferta HP[],int k){
    int inicio, i;
    int pos = h(k,0);
    inicio=pos;

    oferta output;
    output.cod_producto = 0;
    output.cantidad_descuento = 0;
    output.descuento = 0;

    for(i = 1; HP[pos].cod_producto != VACIA && HP[pos].cod_producto != k; i++){
        pos = (inicio + h(k, i)) % CANT_OFERTA; // próxima ranura en la secuencia
    }

    if(HP[pos].cod_producto == k){
            output = HP[pos];
            return output;
    }

    return output;
}


void hashDisplayOferta(oferta HT[]){
   int i = 0;
   for(i = 0; i<CANT_OFERTA; i++) {
      if(HT[i].cod_producto != VACIA)
         printf(" (Codigo: %d, Descuento: %d. Cantidad: %d)",HT[i].cod_producto,HT[i].descuento,HT[i].cantidad_descuento);
      else
         printf(" ~~ ");
   }
   printf("\n");
}

void hashDisplayProducto(producto HT[]){
   int i = 0;
   for(i = 0; i<CANT_PRODUCTO; i++) {
      if(HT[i].cod_producto != VACIA)
         printf(" (%d,%s)",HT[i].cod_producto,HT[i].nbre_producto);
      else
         printf(" ~~ ");
   }
   printf("\n");
}

int doDescuento(oferta HT[], int k){
    int i;
    int pos = h(k,0);
    int re=0;
    int inicio=pos;
    for(i = 1; HT[pos].cod_producto != VACIA && HT[pos].cod_producto != k; i++){
        pos = (inicio + h(k, i)) % CANT_OFERTA;
    }
        
    if(HT[pos].cod_producto == k && HT[pos].cantidad_descuento > 0){
        (&HT)[pos]->cantidad_descuento--;
        re = HT[pos].descuento;
    }
    return re;

}

int main(){
    int i,j;
    char buffer[100];

    oferta *inputOfertas = hashInitOferta();
    producto *inputProductos = hashInitProducto();

    FILE *input;
    input = fopen("compras.txt","r");

    fgets(buffer,100,input);
    strtok(buffer, "\n");
    int cantCompras = atoi(buffer);
    printf("%s\n",buffer);

    for(i=0;i<cantCompras;i++){
        int total = 0;
        fgets(buffer,100,input);
        strtok(buffer, "\n");
        int compras = atoi(buffer);
        for(j=0;j<compras;j++){
            fgets(buffer,100,input);
            int id = atoi(buffer);
            //printf("Producto comprado:%s. Codigo:%s\n",searchProducto(inputProductos,atoi(buffer)).nbre_producto,buffer);
            total = total + searchProducto(inputProductos,id).precio - doDescuento(inputOfertas,id);
        }
        printf("%d\n",total);
    }

    free(inputOfertas);
    free(inputProductos);
    fclose(input);

}