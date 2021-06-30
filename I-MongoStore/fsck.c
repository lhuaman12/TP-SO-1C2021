/*
 * fsck.c
 *
 *  Created on: 24 jun. 2021
 *      Author: utnso
 */

#include "fsck.h"

void repararSuperBloque()
{
	int bloquesActuales = contarBloques();
	sobreEscribirSuperBloque(RUTA_SUPER_BLOQUE,bloquesActuales);
	log_debug(log_IMONGO,"<> SE REPARO EL SUPER BLOQUE");
}

void repararBitmap()
{
	int cant = list_size(bloquesUsados);
	log_info(log_IMONGO,"Lista: %d",cant);
	int index = 0;
	int numero;
	while(index < cant)
	{
		numero = list_get(bloquesUsados,index);
		estaEnBitmap(numero);
		index++;
	}
	log_debug(log_IMONGO,"<> SE REPARO EL BITMAP");
}

void repararSize(char* recurso)
{
	char* path = PUNTO_MONTAJE;
	string_append_with_format(&path,"/Files/%s.ims",recurso);
	FILE* metadata = fopen(path,"r+b");
}


void responder_bitacora(Tripulante* trip)
{

char* id = recibir_id(trip->conexion);

log_info(trip->log,"<> Buscando la bitacora de %s",id);
char* buffer[] = {"Se movio el tripulante a 1|2","Diego es puto","Luis es capo","Bruno es puto tamnien"};
//char* buffer[] = obtenerBitacora(id);
int tamanio = 4;
//buffer = exportarBitacora();

char* mensaje = string_new();
string_append_with_format(&mensaje,"%s,",string_itoa(tamanio+1));

for(int i=0;i<tamanio;i++)
{
	string_append_with_format(&mensaje,"%s,",buffer[i]);
}

enviar_mensaje_por_codigo(mensaje,BITACORA,trip->conexion);
}
