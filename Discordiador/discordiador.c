#include "discordiador.h"
#include<readline/readline.h>
int main(void)
{
	int CodigoDeconexion;


//	discordiador_config = config_create("/home/utnso/tp-2021-1c-bastardosSinGloria/Discordiador/discordiador.config");
//SE INICIA DISCORDIADOR_LOGGER

	iniciar_logger();


	leer_config();
	//leer_consola();

		//ip = config_get_string_value(discordiador_config,"IP");
		//puerto = config_get_string_value(discordiador_config, "PUERTO");
		//crear_conexion(ip, puerto);
	//char* valor;
	//valor = config_get_string_value(discordiador_config, "CLAVE");
	//enviar_mensaje(valor, CodigoDeconexion);
	//paquete(conexion);


	CodigoDeconexion = conectar_con_servidor();
	//abrir_chat(CodigoDeconexion);
	chat(CodigoDeconexion);
	terminar_programa(CodigoDeconexion);
}

void abrir_chat(CodigoDeconexion){

	char* valor;

	valor = config_get_string_value(discordiador_config, "CLAVE");

	enviar_mensaje(valor, CodigoDeconexion);

}

int conectar_con_servidor(){
	char* ip;
	char* puerto;

	ip = config_get_string_value(discordiador_config,"IP");
	puerto = config_get_string_value(discordiador_config, "PUERTO");

	return crear_conexion(ip,puerto);
}


void iniciar_logger(void)
{
	discordiador_logger = log_create("disc.log", "DISCORDIADOR", 1, LOG_LEVEL_INFO);

	log_info(discordiador_logger, "Cliente Discordiador");
}


void leer_config(void)
{
	char* valor;

	discordiador_config = config_create("/home/utnso/tp-2021-1c-bastardosSinGloria/Discordiador/discordiador.config");

	//tenemos que levantar en valor asociado a la clave "CLAVE" del archivo y asignarselo a la variable valor
	valor = config_get_string_value(discordiador_config,"CLAVE");


	log_info(discordiador_logger, "ASIGNO VALOR:");

	//Por último, logeame ese valor :)
	log_info(discordiador_logger, valor);

}

/*
void leer_consola(void)
{
	char* leido;

	//El primero te lo dejo de yapa
	leido = readline("<>");

	while(strcmp(leido,"\0") != 0) {
		log_info(discordiador_logger, leido);
		free(leido);
		leido = readline(">");
	}

	free(leido);
}*/

void chat(int conexion)
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
void paquete(int conexion)
{

	char* leido;
	t_paquete* paquete = crear_paquete();

	leido = readline(">");

	while(strcmp(leido,"\0") != 0) {
		agregar_a_paquete(paquete, leido, strlen(leido) + 1);
		free(leido);
		leido = readline(">");
	}

	free(leido);
	enviar_paquete(paquete, conexion);

	eliminar_paquete(paquete);
}
*/
void terminar_programa(int conexion)
{
	liberar_conexion(conexion);
	log_destroy(discordiador_logger);
	config_destroy(discordiador_config);
}
