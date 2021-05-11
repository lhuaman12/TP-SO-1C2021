/*
 * servidor.c
 *
 *  Created on: 3 mar. 2019
 *      Author: utnso
 */

#include "servidor.h"


int main(void)
{
	//escuchar_y_enviar(IP,PUERTO_ESCUCHA_SERVER);














	/*
	logger = log_create("log.log", "Servidor", 1, LOG_LEVEL_DEBUG);

	pthread_t hilo1;

	void iterator(char* value)
	{
		printf("%s\n", value);
	}


	int server_fd = iniciar_servidor();

	log_info(logger, "Servidor listo para recibir al cliente en %d",server_fd);

	int cliente_fd = esperar_cliente(server_fd);
	log_info(logger,"El cliente es: %d", cliente_fd);
	pthread_create(&hilo1, NULL, enviar_a_cliente, cliente_fd);

	t_list* lista;

	while(1)
	{
		int cod_op = recibir_operacion(cliente_fd);
		switch(cod_op)
		{
	//	case CONEXION:
	//		recibir_codigo(cliente_fd); --------------------> Esto hay que sacarlo?

		case MENSAJE:
			//recibir_mensaje(cliente_fd);
			recibir_mensaje_encriptado(cliente_fd);
			break;
		case PAQUETE:
			lista = recibir_paquete(cliente_fd);
			printf("Me llegaron los siguientes valores:\n");
			list_iterate(lista, (void*) iterator);
			break;
		case -1:
			log_error(logger, "el cliente se desconecto. Terminando servidor");
			return EXIT_FAILURE;
		default:
			log_warning(logger, "Operacion desconocida. No quieras meter la pata");
			break;
		}
	}


	pthread_join(hilo1, NULL);
	return EXIT_SUCCESS;
*/


}




void *enviar_a_cliente(int conexion)
{
	char* leido;
		leido = readline(">");

		while(strcmp(leido,"\0") != 0) {
				enviar_mensaje(leido,conexion);
				free(leido);
				leido = readline(">");
			}
		enviar_mensaje(leido,conexion);
		free(leido);
}

/*
void recibir_codigo(int cliente_fd)
{
	char* mensaje;
	mensaje = recibir_y_guardar_mensaje(cliente_fd);


	int codigoConexion = atoi(mensaje);
	pthread_create(&hilo, NULL, enviar_a_cliente, cliente_fd);

}
*/


// string sabotaje = "HORA,LUGAR,CANT_PARTICIPANTES"




/*
void recibir_mensaje_encriptado(int cliente_fd)
{
	char* mensaje;
	char** mensaje_decriptado;

	mensaje = recibir_y_guardar_mensaje(cliente_fd);

	mensaje_decriptado = string_split(mensaje,",");


	int hora = atoi(mensaje_decriptado[0]);
	char* lugar = mensaje_decriptado[1];
	int cant_participantes = atoi(mensaje_decriptado[2]);

	hora += 18;

	//"2" -> 2
	//STRING -> INT

	log_info(logger,"La hora es %d", hora);
	log_info(logger,"La ubicacion es %s", lugar);
	log_info(logger,"La cantidad de tripulantes es %d", cant_participantes);


}*/
