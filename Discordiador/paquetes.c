#include "paquetes.h"


void enviar_prueba(int socket)
{
	t_patota_envio* patota = malloc(sizeof(t_patota_envio));
	patota->id_patota = "0";
	patota->tareas="2,HACER_CACA,MATEO,GOD";
	patota->trips="2,MATEO,1,3,SEGURO,BRUNO,3,4,INSEGURO";

	//patota = encriptar_patota(char* id,char* cant_tareas,char* cant_trip,char* ruta_tareas,char* list_trip);

	enviar_patota(patota,socket);
}

t_patota_envio* crear_patota(char* id,char* tareas,char* trips)
{
	t_patota_envio* nuevaPatota= malloc(sizeof(t_patota_envio));

	nuevaPatota->id_patota= id;
	nuevaPatota->tareas = tareas;
	nuevaPatota->trips = trips;

	return nuevaPatota;
}
void enviar_cambio_estado(int socket,char* estado, int tid)
{
	char* mensaje = malloc(strlen(estado)+1);
	strcpy(mensaje,"");
	char* id = string_itoa(tid);
	string_append_with_format(&mensaje,"%s,",id);
	string_append_with_format(&mensaje,"%s,",estado);

	conectar_envio(socket,configuracion_user->ip_ram,configuracion_user->puerto_ram);

	enviar_mensaje_por_codigo(mensaje,CAMBIO_ESTADO,socket);

	free(mensaje);
	free(id);

}


void enviar_IO(int socket,char* tarea,char* cantidad)
{
	conectar_envio(socket,configuracion_user->ip_file_system,configuracion_user->puerto_file_system);

	if(strcmp(tarea,"GENERAR_OXIGENO")==0)
	{
		enviar_mensaje_por_codigo(cantidad,GENERAR_OXIGENO,socket);
	}else if(strcmp(tarea,"GENERAR_COMIDA")==0)
	{
		enviar_mensaje_por_codigo(cantidad,GENERAR_COMIDA,socket);
	}else if(strcmp(tarea,"GENERAR_BASURA")==0)
	{
		enviar_mensaje_por_codigo(cantidad,GENERAR_BASURA,socket);
	}else if(strcmp(tarea,"CONSUMIR_OXIGENO")==0)
	{
		enviar_mensaje_por_codigo(cantidad,CONSUMIR_OXIGENO,socket);
	}else if(strcmp(tarea,"CONSUMIR_COMIDA")==0)
	{
		enviar_mensaje_por_codigo(cantidad,CONSUMIR_COMIDA,socket);
	}else if(strcmp(tarea,"CONSUMIR_BASURA")==0)
	{
		enviar_mensaje_por_codigo(cantidad,CONSUMIR_BASURA,socket);
	}

	//liberar_conexion(socket);
}

void enviar_inicio_tarea(int socket,char* tarea,int tid)
{
	conectar_envio(socket,configuracion_user->ip_file_system,configuracion_user->puerto_file_system);

	char* mensaje = malloc(strlen(tarea)+1);
	strcpy(mensaje,"");
	char* id = string_itoa(tid);
	string_append_with_format(&mensaje,"%s,",id);
	string_append_with_format(&mensaje,"%s,",tarea);

	enviar_mensaje_por_codigo(mensaje,REGISTRAR_INICIO_TAREA,socket);
	free(mensaje);
	free(id);

}
void enviar_fin_tarea(int socket,char* tarea,int tid)
{
	conectar_envio(socket,configuracion_user->ip_file_system,configuracion_user->puerto_file_system);

	char* mensaje = malloc(strlen(tarea)+1);
	strcpy(mensaje,"");
	char* id = string_itoa(tid);
	string_append_with_format(&mensaje,"%s,",id);
	string_append_with_format(&mensaje,"%s,",tarea);

	enviar_mensaje_por_codigo(mensaje,REGISTRAR_FIN_TAREA,socket);
	free(mensaje);
	free(id);

}
void enviar_patota(t_patota_envio* patota,int socket_ram)
{
	enviar_mensaje_por_codigo(patota->id_patota,INICIAR_PATOTAS,socket_ram);
	//sleep(1);
	enviar_mensaje_por_codigo(patota->tareas,RECIBIR_TAREAS,socket_ram);
	//sleep(1);
	enviar_mensaje_por_codigo(patota->trips,RECIBIR_TRIPS,socket_ram);
	//sleep(1);
	enviar_mensaje_por_codigo("0",FIN_PATOTA,socket_ram);

}


void enviar_movimiento(t_tripulante* trip)
{
	char* palabra = malloc(100);
	strcpy(palabra,"");

	char* tid = string_itoa(trip->TID);
	char* posx = string_itoa(trip->posicion->x);
	char* posy = string_itoa(trip->posicion->y);

	string_append_with_format(&palabra,"%s,",tid);
	string_append_with_format(&palabra,"%s,",posx);
	string_append_with_format(&palabra,"%s,",posy);

	enviar_mensaje_por_codigo(palabra,REGISTRAR_MOVIMIENTO,SOCKET_IMONGO);
	free(palabra);

}

void enviar_actualizar_pos(t_tripulante* trip)
{
	char* palabra = malloc(100);
	strcpy(palabra,"");

	char* tid =	string_itoa(trip->TID);
	char* posx = string_itoa(trip->posicion->x);
	char* posy = string_itoa(trip->posicion->y);

	string_append_with_format(&palabra,"%s,",tid);
	string_append_with_format(&palabra,"%s,",posx);
	string_append_with_format(&palabra,"%s,",posy);

	enviar_mensaje_por_codigo(palabra,POSICION_TRIPULANTE_ACTUALIZADA,trip->socket_ram);

	//recibir_operacion(trip->socket_ram);
	//char* respuesta = recibir_y_guardar_mensaje(trip->socket_ram);



	//TODO: EVALUAR SI LLEGO BIEN

	free(palabra);

}









