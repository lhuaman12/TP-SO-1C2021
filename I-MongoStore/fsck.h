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
void responder_bitacora(Tripulante* trip);

//SABOTAJES
void repararSuperBloque();
void repararBitmap();

void repararSize(char* recurso);
void repararBlockCount(char* recurso);
void repararMD5(char* recurso,char caracter);



#endif /* FSCK_H_ */
