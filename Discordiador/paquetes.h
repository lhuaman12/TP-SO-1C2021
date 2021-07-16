/*
 * paquetes.h
 *
 *  Created on: 16 jul. 2021
 *      Author: utnso
 */

#ifndef PAQUETES_H_
#define PAQUETES_H_
#include "discordiador.h"

#include <conections.h>
#include <utils_cliente.h>


typedef struct
{
	char* id_patota;
	//cant_tareas,tarea1,tarea2,..,tareaN;
	char* tareas;
	//cant_trip,trip1,posx,posy,estado,trip2,posx,posy,estado,tripN,posx,posy,estado
	char* trips;

}t_patota_envio;

void enviar_prueba(int socket);
void enviarPatota(t_patota_envio* patota,int socket);
void enviar_patota(t_patota_envio* patota, int socket_cliente);

#endif /* PAQUETES_H_ */
