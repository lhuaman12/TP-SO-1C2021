


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




t_log* miRam_logger;
t_config* miRam_config;
int* ram;


void iniciar_logger();
void iniciar_config();
void reservar_memoria();
//int iniciar_mapa(char*,int,int);
int iniciar_escucha_a_tripulantes();


#endif  MIRAM_H_


















