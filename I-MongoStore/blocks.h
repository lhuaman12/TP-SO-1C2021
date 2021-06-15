#ifndef SRC_BLOCKS_H_
#define SRC_BLOCKS_H_

#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<commons/collections/list.h>
#include<commons/bitarray.h>

#include"servidor.h"


int getBlockQuantity(char** arrayDeBlocks);
char* getDatosDeBlocks(char** arrayDeBlocks, int size);
char* getPathDeBlock(int bloque);
void guardarDatosEnBlocks(char* contenidoNuevo, char** arrayDeBlocks);

#endif /* SRC_BLOCKS_H_ */
