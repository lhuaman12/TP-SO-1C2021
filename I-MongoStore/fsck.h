/*
 * fsck.h
 *
 *  Created on: 24 jun. 2021
 *      Author: utnso
 */

#ifndef FSCK_H_
#define FSCK_H_

#include "servidor.h"

void restaurarRecurso(char* recurso, char caracter);
int analizarBloque(int posicion);
char* obtenerPosicion();

//SABOTAJES
void iniciarFSCK();
void repararFiles();

void repararSuperBloque();
void repararBitmap();

void repararSize(char* recurso);
void repararBlockCount(char* recurso);
void repararMD5(char* recurso,char caracter);


void responder_bitacora(Tripulante* trip);
void recibir_movimiento(int socket);
void recibir_inicio_tarea(int socket);
void recibir_fin_tarea(int socket);



#endif /* FSCK_H_ */
