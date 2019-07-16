#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// ---------- MODIFICAR ESTO ----------

#define OUTP "productos.dat"  // Nombre del archivo de productos
#define REFP "productos_reference.txt"  // Nombre de la referencia de productos
#define OUTO "ofertas.dat"  // Nombre del archivo de ofertas
#define REFO "ofertas_reference.txt"  // Nombre de la referencia de ofertas

int numP = 5;  // Cantidad de productos
int codP[] = {1, 2, 3, 4, 5};  // Codigos de los productos
char *nbreP[] = {"Manzana", "Pera", "Jabon", "Bebida", "Pan"};  // Nombres de los productos
int precioP[] = {200, 150, 300, 800, 80};  // Precios de los productos

int numO = 3;  // Cantidad de ofertas
int codO[] = {4, 5};  // Codigos de los productos en oferta
int cantidadO[] = {3, 6};  // Cantidad de productos por oferta
int descuentoO[] = {800, 160};  // Descuentos

// ---------- MODIFICAR ESTO ----------

typedef struct {
	int cod_producto;
	char nbre_producto[31];
	int precio;
} producto;

typedef struct {
	int cod_producto;
	int cantidad_descuento;
	int descuento;
} oferta;

producto P;
oferta O;

int main() {
	FILE *outP = fopen(OUTP, "w");
	if (outP == NULL) { printf("Error al abrir %s\n", OUTP); exit(-1); }
	FILE *refP = fopen(REFP, "w");
	if (refP == NULL) { printf("Error al abrir %s\n", REFP); exit(-1); }
	FILE *outO = fopen(OUTO, "w");
	if (outO == NULL) { printf("Error al abrir %s\n", OUTO); exit(-1); }
	FILE *refO = fopen(REFO, "w");
	if (refO == NULL) { printf("Error al abrir %s\n", REFO); exit(-1); }

	fwrite(&numP, sizeof(int), 1, outP);
	fprintf(refP, "Cantidad de structs: %d\n", numP);
	for (int i = 0; i < numP; i++) {
		P.cod_producto = codP[i];
		strcpy(P.nbre_producto, nbreP[i]);
		P.precio = precioP[i];
		fwrite(&P, sizeof(producto), 1, outP);
		fprintf(refP, "\n[Struct %d]\n", i + 1);
		fprintf(refP, "  cod_producto: %d\n  nbre_producto: %s\n  precio: %d\n", P.cod_producto, P.nbre_producto, P.precio);
	}

	fwrite(&numO, sizeof(int), 1, outO);
	fprintf(refO, "Cantidad de structs: %d\n", numO);
	for (int i = 0; i < numO; i++) {
		O.cod_producto = codO[i];
		O.cantidad_descuento = cantidadO[i];
		O.descuento = descuentoO[i];
		fwrite(&O, sizeof(oferta), 1, outO);
		fprintf(refO, "\n[Struct %d]\n", i + 1);
		fprintf(refO, "  cod_producto: %d\n  cantidad_descuento: %d\n  descuento: %d\n", O.cod_producto, O.cantidad_descuento, O.descuento);
	}

	fclose(outP);
	fclose(refP);
	fclose(outO);
	fclose(refO);

	return 0;
}