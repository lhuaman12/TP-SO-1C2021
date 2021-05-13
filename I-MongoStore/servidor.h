/*
 * servidor.h
 *
 *  Created on: 3 mar. 2019
 *      Author: utnso
 */

#ifndef SERVIDOR_H_
#define SERVIDOR_H_

/*
#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<string.h>
#include<commons/string.h>
#include<commons/log.h>
#include<readline/readline.h>
#include "utils_servidor.h"
*/
// Estas estan todas incluidas en SharedUtils.h


#include<SharedUtils.h>

#define IP "127.0.0.1"
#define PUERTO_ESCUCHA_SERVER "4444"



t_log* logger;
pthread_t hilo;


void* enviar_a_cliente();
//void recibir_codigo();
//void recibir_mensaje_encriptado(int cliente_fd);


#endif /* SERVIDOR_H_ */
