/*
 * recibir_paquete.c
 *
 *  Created on: 16 jul. 2021
 *      Author: utnso
 */

#include "recibir_paquete.h"


t_patota_envio* recibir_patota(int socket)
{

	log_debug(log_IMONGO,"<>START: Recibir patota");
	t_patota_envio* patota = malloc(sizeof(t_patota_envio));
	patota->id_patota = recibir_id(socket);
	int i = 1;
	while(i==1)
	{
		int cod_op = recibir_operacion(socket);
		switch(cod_op)
		{
		case RECIBIR_TAREAS:
			patota->tareas = recibir_y_guardar_mensaje(socket);
			break;
		case RECIBIR_TRIPS:
			patota->trips = recibir_y_guardar_mensaje(socket);
			break;
		case FIN_PATOTA:
			i=0;
			break;
		default:
			log_error(log_IMONGO,"ERROR AL RECIBIR PATOTA");
			break;
		}

	}
	return patota;
	log_debug(log_IMONGO,"<>END: Recibir patota");

}
