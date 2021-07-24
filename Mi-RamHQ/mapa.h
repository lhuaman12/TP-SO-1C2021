/*
 * mapa.h
 *
 *  Created on: 23 jul. 2021
 *      Author: utnso
 */

#ifndef MAPA_H_
#define MAPA_H_

#include <ncurses.h>
#include <nivel-gui/nivel-gui.h>
#include <nivel-gui/tad_nivel.h>

#include "miram.h"

int error;
int columnas;
int filas;

pthread_mutex_t mutexMapa;

void ASSERT_CREATE(char id,int error);
void crearMapaVacio();
void actualizar_Nivel();
void moverTripulante(char id , int posX, int posY);
void dibujar_tripulante( char id, int posX, int posY);
void expulsarDelMapa(char unId);
void finalizar_mapa();


NIVEL* nivel;




#endif /* MAPA_H_ */
