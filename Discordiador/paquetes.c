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

t_patota_envio* encriptar_patota(char* id,char* cant_tareas,char* cant_trip,char* ruta_tareas,char* list_trip)
{

}


void enviar_patota(t_patota_envio* patota,int socket_ram)
{
	enviar_mensaje_por_codigo(patota->id_patota,INICIAR_PATOTAS,socket_ram);
	sleep(1);
	enviar_mensaje_por_codigo(patota->tareas,RECIBIR_TAREAS,socket_ram);
	sleep(1);
	enviar_mensaje_por_codigo(patota->trips,RECIBIR_TRIPS,socket_ram);
	sleep(1);
	enviar_mensaje_por_codigo("0",FIN_PATOTA,socket_ram);

}
