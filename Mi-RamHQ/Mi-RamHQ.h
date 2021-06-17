




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


// VALORES GLOBALES DE CONFIG
int PUERTO_ESCUCHA_MIRAM;
char* ESQUEMA_MEMORIA;
int TAMANIO_MEMORIA_RAM;
char*  CRITERIO_ELECCION_DE_SEGMENTO;
int tamanioPCB=8;
int tamanioTCB=21;
int tamanioTablaSegmento=9;

// DECLARACIÓN DE LOG,CONFIG,RAM, LISTAS Y TIPOS DE SEGMENTOS.

t_log* miRam_logger;
t_config* miRam_config;
uint32_t* ram;
t_list* tablaDeSegmentos;


//ESTRUCTURAS TIPOS DE SEGMENTOS

typedef struct tipo_tabla_segmentos{

	bool ocupado;
	uint32_t* base;
	uint32_t* limite;

}t_tabla_segmentos;

typedef struct tipo_segmento{

	uint32_t numeroSegmento;
	_Bool ocupado;
	uint32_t* base;
	uint32_t* limite;
	void* cosa;

}t_segmento;




//  ESTRUCTURAS DE PCB , TCB Y CONJUNTO DE TAREAS

//PATOTA CONTROL BLOCK
typedef struct pcb{
	uint32_t pid;
	uint32_t tareas; // direccion logica del inicio de las tareas QUE CARAJO SE LE ASIGNA A UNA TAREA????
}pcb;

// TRIPULANTE CONTROL BLOCK
typedef struct tcb{
	uint32_t tid;
	uint32_t posicionX;
	uint32_t posicionY;
	uint32_t idProxInstruccion;
	uint32_t ubicacionPCBtripulante;
	char estado;
}tcb;



// ENUMERACION DE FUNCIONES UTILIZADAS

void iniciar_logger();
void iniciar_config();
void reservar_memoria();
void guardar_cosa_en_segmento_adecuado(void* cosa,uint32_t tamanioCosa);
void prender_server();
int existeSegmento();
pcb* crear_PCB(uint32_t pid, uint32_t tareas);
tcb* crear_TCB(uint32_t tid,uint32_t pos_x, uint32_t pos_y,char estadoTripulante,uint32_t proxInstruccion,uint32_t direccionPCB);
void crear_estructuras();
t_tabla_segmentos* crear_primer_segmento();
bool existe_segmento_libre(uint32_t tamanioCosa);
t_tabla_segmentos* recortar_segmento_y_devolverlo(uint32_t tamanio);
t_tabla_segmentos* buscar_segmento_segun_criterio(uint32_t tamanioCosa);
t_tabla_segmentos* buscar_segmento_libre_primer_ajuste(uint32_t tamanio);
void agregarSegmentoRestanteATabla(void* limite,void* base);
t_tabla_segmentos* buscar_segmento_libre_mejor_ajuste(uint32_t tamanio);


//int iniciar_mapa(char*,int,int);
//int iniciar_escucha_a_tripulantes();


#endif MIRAM_H


