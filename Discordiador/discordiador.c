#include "discordiador.h"
#include<readline/readline.h>


int main(void)
{

	char*ip;
	int PUERTO_PARA_ENVIAR;
	int PUERTO_ESCUCHA = atoi(PUERTO_ESCUCHA_CLIENTE);
	int codigoDeConexion = crearSocket();



	iniciar_logger();
	leer_config();
	ip = config_get_string_value(discordiador_config,"IP");
	PUERTO_PARA_ENVIAR = atoi(config_get_string_value(discordiador_config,"PUERTO_PARA_ENVIAR"));
	pthread_create(&hilo_escucha,NULL,crear_escucha,PUERTO_ESCUCHA);

	conectar_envio(codigoDeConexion,ip,PUERTO_PARA_ENVIAR,discordiador_logger);
	chat(codigoDeConexion);

	pthread_join(hilo_escucha,NULL);
	terminar_programa(codigoDeConexion,discordiador_config,discordiador_logger);
}

void* crear_escucha(int puerto)
{
	int codigoEscucha = crearSocket();
	log_info(discordiador_logger,"CLIENTE LISTO PARA ESCUCHAR");
			int conexion_escucha = escuchar_puerto(codigoEscucha,puerto);
			if(conexion_escucha == -1)
			{
				log_info(discordiador_logger,"ERROR: LA CONEXION ES -1");
			}

			while(1)
			{
				int cod_op = recibir_operacion(conexion_escucha);
				switch(cod_op)
				{

				case MENSAJE:
					//recibir_mensaje(cliente_fd);
					log_info(discordiador_logger,"ME LLEGO UN MENSAJE");
					recibir_mensaje_encriptado(conexion_escucha,discordiador_logger);
					break;
				case -1:
					log_error(discordiador_logger, "El cliente se desconecto. Terminando servidor");
					return EXIT_FAILURE;
				default:
					log_warning(discordiador_logger, "Operacion desconocida. No quieras meter la pata");
					break;
				}
			}
			return EXIT_SUCCESS;
}


void iniciar_logger(void)
{
	discordiador_logger = log_create("disc.log", "DISCORDIADOR", 1, LOG_LEVEL_INFO);
}


void leer_config(void)
{
	char* valor;

	discordiador_config = config_create("../discordiador.config");

}


