//#include "discordiador.h"
#include "consola.h"

int conexion_ram_hq;
int conexion_mongo_store;

// la estructura de los tripulantes tiene que ser una cola porque es fifo por defecto


int main(void) {
	pthread_t consola;
	iniciar_logger("logs/disc.log");
	setear_configs();
	configurar_planificacion();
	//TODO: levantar conexion con mi mongostore
	//iniciar_conexiones();
	pthread_create(&consola, NULL, (void*) consola_discordiador, NULL);

	pthread_join(consola,NULL);

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
	discordiador_config = config_create("configs/discordiador.config");

	//obtener las configs
	configuracion_user->ip_ram = string_duplicate(config_get_string_value(discordiador_config,"IP_MI_RAM_HQ"));
	configuracion_user->puerto_ram =string_duplicate( config_get_string_value(discordiador_config,"PUERTO_MI_RAM_HQ"));
	configuracion_user->algoritmo= string_duplicate(config_get_string_value(discordiador_config,"ALGORITMO"));
	configuracion_user->retardo_ciclo_cpu = atoi(config_get_string_value(discordiador_config,"RETARDO_CICLO_CPU"));
	configuracion_user->grado_multitarea=atoi(config_get_string_value(discordiador_config,"GRADO_MULTITAREA"));
	configuracion_user->quantum = atoi(config_get_string_value(discordiador_config,"QUANTUM"));
	/////
	inicializar_estructuras();

	config_destroy(discordiador_config);
}

void configurar_planificacion(){
	//if(strcmp(configuracion_user->algoritmo,"FIFO")==0){
		estructura_planificacion=malloc(sizeof(t_estructura_fifo));
		estructura_planificacion->cola_tripulantes_block=queue_create();
		estructura_planificacion->cola_tripulantes_new=queue_create();
		estructura_planificacion->cola_tripulantes_ready=queue_create();
		estructura_planificacion->tripulantes_exec=list_create();
		estructura_planificacion->cola_tripulantes_block_emergencia = queue_create();
	//}
	//else
//		log_warning(discordiador_logger,"Error al leer el algoritmo de planificacion");

}


void inicializar_estructuras(){
	lista_patotas=list_create();
	//// contador
	tid_contador=0;
	/// TODO: sacar cuando conectamos a ram
	tareas_io = malloc(sizeof(char*)*6);
	tareas_io[0]=string_duplicate("GENERAR_OXIGENO");
	tareas_io[1]=string_duplicate("CONSUMIR_OXIGENO");
	tareas_io[2]=string_duplicate("GENERAR_COMIDA");
	tareas_io[3]=string_duplicate("CONSUMIR_COMIDA");
	tareas_io[4]=string_duplicate("GENERAR_BASURA");
	tareas_io[5]=string_duplicate("DESCARTAR_BASURA");

	//// para pausar la planificacion
	pthread_mutex_init(&mutex_pausa,NULL);
	pthread_cond_init(&planificacion_pausa,NULL);
	planificador_pausado=0;
	levantar_hilo_planificacion=1;
	pausado=0;
	//
	sabotaje=malloc(sizeof(t_sabotaje));
	sabotaje->hay_sabotaje=0;
	sabotaje->posicion=malloc(sizeof(t_posicion));

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
			printf("holis");
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
