/*
 * interfaz.h
 *
 *  Created on: 15 jun. 2021
 *      Author: utnso
 */

#ifndef INTERFAZ_H_
#define INTERFAZ_H_

#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<commons/collections/list.h>
#include<commons/bitarray.h>

#include"servidor.h"

int obtenerBytes(int cantidad);
void guardarRecursos(char* recurso, int cantidad,char* nombre);


#endif /* INTERFAZ_H_ */
