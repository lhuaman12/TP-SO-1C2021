#ifndef SRC_BLOCKS_H_
#define SRC_BLOCKS_H_

#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<commons/collections/list.h>
#include<commons/bitarray.h>

#include"servidor.h"

void guardarEnBloque(char* contenido,int id_bloque);

int calcularBloquesPorContenido(char* contenido);

#endif /* SRC_BLOCKS_H_ */
