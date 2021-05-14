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
	int conexion_escucha;
	int puerto_escucha = atoi(PUERTO_ESCUCHA_SERVER);
	int puerto_envio = atoi(PUERTO_ENVIO_SERVER);
	logger = log_create("log.log", "Servidor", 1, LOG_LEVEL_DEBUG);
	int conexion_server = crearSocket();
	log_info(logger,"SERVIDOR LISTO");
	conexion_escucha = escuchar_puerto(conexion_server,puerto_escucha);
		if(conexion_escucha == -1)
		{
			log_info(logger,"ERROR: LA CONEXION ES -1");
		}
	pthread_create(&hilo_envio,NULL,abrir_chat,puerto_envio);

		while(1)
		{
			int cod_op = recibir_operacion(conexion_escucha);
			switch(cod_op)
			{

			case MENSAJE:
				//recibir_mensaje(cliente_fd);
				recibir_mensaje_encriptado(conexion_escucha,logger);
				break;
			case -1:
				log_error(logger, "El cliente se desconecto. Terminando servidor");
				return EXIT_FAILURE;
			default:
				log_warning(logger, "Operacion desconocida. No quieras meter la pata");
				break;
			}
		}
		pthread_join(hilo_envio,NULL);
		return EXIT_SUCCESS;



}

void* abrir_chat(int PUERTO_PARA_ENVIAR)
{
	int conexion_envio = crearSocket();
	log_info(logger,"SERVER ENVIA MENSAJES");
	sleep(5);
	conectar_envio(conexion_envio,IP,PUERTO_PARA_ENVIAR,logger);
	chat(conexion_envio);
}


