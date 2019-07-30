#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

/*--------------------------------- */

struct nodo {
   int cantidad;
   int numero;
   struct nodo *sig;
};

struct nodo *cabeza = NULL;

struct nodo* find(int numero);

void insert(int numero){
	if(find(numero)==NULL){	
		struct nodo *aux = (struct nodo*) malloc(sizeof(struct nodo));
   	    aux->numero = numero;
   	    aux->cantidad = 1;
   	    aux->sig = cabeza;
   	    cabeza = aux;
	}else{
		find(numero)->cantidad++;
	}
}

struct nodo* find(int numero) {
   struct nodo* actual = cabeza;
   if(cabeza == NULL){
      return NULL;
   }
   while(actual->numero != numero){
      if(actual->sig == NULL){
         return NULL;
      }else{
         actual = actual->sig;
      }
   }      
   return actual;
}

void deleteList(){
   while(cabeza!=NULL){
      struct nodo *aux = cabeza;
      cabeza=cabeza->sig;
      free(aux);
   }
}

/*--------------------------------- */

#define VACIA -1
int CANT_OFERTA;
int CANT_PRODUCTO;

/*----------------------------------*/

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

int h2(int k, int pos){
    return 7-k%7;
}

int h(int k, int i,int pos){
    return k;
}

int hashInsertOferta(oferta HT[], int k, int cant_desc, int desc);
int hashInsertProducto(producto HT[], int k, char nombre[31], int pre);

producto *hashInitProducto(){
    int i,M;
    producto inProducto;

    FILE *product;
    product = fopen("productos.dat", "r");

    fread(&M, sizeof(int),1,product);
    int  largeProduct = (int) floor( ((double)10/(double)7)*M);
    producto *HP = malloc(sizeof(producto)*largeProduct);
    CANT_PRODUCTO = largeProduct;

    for (i = 0; i < largeProduct; i++){
        HP[i].cod_producto=VACIA;
    }


    for(i=0;i<M;i++){
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

    int  largeOfert = (int) floor( ((double)10/(double)7)*M);
    CANT_OFERTA = largeOfert;
    oferta *HO = malloc(sizeof(oferta)*largeOfert);
    for (i = 0; i < largeOfert; i++){
        HO[i].cod_producto=VACIA;
    }

    for(i=0;i<M;i++){
        fread(&inOferta, sizeof(oferta), 1, ofert);
        hashInsertOferta(HO,inOferta.cod_producto, inOferta.cantidad_descuento, inOferta.descuento);
    }

    fclose(ofert);
    return HO;
}

int hashInsertOferta(oferta HT[], int k, int cant_desc, int desc) {
    int i;
    int pos = h(k,0,1)%CANT_OFERTA;
    int inicio = pos;

    for(i=1;HT[pos].cod_producto != VACIA && HT[pos].cod_producto != k;i++){
        pos = (inicio + i*h2(k, 1)) % CANT_OFERTA;
    }

    if(HT[pos].cod_producto==k){
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
    int pos = h(k,0,0)%CANT_PRODUCTO;
    int inicio = pos;

    for (i = 1; HT[pos].cod_producto != VACIA && HT[pos].cod_producto != k; i++){
        pos = (inicio + i*h2(k,0)) % CANT_PRODUCTO;
    }

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
    int pos = h(k,0,0)%CANT_PRODUCTO;
    inicio=pos;

    producto output;
    strcpy(output.nbre_producto,"NADA");
    output.cod_producto=VACIA;
    output.precio = 0;

    for (i = 1; HP[pos].cod_producto != VACIA && HP[pos].cod_producto != k; i++){
        pos = (inicio + i*h2(k, 0)) % CANT_PRODUCTO;
    }
    
    if (HP[pos].cod_producto == k){
        output = HP[pos];
    }

    return output;
}

oferta searchOferta(oferta HP[],int k){
    int inicio, i;
    int pos = h(k,0,1)%CANT_OFERTA;
    inicio=pos;

    oferta output;
    output.cod_producto=VACIA;
    output.cantidad_descuento = 0;
    output.descuento = 0;

    for (i = 1; HP[pos].cod_producto != VACIA && HP[pos].cod_producto != k; i++){
        pos = (inicio + i*h2(k,1)) % CANT_OFERTA;
    }
    
    if(HP[pos].cod_producto == k){
        output = HP[pos];
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
   printf("\n\n");
}

void hashDisplayProducto(producto HT[]){
   int i = 0;
   for(i = 0; i<CANT_PRODUCTO; i++) {
      if(HT[i].cod_producto != VACIA)
         printf(" (%d,%s,%d)",HT[i].cod_producto,HT[i].nbre_producto,HT[i].precio);
      else
         printf(" ~~ ");
   }
   printf("\n");
}

int main(){
    int i,j;
    char buffer[1000],buffer_aux[1000];

    oferta *inputOfertas = hashInitOferta(); 
    producto *inputProductos = hashInitProducto();

    FILE *input,*output;
    input = fopen("compras.txt","r");
    output = fopen("boletas.txt","w");

    fgets(buffer,100,input);
    strtok(buffer, "\n");
    int cantCompras = atoi(buffer);
    sprintf(buffer_aux,"%d\n",cantCompras);
    fputs(buffer_aux,output);

    for(i=0;i<cantCompras;i++){

        int total = 0;
        fgets(buffer,1000,input);
        strtok(buffer, "\n");
        int compras = atoi(buffer);

        for(j=0;j<compras;j++){
            fgets(buffer,1000,input);
            strtok(buffer,"\n");
            int id = atoi(buffer);
            insert(id);          
        }

        while(cabeza!=NULL){
            producto actualProducto = searchProducto(inputProductos,cabeza->numero);
            oferta actualOferta = searchOferta(inputOfertas,cabeza->numero);
            
            if(actualProducto.cod_producto!=VACIA){
                if(actualOferta.cod_producto==VACIA){
                    total = total + cabeza->cantidad*actualProducto.precio;
                }else{
                    total = total + (cabeza->cantidad%(actualOferta.cantidad_descuento))*actualProducto.precio + (cabeza->cantidad - cabeza->cantidad%actualOferta.cantidad_descuento)*(actualProducto.precio-actualOferta.descuento);
                }                
            }
            cabeza=cabeza->sig;
        }
        
        sprintf(buffer_aux,"%d\n",total);
        fputs(buffer_aux,output);
        deleteList();
    }
    
    free(inputOfertas);
    free(inputProductos);
    fclose(input);
    fclose(output);
    return 0;
}