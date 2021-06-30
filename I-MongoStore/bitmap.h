#ifndef SRC_BITMAP_H_
#define SRC_BITMAP_H_


#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<commons/collections/list.h>
#include<commons/bitarray.h>

#include"servidor.h"


typedef struct
{
	int id_bit;
	int usado;
}t_bit;


//LOGICA DE BITMAP

/* Setea el bloque en uso en la estructura de bitarray
 * Tambien setea en "1" el bloque indicado en el archivo Bitmap.bin
 *
 * Retorna el numero de bloque a utilizar
 * Para modificar el metadata del file / abrir el bloque en concreto / etc.
 */
int solicitarBloque(void);


/* Libera el bloque indicado la estructura de bitarray
 * Setea en "0" el bloque indicado en el archivo Bitmap.bin
 */
void liberarBloque(int bloqueALiberar);

void solicitarBloquePorNumero(int id);

void guardarBackup(t_list* bitList,int bloque);


void estaEnBitmap(int bloque);

#endif /* SRC_BITMAP_H_ */
