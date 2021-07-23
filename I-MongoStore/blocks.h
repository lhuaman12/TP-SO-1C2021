#ifndef SRC_BLOCKS_H_
#define SRC_BLOCKS_H_

#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<commons/collections/list.h>
#include<commons/bitarray.h>

#include"servidor.h"


typedef struct
{
	int id;
	int size;
	int libre;
	char* contenido;

} t_bloque;
//BLOQUES EN MEMORIA
void cargarBLoqueEnMemoria(char* contenido,int id_bloque);
void liberarBloque(int id_bloque);
void liberarBloqueMemoria(int id_bloque);
void copiarADisco();
void copiarAMemoria();

void borrarContenido(char* id);
void sacarCantidad(int id,int cantidad,char* path);

char* limpiarContenido(char* buffer);
int buscarSizeContenido(char* id);
//BLOQUES EN DISCO
void guardarEnBloque(char* contenido,int id_bloque);

int contarComas(char* palabra);
int contarBloques();
int calcularBloquesPorContenido(char* contenido);
int calcularBloquesPorTamanio(int tamanio);
#endif /* SRC_BLOCKS_H_ */
