/*
 * servidor.c
 *
 *  Created on: 3 mar. 2019
 *      Author: utnso
 */

#include "servidor.h"


int main(void)
{
	//char*ip = IP;
	//int puerto_envio = atoi(PUERTO_ENVIO_SERVER);
	log_IMONGO = log_create("log.log", "I-MONGO-STORE", 1, LOG_LEVEL_DEBUG);
	//log_info(logger,"SERVIDOR LISTO");
	prender_server();
	//pthread_create(&hilo_envio,NULL,abrir_chat,puerto_envio);

}


void prender_server()
{
	int puerto_escucha = atoi(PUERTO_ESCUCHA_SERVER);
	int socket_interno = crearSocket();
	log_info(log_IMONGO,"SERVIDOR LISTO");
	asignar_escuchas(socket_interno,puerto_escucha);

}

void escuchar_cliente()
{
	int puerto_escucha = atoi(PUERTO_ESCUCHA_SERVER);
	int socket_interno = crearSocket();
	log_info(log_IMONGO,"SERVIDOR LISTO");
	socket_interno = escuchar_puerto(socket_interno,puerto_escucha,log_IMONGO);

	int cod_op = recibir_operacion(socket_interno);
				switch(cod_op)
				{

				case MENSAJE:
					//recibir_mensaje(cliente_fd);
					recibir_mensaje_encriptado(socket_interno,logger);
					break;
				case -1:
					log_error(log_IMONGO, "El cliente se desconecto. Terminando servidor");
					break;
				default:
					log_warning(log_IMONGO, "Operacion desconocida. No quieras meter la pata");
					break;
				}

}


//ABRE EL CHAT DE ENVIO AL CLIENTE. TIENE QUE ESPERAR QUE SE ABRA LA ESCUCHA DEL CLIENTE.
void* abrir_chat(int PUERTO_PARA_ENVIAR)
{
	int conexion_envio = crearSocket();
	log_info(log_IMONGO,"SERVER ENVIA MENSAJES");
	sleep(5);
	conectar_envio(conexion_envio,IP,PUERTO_PARA_ENVIAR,logger);
	chat(conexion_envio);
}


