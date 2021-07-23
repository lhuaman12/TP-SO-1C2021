/*
 * estructurasCliente-Servidor.c
 *
 *  Created on: 22 may. 2021
 *      Author: utnso
 */

#include "estructurasCliente-Servidor.h"

Tripulante* crearTripulante()
{
	Tripulante* tripulante = malloc(sizeof(Tripulante));
	tripulante->log = log_create("trip.log", "TRIPULANTE", 1, LOG_LEVEL_DEBUG);
	tripulante->hilo = 0;
	tripulante->conexion = -1;
	return tripulante;

}

void borrarTripulante(Tripulante* trip)
{
	log_destroy(trip->log);
	close(trip->conexion);
	free(trip);
}
