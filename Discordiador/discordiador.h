#ifndef DISCORDIADOR_H_
#define DISCORDIADOR_H_

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/config.h>
#include <readline/readline.h>
#include <string.h>
#include <semaphore.h>
#include <commons/collections/list.h>


#include"paquetes.h"



// dejo las configs en memoria por ahora

//CONEXIONES
int SOCKET_RAM;
int SOCKET_IMONGO;
int SOCKET_SABOTAJE;
pthread_t hiloIMONGO;

//OTROS
typedef struct {
	char *ip_ram;
	int puerto_ram;
	char *ip_file_system;
	int puerto_file_system;
	int grado_multitarea;
	char *algoritmo;
	int quantum;
	int duracion_sabotaje;
	int retardo_ciclo_cpu;

}t_config_user;

// logger y configs
t_log* discordiador_logger;
t_config_user* configuracion_user;

// informacion para el discordiador
t_list* lista_patotas;
int tid_contador;
////// sabotajes
//int hay_sabotajes; //mutex aca


/// Para sincronizacion de los tripulantes y los ciclos de cpu

void* escucharRAM();
void* escucharIMONGO();
char* pedir_algo(int socket,char* pid);
void atender_RAM(int conexion);
void atender_IMONGO(int conexion);

void* escuchaSabotajes();
void leer_bitacora(int socket);
void atender_sabotaje(int socket);


void iniciar_logger(char* path);
void leer_configs(void);
void setear_configs();
void iniciar_conexiones();
void inicializar_estructuras();
void configurar_planificacion();

#endif /* DISCORDIADOR_H_ */
