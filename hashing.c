#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

/*--------------------------------- */
//Funciones y struct de listas enlazadas, utilizadas como herramienta en el programa.
struct nodo {
   int cantidad;
   int numero;
   struct nodo *sig;
};

//Lista utilizada en programa
struct nodo *cabeza = NULL;

struct nodo* find(int numero);

//Inserta en lista. En caso de que el numero ya se encuentre, le suma 1 a int cantidad.
void insert(int numero){
    struct nodo *aux = find(numero);
	if(aux==NULL){	
		struct nodo *aux = (struct nodo*) malloc(sizeof(struct nodo));
   	    aux->numero = numero;
   	    aux->cantidad = 1;
   	    aux->sig = cabeza;
   	    cabeza = aux;
	}else{
		aux->cantidad++;
	}
}

//Retorna nodo con int numero, si no esta retorna NULL
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

//Reinicia lista
void rebootList(){
   while(cabeza!=NULL){
      struct nodo *aux = cabeza;
      cabeza=cabeza->sig;
      free(aux);
   }
}

/*--------------------------------- */

#define VACIA -1
int CANT_OFERTA; //Largo tabla hashing ofertas
int CANT_PRODUCTO; //Largo tabla hashing productos

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

//Segunda funcion de hashing
int h2(int k){
	return 7-k%7;
}

int p(int k, int i,int pos){
    return i*h2(k);
}

//Primera funcion de hashing
int h(int k, int i,int pos){
    return k;
}

int hashInsertOferta(oferta HT[], int k, int cant_desc, int desc);
int hashInsertProducto(producto HT[], int k, char nombre[31], int pre);

/*****************
 * producto *hashInitProducto
 * *****
 * Funcion lee el archivo productos.dat, crea la tabla hashing y inserta los elementos en ella.
 * *****
 * input:
 *     No hay
 * ****
 * returns:
 *     producto *: Tabla hashing con los productos insertados.
 * ***/
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

/*****************
 * producto *hashInitOferta
 * *****
 * Funcion lee el archivo ofertas.dat, crea la tabla hashing y inserta los elementos en ella.
 * *****
 * input:
 *     No hay
 * ****
 * returns:
 *     producto *: Tabla hashing con las ofertas insertadas.
 * ***/
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

/*****************
 * int hashInsertOferta
 * *****
 * Dado la clave, la informacion y la tabla hashing de ofertas, esta funcion inserta 
 * en la tabla utilizando las funciones de hashing.
 * *****
 * input:
 *     oferta HT[]: Tabla hashing
 *     int k: clave de oferta a insertar
 *     int cant_desc: cantidad de productos para el descuento
 *     int desc: descuento
 * ****
 * returns:
 *     int 1 en caso de ser insertado, 0 en caso contrario
 * ***/
