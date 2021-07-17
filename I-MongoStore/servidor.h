/*
 * servidor.h
 *
 *  Created on: 3 mar. 2019
 *      Author: utnso
 */

#ifndef SERVIDOR_H_
#define SERVIDOR_H_

#include<signal.h>
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

#include"archivos.h"
#include"blocks.h"
#include"interfaz.h"
#include"bitmap.h"
#include"fsck.h"

#include "recibir_paquete.h"

#define IP "127.0.0.1"


//VARIABLES DEL CONFIG
int PUERTO_DISC;
int TIEMPO_SINCRO;
char* PUNTO_MONTAJE;
char* POSICIONES_SABOTAJE;




//VARIABLES GLOBALES
int BLOCKS;
int BLOCK_SIZE;
int NUEVO_FS;
char* BITMAP;

// *PARA EL BITMAP
char* RUTA_BITMAP;
void* bufferBitArray;
t_bitarray* bitarray;


// *PARA LAS ESTRUCTURAS
char* RUTA_SUPER_BLOQUE;
char* RUTA_FILES;
char* RUTA_BLOCKS;
char* RUTA_BITACORA;

// *PARA LOS BLOQUES
t_list* bloques;

// *SEMAFOROS
pthread_mutex_t semaforoBitmap;
pthread_mutex_t semaforoBlock;
pthread_mutex_t semaforoSuperBloque;
pthread_mutex_t semaforoFiles;
pthread_mutex_t semaforoBitacoras;

// *LOGS, CONFIG E HILOS

t_log* log_IMONGO;
t_config* config_IMONGO;
t_config* super_config;
pthread_t hiloSincro;



//FUNCIONES
void* sincronizarDisco();
void prender_server();
void iniciar_log();
void leer_config();

void init_semaforos();
void iniciar_filesystem();
void init_bitmap();
void leer_super_bloque();
void init_directorios();

void crear_super_bloque();
void escribir_super_bloque();
void init_bloques_vacio();
void init_bitmap();
void init_bloques_usado();

void* atender_tripulante(Tripulante* trip);


char* buscar_tareas(char* path);
char* contarTareas(char* palabra);


#endif /* SERVIDOR_H_ */
