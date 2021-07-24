/*
 * miram.h
 *
 *  Created on: 23 jul. 2021
 *      Author: utnso
 */

#ifndef MIRAM_H_
#define MIRAM_H_

#include "estructurasMemoria.h"
#include "estructurasCliente-Servidor.h"
//
#include <stdio.h>
#include <stdlib.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/config.h>
#include <commons/bitarray.h>
#include <readline/readline.h>
#include <pthread.h>
#include <stdarg.h>
#include <math.h>
#include "mapa.h"

//

typedef enum
{
	SEGMENTACION,
	PAGINACION
}tipo_memoria;


//VARIABLES GLOBALES CONFIG;

int SOCKET_MIRAM;

int PUERTO_MIRAM;
char* IP_MIRAM;
char* RUTA_LOG;
int TAM_MEM; 		// tamanio en bytes de la memoria real
char* ESQUEMA_MEM; 		// Esquema de administración de memoria, puedeser: SEGMENTACION o PAGINACION
int TAM_PAG; 		// Tamaño en bytes de la cada página
int TAM_SWAP; 			// Tamaño en bytes del área de SWAP
char* PATH_SWAP; 			// Ubicación del archivo de SWAP
char* ALGORITMO_VM; 	// LRU o CLOCK - Indica el algoritmo de reemplazo a utilizar en el esquema de paginación
char* CRITERIO_SELECCION;   // El tipo de algoritmo de selección de partición libre a utilizar en el esquema de segmentación  (FF/BF)

t_log* miRam_logger;
t_config* miRam_config;

void* atender_tripulante(Tripulante* trip);
void cargar_configuracion(void);
void manejar_tripus(int socket);
void manejarConexion(int socket);
void prender_server();
void dump();
void signalCompactacion(int sig);
void signalDump(int sig);


#endif /* MIRAM_H_ */
