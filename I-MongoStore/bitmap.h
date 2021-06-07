#ifndef SRC_BITMAP_H_
#define SRC_BITMAP_H_

#include<stdio.h>
#include<stdlib.h>
#include<readline/readline.h>
#include<signal.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<netdb.h>

#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<commons/collections/list.h>
#include<commons/bitarray.h>

#include"GameCard.h"

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

#endif /* SRC_BITMAP_H_ */