#include "discordiador.h"
#include "consola.h"

// la estructura de los tripulantes tiene que ser una cola porque es fifo por defecto


int main(void) {

	pthread_t consola;
	iniciar_logger("disc.log");
	setear_configs();

	configurar_planificacion();

	iniciar_conexiones();

	pthread_create(&consola, NULL, (void*) consola_discordiador, NULL);

	pthread_join(consola,NULL);

	return 0;
}



void iniciar_conexiones()
{
	SOCKET_RAM = crearSocket();
	conectar_envio(SOCKET_RAM,configuracion_user->ip_ram,configuracion_user->puerto_ram);

	SOCKET_IMONGO =crearSocket();
	conectar_envio(SOCKET_IMONGO,configuracion_user->ip_file_system,configuracion_user->puerto_file_system);
	//enviar_mensaje_por_codigo("0",SABOTAJE,SOCKET_IMONGO);

	SOCKET_SABOTAJE = crearSocket();
	pthread_create(&hiloIMONGO,NULL,(void*)escuchaSabotajes,NULL);
	pthread_detach(hiloIMONGO);



}

void atender_sabotaje(int socket)
{
	//px,py
	char* posicion = recibir_y_guardar_mensaje(socket);
	char** mensaje_dec = string_split(posicion,"|");
	int x = atoi(mensaje_dec[0]);
	int y = atoi(mensaje_dec[1]);
	log_info(discordiador_logger,"Me llego un sabotaje en X:%d Y:%d",x,y);
	sabotaje->posicion->x = x;
	sabotaje->posicion->y = y;
	sabotaje->hay_sabotaje=true;
	liberar_conexion(socket);

	free(mensaje_dec);
}

void* escuchaSabotajes()
{
	int conexion = -1;
	escuchaEn(SOCKET_SABOTAJE,(configuracion_user->puerto_file_system)+1);
	while(1)
	{
		conexion = aceptarConexion(SOCKET_SABOTAJE);

		if(conexion!=-1)
		{
			log_info(discordiador_logger,"CONECTO");
			recibir_operacion(conexion);
			atender_sabotaje(conexion);
		}

	}

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
	configuracion_user->ip_ram = string_duplicate(config_get_string_value(discordiador_config,"IP_MI_RAM_HQ"));
	configuracion_user->puerto_ram = config_get_int_value(discordiador_config,"PUERTO_MI_RAM_HQ");

	configuracion_user->ip_file_system = string_duplicate(config_get_string_value(discordiador_config,"IP_I_MONGO_STORE"));
	configuracion_user->puerto_file_system= config_get_int_value(discordiador_config,"PUERTO_I_MONGO_STORE");

	configuracion_user->algoritmo= string_duplicate(config_get_string_value(discordiador_config,"ALGORITMO"));
	configuracion_user->retardo_ciclo_cpu = atoi(config_get_string_value(discordiador_config,"RETARDO_CICLO_CPU"));
	configuracion_user->grado_multitarea=atoi(config_get_string_value(discordiador_config,"GRADO_MULTITAREA"));
	configuracion_user->quantum = atoi(config_get_string_value(discordiador_config,"QUANTUM"));

	configuracion_user->duracion_sabotaje = config_get_int_value(discordiador_config,"DURACION_SABOTAJE");

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

