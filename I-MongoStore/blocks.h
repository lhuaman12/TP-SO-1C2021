#ifndef SRC_BLOCKS_H_
#define SRC_BLOCKS_H_

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

int getBlockQuantity(char** arrayDeBlocks);
char* getDatosDeBlocks(char** arrayDeBlocks, int size);
char* getPathDeBlock(int bloque);
void guardarDatosEnBlocks(char* contenidoNuevo, char** arrayDeBlocks);

#endif /* SRC_BLOCKS_H_ */