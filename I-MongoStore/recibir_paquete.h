/*
 * recibir_paquete.h
 *
 *  Created on: 16 jul. 2021
 *      Author: utnso
 */

#ifndef RECIBIR_PAQUETE_H_
#define RECIBIR_PAQUETE_H_

#include "servidor.h"//"Mi-Ram.h"
#include <conections.h>



typedef struct
{
	char* id_patota;
	//cant_tareas,tarea1,tarea2,..,tareaN;
	char* tareas;
	//cant_trip,trip1,posx,posy,estado,trip2,posx,posy,estado,tripN,posx,posy,estado
	char* trips;

}t_patota_envio;

t_patota_envio* recibir_patota(int socket);
#endif /* RECIBIR_PAQUETE_H_ */
