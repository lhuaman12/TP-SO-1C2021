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
#include <conections.h>
#include<readline/readline.h>
#include "utils_servidor.h"

// Estas estan todas incluidas en SharedUtils.h


#include<conexiones.h>

#define IP "127.0.0.1"
#define PUERTO_ESCUCHA_SERVER "4444"
#define PUERTO_ENVIO_SERVER "4446"



t_log* logger;
pthread_t hilo_envio;

void* abrir_chat(int PUERTO_PARA_ENVIAR);
void* enviar_a_cliente();



#endif /* SERVIDOR_H_ */
