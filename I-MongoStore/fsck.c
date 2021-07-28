/*
 * fsck.c
 *
 *  Created on: 24 jun. 2021
 *      Author: utnso
 */

#include "fsck.h"


char* obtenerPosicion()
{
	char* posiciones = POSICIONES_SABOTAJE;
	posiciones = string_substring_until(posiciones,strlen(posiciones)-1);
	posiciones = string_substring(posiciones,1,strlen(posiciones));
	char** posiciones_separadas = string_split(posiciones,",");
	char* valor = posiciones_separadas[numero_sabotaje];

	numero_sabotaje++;

	if(numero_sabotaje == sabotajes_max)
	{
		log_warning(log_IMONGO,"<> NO MAS SABOTAJES");
	}
	return valor;

	free(posiciones_separadas);

}

void iniciarFSCK()
{
	repararSuperBloque();
	repararBitmap();

	/////////////////////////

	//repararSize();
	//repararBlockCount();
	//repararMD5();
}





//PERFECTO
void repararSuperBloque()
{

	int bloquesActuales = contarBloques();
	sobreEscribirSuperBloque(RUTA_SUPER_BLOQUE,bloquesActuales);
	log_debug(log_IMONGO,"<> SE REPARO EL SUPER BLOQUE");

}

//PERFERCTO
void repararBitmap()
{
	int cant = BLOCKS;
	int index = 0;
	int valor;

	log_debug(log_IMONGO,"<> START: Reparar Bitmap");

	pthread_mutex_lock(&semaforoBitmap);
	FILE* bitmap = fopen(RUTA_BITMAP,"r+b");

	char buffer;

	while(index<cant)
	{
		fread(&buffer,sizeof(char),1,bitmap);

		valor = analizarBloque(index);

		fseek(bitmap, index,SEEK_SET);
		fwrite(string_itoa(valor),1,1,bitmap);

		index++;
	}

	fclose(bitmap);
	pthread_mutex_unlock(&semaforoBitmap);

	log_debug(log_IMONGO,"<> END: Reparar Bitmap");

}


int analizarBloque(int posicion){


	//log_debug(log_IMONGO,"<> START: Analisis Bloque %d",posicion);

	bool criterioBusqueda(t_bloque* block)
	{
		return block->id == posicion;
	}

	if(list_any_satisfy(bloques,criterioBusqueda))
	{
		return 1;
	}
	else
	{
		return 0;
	}

}

//PROBAR RepararSize

void repararSize(char* recurso)
{

	log_debug(log_IMONGO,"<> START: Reparar Size");

	char* filePath = malloc(70);
	strcpy(filePath,RUTA_FILES);
	string_append_with_format(&filePath,"%s.ims",recurso);

	int contenidoActual;
	int contenidoTotal = 0;

	char* bloquesEnUso = buscarBloquesUsados(filePath);

	char** bloquesTotales = string_split(bloquesEnUso,",");

	int tamanio = contarComas(bloquesEnUso);

	for(int i = 0; i<tamanio; i++)
	{
		contenidoActual = buscarSizeContenido(bloquesTotales[i]);
		contenidoTotal += contenidoActual;
	}

	pthread_mutex_lock(&semaforoFiles);

	t_config* metadata = config_create(filePath);

	config_set_value(metadata,"SIZE",string_itoa(contenidoActual));

	config_save(metadata);

	config_destroy(metadata);

	pthread_mutex_unlock(&semaforoFiles);

	log_debug(log_IMONGO,"<> END: Reparar Size");

	free(filePath);
}


void repararBlockCount(char* recurso)
{

	log_debug(log_IMONGO,"<> START: Reparar BlockCount");

	char* filePath = malloc(70);
	strcpy(filePath,RUTA_FILES);
	string_append_with_format(&filePath,"%s.ims",recurso);

	char* bloquesEnUso = buscarBloquesUsados(filePath);

	int tamanio = contarComas(bloquesEnUso);

	pthread_mutex_lock(&semaforoFiles);

	t_config* metadata = config_create(filePath);

	config_set_value(metadata,"BLOCK_COUNT",string_itoa(tamanio));

	config_save(metadata);

	config_destroy(metadata);

	pthread_mutex_unlock(&semaforoFiles);

	log_debug(log_IMONGO,"<> END: Reparar BlockCount");

	free(filePath);

}

void restaurarRecurso(char* recurso, char caracter)
{

	log_debug(log_IMONGO,"<> START: Restauracion");

	char* filePath = malloc(70);
	strcpy(filePath,RUTA_FILES);
	string_append_with_format(&filePath,"%s.ims",recurso);

	int tamanioContenido = obtenerSizeFile(filePath);

	char* bloquesEnUso = buscarBloquesUsados(filePath);

	char** bloquesTotales = string_split(bloquesEnUso,",");

	int tamanio = contarComas(bloquesEnUso);

	for(int i = 0; i<tamanio; i++)
	{
		borrarContenido(bloquesTotales[i]);
	}

	eliminarFile(filePath);

	log_info(log_IMONGO,"Elimino el file");

	sleep(3);

	generarRecurso(recurso,caracter,tamanioContenido-1);

	log_debug(log_IMONGO,"<> END: Restauracion");



}



void repararMD5(char* recurso,char caracter)
{
	log_debug(log_IMONGO,"<> START: Chequear orden Blocks");

	char* filePath = malloc(70);
	strcpy(filePath,RUTA_FILES);
	string_append_with_format(&filePath,"%s.ims",recurso);

	char* bloquesEnUso = buscarBloquesUsados(filePath);

	char* md5_file = obtenerMD5(filePath);

	char* md5_actual = str2md5(bloquesEnUso,strlen(bloquesEnUso));

	if(strcmp(md5_file,md5_actual)==0)
	{
		log_debug(log_IMONGO,"<> No sabotearon el orden");
	}else
	{
		log_debug(log_IMONGO,"<> Reparando el orden..");
		restaurarRecurso(recurso,caracter);
	}

	log_debug(log_IMONGO,"<> END: Chequear orden Blocks");

	free(filePath);

}

void responder_bitacora(Tripulante* trip)
{

char* id = recibir_id(trip->conexion);

log_info(trip->log,"<> Buscando la bitacora de %s",id);

char* buffer = obtenerBitacora(id);

enviar_mensaje_por_codigo(buffer,BITACORA,trip->conexion);
}


//

void recibir_movimiento(int socket)
{
	char* mensaje = recibir_y_guardar_mensaje(socket);
	char** mensaje_decriptado = string_split(mensaje,",");

	int id = atoi(mensaje_decriptado[0]);
	char* posx = mensaje_decriptado[1];
	char* posy = mensaje_decriptado[2];

	registrarMovimiento(id,posx,posy);

}

void recibir_inicio_tarea(int socket)
{
	char* mensaje = recibir_y_guardar_mensaje(socket);
	char** mensaje_decriptado = string_split(mensaje,",");

	int id = atoi(mensaje_decriptado[0]);
	char* tarea = mensaje_decriptado[1];

	registrarInicioTarea(id,tarea);
}

void recibir_fin_tarea(int socket)
{
	char* mensaje = recibir_y_guardar_mensaje(socket);
	char** mensaje_decriptado = string_split(mensaje,",");

	int id = atoi(mensaje_decriptado[0]);
	char* tarea = mensaje_decriptado[1];

	registrarFinTarea(id,tarea);
}







