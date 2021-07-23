/*
 * estructurasCliente-Servidor.h
 *
 *  Created on: 11 may. 2021
 *      Author: utnso
 */

#ifndef SRC_ESTRUCTURASCLIENTE_SERVIDOR_H_
#define SRC_ESTRUCTURASCLIENTE_SERVIDOR_H_


#include<stdlib.h>
#include<stdio.h>
#include<commons/log.h>

typedef enum
{
	GENERAR_OXIGENO,
	GENERAR_COMIDA,
	GENERAR_BASURA,
	CONSUMIR_OXIGENO,
	CONSUMIR_COMIDA,
	CONSUMIR_BASURA,
	BITACORA,
	OBTENER_BITACORA,
	EXPULSAR_TRIPULANTES,
	INICIAR_PATOTAS,
	ENVIAR_PROXIMA_TAREA,
	POSICION_TRIPULANTE_ACTUALIZADA,
	RECIBIR_TRIPULANTE,
	FIN_PATOTA,
	RECIBIR_TAREAS,
	RECIBIR_TRIPS,
	CONFIRMAR_PATOTA,
	MENSAJE,
	CONEXION,
	PAQUETE
}op_code;

typedef struct
{
	int size;
	void* stream;
} t_buffer;

typedef struct
{
	op_code codigo_operacion;
	t_buffer* buffer;
} t_paquete;


typedef struct
{
	//BASICO;
	int id_tripulante;
	int conexion;
	t_log* log;
	pthread_t hilo;
	pthread_mutex_t mutex;

}Tripulante;


Tripulante* crearTripulante();
void borrarTripulante(Tripulante* trip);

#endif /* SRC_ESTRUCTURASCLIENTE_SERVIDOR_H_ */
