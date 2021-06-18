#include "discordiador.h"
#include "consola.h"



// la estructura de los tripulantes tiene que ser una cola porque es fifo por defecto


int main(void) {
	pthread_t consola;
	//pthread_t escucha_ramhq, escucha_mongostore;
	iniciar_logger("disc.log");
	inicializar_estructuras();
	setear_configs();
	char* ip = "127.0.0.1";
	conexion_mongo_store = crearSocket();
	conectar_envio(conexion_mongo_store,ip,4444);
	//iniciar_conexiones();
	//consola_discordiador();
	//pthread_create(&hilo_escucha_ramhq, NULL, (void*) hilo_escucha_ramhq, NULL);
	//TODO: levantar conexion con mi mongostore
	pthread_create(&consola, NULL, (void*) consola_discordiador, NULL);
	pthread_join(consola,NULL);
	//pthread_detach(consola);
	//pthread_detach(escucha_ramhq);

	/*
	 pthread_create(&hilo_escucha,NULL,crear_escucha,PUERTO_ESCUCHA);
	 int codigoDeConexion = crearSocket();
	 conectar_envio(codigoDeConexion,ip,PUERTO_PARA_ENVIAR,discordiador_logger);
	 chat(codigoDeConexion);
	 pthread_join(hilo_escucha,NULL);
	 terminar_programa(codigoDeConexion,discordiador_config,discordiador_logger);
	 */


	return 0;
}

void iniciar_logger(char* path) {
	discordiador_logger = log_create(path, "Discordiador logger", 1, LOG_LEVEL_INFO);
	log_info(discordiador_logger, "Inicio de logs de discordiador..");
}

void setear_configs() {
	t_config* discordiador_config;
	//logueamos el inicio
	log_info(discordiador_logger, "Seteando las configuraciones..");

	// reservo el espacio para las configs
	configuracion_user = malloc(sizeof(t_config_user));

	//inicializar la config
	discordiador_config = config_create("../discordiador.config");

	//obtener las configs
	//configuracion_user->ip_file_system = config_get_string_value(discordiador_config,"IP");
	//configuracion_user->ip_ram = config_get_string_value(discordiador_config,"IP_MI_RAM_HQ");
	//configuracion_user->puerto_ram = config_get_string_value(discordiador_config, "PUERTO_MI_RAM_HQ");
	//printf("%s", configuracion_user->ip_ram);
//	printf("%s", configuracion_user->puerto_ram);
	//TODO:crear las demas
	//TODO:en caso que no exista alguna configs loguear y señalar error (abortar ejecucion)

	///
	inicializar_estructuras();

	config_destroy(discordiador_config);
}

/// conexion bidireccional discordiador cliente y ramhq servidor
/*
void iniciar_conexiones(void) {
	conexion_ram_hq = crear_conexion(configuracion_user->ip_ram,
			configuracion_user->puerto_ram);
	log_info(logger, "conexion con ram hq establecida\n");
	conexion_mongo_store=crear_conexion(configuracion_user->ip_file_system,configuracion_user->puerto_file_system);
	 log_info(logger,"conexion con ram hq establecida\n");

}
*/
/*
void hilo_escucha_ramhq(void) {
	int op;
	char *mensaje;
	char **mensaje_descifrado;

	while (1) {
		op = recibir_operacion(conexion_ram_hq);
		switch (op) {
		case CONFIRMACION_INICIAR_PATOTA:
			printf("holis"); // TODO: ACCION poner tripulantes en ready
			break;
		}
		case -1:
			printf("Error");
			break;
		default:
			printf("Operacion Desconocida");
			break;

	}
}
*/
void iniciar_planificacion(){
	if(strcmp(configuracion_user->algoritmo,"FIFO")==0){
		//TODO:configurar fifo
		printf("FIFO");
	}
	else if(strcmp(configuracion_user->algoritmo,"RR")==0){
		//TODO:configurar RR
		printf("RR");
	}
	else {
		log_warning(discordiador_logger,"Error al abrir configuracion de planificacion");
		exit(1);
	}

}


void inicializar_estructuras(){
	lista_patotas=list_create();
	tid_actual=0;

}

/*
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

 */
