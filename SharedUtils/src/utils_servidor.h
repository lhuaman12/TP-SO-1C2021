/*
 * conexiones.h
 *
 *  Created on: 3 mar. 2019
 *      Author: utnso
 */

#ifndef CONEXIONES_H_
#define CONEXIONES_H_

#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/log.h>
#include<commons/collections/list.h>
#include<string.h>

#include "estructurasCliente-Servidor.h"

#define IP "127.0.0.1"
#define PUERTO "4444"

/*
typedef enum
{
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
*/

t_log* logger;

void* recibir_buffer(int*, int);

int iniciar_servidor(char*,char*);
int esperar_cliente(int);
t_list* recibir_paquete(int);
void recibir_mensaje(int);
char* recibir_y_guardar_mensaje(int socket_cliente);
int recibir_operacion(int);
char* recibir_id(int socket_cliente);

void recibir_bitacora(int cliente_fd);

#endif /* CONEXIONES_H_ */
