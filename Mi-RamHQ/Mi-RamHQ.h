




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
