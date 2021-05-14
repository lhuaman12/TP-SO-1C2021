/*
 * discordiador.h
 *
 *  Created on: 26 abr. 2021
 *      Author: utnso
 */

#ifndef DISCORDIADOR_H_
#define DISCORDIADOR_H_




#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>
#include<string.h>

#include<conexiones.h>
#include <conections.h>
#include "utils_cliente.h"



#define IP "127.0.0.1" // Esto creo que habria que sacarlo del .config
#define PUERTO_ESCUCHA_CLIENTE "4446"




// Por lo general, a menos que tengan que tener recursos compartidos, no es buena práctica usar variables globales.
//Nosotros lo estamos haciendo por la simplicidad del TP0 :)

t_log* discordiador_logger;
t_config* discordiador_config;
pthread_t hilo_escucha;

void* crear_escucha(int puerto);
void iniciar_logger(void);
void leer_config(void);


#endif /* DISCORDIADOR_H_ */
