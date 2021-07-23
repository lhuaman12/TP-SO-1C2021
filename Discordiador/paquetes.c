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

	char* pid = string_itoa(trip->PID);
	char* posx = string_itoa(trip->posicion->x);
	char* posy = string_itoa(trip->posicion->y);

	string_append_with_format(&palabra,"%s,",pid);
	string_append_with_format(&palabra,"%s,",posx);
	string_append_with_format(&palabra,"%s,",posy);

	enviar_mensaje_por_codigo(palabra,REGISTRAR_MOVIMIENTO,SOCKET_IMONGO);

}










