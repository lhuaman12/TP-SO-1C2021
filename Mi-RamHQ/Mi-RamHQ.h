#ifndef MIRAM_H_
#define MIRAM_H_

#include<stdio.h>
#include<stdlib.h>

#include<pthread.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>
#include<string.h>
#include "estructurasCliente-Servidor.h"
#include<conexiones.h>
#include <conections.h>

#include <nivel-gui/nivel-gui.h>
#include <nivel-gui/tad_nivel.h>
#include <curses.h>
#include <stdbool.h>
#include <inttypes.h>

// VALORES GLOBALES DE CONFIG

int PUERTO_ESCUCHA_MIRAM;
char* ESQUEMA_MEMORIA;
int TAMANIO_MEMORIA_RAM;
char* CRITERIO_ELECCION_DE_SEGMENTO;
int tamanioPCB=8;
int tamanioTCB=21;
int tamanioTablaSegmento=9;
uint32_t tam_frame;

// DECLARACIÓN DE LOG,CONFIG,RAM, LISTAS Y TIPOS DE SEGMENTOS.

t_log* miRam_logger;
t_config* miRam_config;
uint32_t* ram;
t_list* tablaDeSegmentosLibres;
t_list* listaDeTablas;



t_list* l_tablas;

//ESTRUCTURAS TIPOS DE SEGMENTOS

typedef enum{
PCB,
TCB,
TAREAS
}tipo_dato_guardado;



typedef struct {

	bool ocupado;
	uint32_t* base;
	uint32_t* limite;
	tipo_dato_guardado tipo_dato;
	int pid;

}t_tabla_segmentos;




//  ESTRUCTURAS DE PCB , TCB Y CONJUNTO DE TAREAS

//PATOTA CONTROL BLOCK
typedef struct{
	uint32_t pid;
	uint32_t tareas; // direccion logica del inicio de las tareas QUE CARAJO SE LE ASIGNA A UNA TAREA????
}pcb;

// TRIPULANTE CONTROL BLOCK
typedef struct{
	uint32_t tid;
	uint32_t posicionX;
	uint32_t posicionY;
	uint32_t idProxInstruccion;
	void* ubicacionPCBtripulante;
	char estado;
}tcb;

// ESTRUCTURA NODO PRINCIPAL DE TAREAS

typedef struct{
	t_list* listaTareas;
}tipo_tarea;


typedef struct{
	uint32_t tid;
	uint32_t pos_y;
	uint32_t pos_x;
	char estado;
}t_tripulante;

typedef struct{
	t_list* tarea;
}listaTareas;



typedef struct{
	uint32_t nroFrame;
	uint32_t desplazamiento;
}t_pagina;


typedef struct{
	uint32_t pid;
	t_pagina* paginas;
}tabla_paginas_proceso;





// DECLARACION DE FUNCIONES UTILIZADAS

void iniciar_logger();
void iniciar_config();
void reservar_memoria();
void guardar_cosa_en_segmento_adecuado(void *cosa,uint32_t tamanioCosa,tipo_dato_guardado tipoCosa,t_list* tablaDeProceso);
void prender_server();
int existeSegmento();
pcb* crear_PCB(uint32_t pid, uint32_t tareas);
tcb* crear_TCB(t_tripulante* tripulante,uint32_t proxInstruccion,void* ubicacionPCB);
//tcb* crear_TCB(uint32_t tid,uint32_t pos_x, uint32_t pos_y,char estadoTripulante,uint32_t proxInstruccion,uint32_t direccionPCB);
void crear_estructuras();
t_tabla_segmentos* crear_primer_segmento();
bool existe_segmento_libre(uint32_t tamanioCosa);
t_tabla_segmentos* recortar_segmento_y_devolverlo(uint32_t tamanio);
t_tabla_segmentos* buscar_segmento_segun_criterio(uint32_t tamanioCosa);
t_tabla_segmentos* buscar_segmento_libre_primer_ajuste(uint32_t tamanio);
void agregarSegmentoRestanteATabla(void* limite,void* base, int indiceLimite);
int encontrar_indice(t_tabla_segmentos* segmentoBuscado);
t_tabla_segmentos* buscar_segmento_libre_mejor_ajuste(uint32_t tamanio);
void mostrarElemento();
void* atender_tripulante(Tripulante* trip);


void iniciarPatota(Tripulante* trip);
void expulsar_tripulante(Tripulante* trip);
void expulsar_tripulante_de_patota(uint32_t tid, uint32_t pid);
t_tabla_segmentos* retornaTCB(t_tabla_segmentos* cosa);
void actualizar_posicion_tripulante(Tripulante* trip);
void actualizarIdTareaARealizar(Tripulante* trip);



//void recibir_mensaje_encriptado(int cliente_fd,t_log* logg);
//void asignar_escuchas(int conexion_server,int puerto);
//void* atender_tripulante(Tripulante* trip);
//void recibir_iniciar_patota(int cliente,t_log* logg);
//void recibir_solicitud_tarea(int cliente,t_log* logg);
//void recibir_expulsar_tripulante(int cliente,t_log* logg);
//void recibir_posicion(int cliente,t_log* logg);


//int iniciar_mapa(char*,int,int);
//int iniciar_escucha_a_tripulantes();




uint32_t* obtener_pos_frame(int numeroDeFrame);





#endif MIRAM_H