int hashInsertOferta(oferta HT[], int k, int cant_desc, int desc) {
    int i;
    int pos = h(k,0,1)%CANT_OFERTA;
    int inicio = pos;

    for(i=1;HT[pos].cod_producto != VACIA && HT[pos].cod_producto != k;i++){
        pos = (inicio + p(k, i,1)) % CANT_OFERTA;
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

/*****************
 * int hashInsertProducto
 * *****
 * Dado la clave, la informacion y la tabla hashing de productos, esta funcion inserta 
 * en la tabla utilizando las funciones de hashing.
 * *****
 * input:
 *     producto HT[]: Tabla hashing
 *     int k: clave de producto a insertar
 *     char nombre[31]: nombre del producto
 *     int pre: precio del producto
 * ****
 * returns:
 *     int 1 en caso de ser insertado, 0 en caso contrario
 * ***/
int hashInsertProducto(producto HT[], int k, char nombre[31], int pre) {
    int i;
    int pos = h(k,0,0)%CANT_PRODUCTO;
    int inicio = pos;

    for (i = 1; HT[pos].cod_producto != VACIA && HT[pos].cod_producto != k; i++){
        pos = (inicio + p(k,i,0)) % CANT_PRODUCTO;
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

/*****************
 * producto searchProducto
 * *****
 * Recorre la tabla de hashing segun la funcion hashing y los elementos en la tabla, buscando el elemento con clave k.
 * Si lo encuentra, retorna el struct producto del elemento, sino, retorna un struct producto vacio
 * *****
 * input:
 *     oferta HT[]: Tabla hashing
 *     int k: clave de producto buscado
 *  ****
 * returns:
 *     producto con clave k, en caso de no ser encontrado retorna un producto vacio
 * ***/
producto searchProducto(producto HP[],int k){
    int inicio, i;
    int pos = h(k,0,0)%CANT_PRODUCTO;
    inicio=pos;

    producto output;
    strcpy(output.nbre_producto,"NADA");
    output.cod_producto=VACIA;
    output.precio = 0;

    for (i = 1; HP[pos].cod_producto != VACIA && HP[pos].cod_producto != k; i++){
        pos = (inicio + p(k, i,0)) % CANT_PRODUCTO;
    }
    
    if (HP[pos].cod_producto == k){
        output = HP[pos];
    }

    return output;
}

/*****************
 * producto searchOferta
 * *****
 * Recorre la tabla de hashing segun la funcion hashing y los elementos en la tabla, buscando el elemento con clave k.
 * Si lo encuentra, retorna el struct oferta del elemento, sino, retorna un struct oferta vacio
 * *****
 * input:
 *     oferta HT[]: Tabla hashing
 *     int k: clave de oferta buscado
 *  ****
 * returns:
 *     oferta con clave k, en caso de no ser encontrado retorna una oferta vacia.
 * ***/
oferta searchOferta(oferta HP[],int k){
    int inicio, i;
    int pos = h(k,0,1)%CANT_OFERTA;
    inicio=pos;

    oferta output;
    output.cod_producto=VACIA;
    output.cantidad_descuento = 0;
    output.descuento = 0;

    for (i = 1; HP[pos].cod_producto != VACIA && HP[pos].cod_producto != k; i++){
        pos = (inicio + p(k,i,1)) % CANT_OFERTA;
    }
    
    if(HP[pos].cod_producto == k){
        output = HP[pos];
    }

    return output;
}

/*****************
 * void hashDisplayOferta
 * *****
 * Recorre la tabla de hashing y printea la informacion de cada elemento.
 * *****
 * input:
 *     oferta HT[]: Tabla hashing
 *  ****
 * returns:
 *     No hay
 * ***/
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

/*****************
 * void hashDisplayProducto
 * *****
 * Recorre la tabla de hashing y printea la informacion de cada elemento.
 * *****
 * input:
 *     oferta HT[]: Tabla hashing
 *  ****
 * returns:
 *     No hay
 * ***/
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

    //Inicializa cada tabla de hashing
    oferta *inputOfertas = hashInitOferta(); 
    producto *inputProductos = hashInitProducto();
    //hashDisplayOferta(inputOfertas);
    hashDisplayProducto(inputProductos);

    //Abre archivos
    FILE *input,*output;
    input = fopen("compras.txt","r");
    output = fopen("boletas.txt","w");

    //Lee el primer numero del archivo compras
    fgets(buffer,100,input);
    strtok(buffer, "\n");
    int cantCompras = atoi(buffer);
    sprintf(buffer_aux,"%d\n",cantCompras);
    fputs(buffer_aux,output);

    //For en donde cada iteracion son las compras de un cliente distinto
    for(i=0;i<cantCompras;i++){

        int total = 0;
        fgets(buffer,1000,input);
        strtok(buffer, "\n");
        int compras = atoi(buffer);

        //Lee todas las compras y las inserta en una lista. Cada nodo de la lista guarda un ID de producto y la cantidad de veces que se compro
        for(j=0;j<compras;j++){
            fgets(buffer,1000,input);
            strtok(buffer,"\n");
            int id = atoi(buffer);
            insert(id);          
        }

        //Recorre la lista, calculando el total dependiendo de la cantidad de productos y las ofertas.
        while(cabeza!=NULL){
            producto actualProducto = searchProducto(inputProductos,cabeza->numero);
            //printf("numero %d, cantidad %d\n",cabeza->numero,cabeza->cantidad);
            if(actualProducto.cod_producto!=VACIA){
                oferta actualOferta = searchOferta(inputOfertas,cabeza->numero);
                if(actualOferta.cod_producto==VACIA){
                    total = total + cabeza->cantidad*actualProducto.precio;
                }else{
                    total = total + (cabeza->cantidad)*(actualProducto.precio-actualOferta.descuento) - (cabeza->cantidad/actualOferta.cantidad_descuento)*actualOferta.descuento;
                }                
            }
            cabeza=cabeza->sig;
        }

        //Escribe en archivo y reinicia la lista
        sprintf(buffer_aux,"%d\n",total);
        fputs(buffer_aux,output);
        rebootList();
    }
    
    //Libera tablas de hashing y cierra archivos
    free(inputOfertas);
    free(inputProductos);
    fclose(input);
    fclose(output);
    return 0;
}
