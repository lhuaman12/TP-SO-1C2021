/*
 * servidor.h
 *
 *  Created on: 3 mar. 2019
 *      Author: utnso
 */

#ifndef SERVIDOR_H_
#define SERVIDOR_H_

#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<string.h>
#include<commons/string.h>
#include<commons/log.h>

#include "utils_servidor.h"


pthread_t hilo;


void* enviar_a_cliente();
void recibir_mensaje_encriptado(int cliente_fd);


#endif /* SERVIDOR_H_ */
