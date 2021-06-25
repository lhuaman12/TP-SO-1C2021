/*
 * interfaz.h
 *
 *  Created on: 15 jun. 2021
 *      Author: utnso
 */

#ifndef INTERFAZ_H_
#define INTERFAZ_H_

#include<string.h>

#include"servidor.h"

char** cortarPalabras(char* palabra,int cantidad);
void guardarContenido(char* path, char* contenido);
void guardarRecurso(char* recurso,char* contenido);


#endif /* INTERFAZ_H_ */
