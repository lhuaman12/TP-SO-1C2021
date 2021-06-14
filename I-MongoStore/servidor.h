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
#include<commons/bitarray.h>
#include<commons/string.h>
#include<commons/config.h>
#include<commons/log.h>
#include<readline/readline.h>
#include <utils_servidor.h>

// Estas estan todas incluidas en SharedUtils.h

#include <conections.h>
#include<conexiones.h>

#include"archivos.h"

#define IP "127.0.0.1"
#define PUERTO_ESCUCHA_SERVER "4444"

//VARIABLES GLOBALES
int BLOCKS;
int BLOCK_SIZE;

// *PARA LA CONEXION
int PUERTO_ESCUCHA_IMONGO;

// *PARA EL BITMAP
char* RUTA_BITMAP;
void* bufferBitArray;
t_bitarray* bitarray;

// *PARA LAS ESTRUCTURAS
char* PUNTO_MONTAJE;

// *PARA LOS BLOQUES


// *SEMAFOROS


// *LOGS, CONFIG E HILOS

t_log* log_IMONGO;
t_config* config_IMONGO;
pthread_t hiloEscuchas;



//FUNCIONES
void prender_server();
void iniciar_log();
void leer_config(void);

void iniciar_filesystem();
void init_bitmap();



#endif /* SERVIDOR_H_ */
