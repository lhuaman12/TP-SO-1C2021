#include"archivos.h"


void crearBitacora(char* path)
{
	char* size = "SIZE=0\n";
	char* blocks = "BLOCKS=\n";

	pthread_mutex_lock(&semaforoFiles);

	FILE* metadata = fopen(path,"w+b");
	fwrite(size, strlen(size), 1, metadata);
	fwrite(blocks, strlen(blocks), 1, metadata);
	fclose(metadata);

	pthread_mutex_unlock(&semaforoFiles);

	log_info(log_IMONGO, "<> SE CREO LA BITACORA");
}

void crearRecurso(char* filePath, char* recurso){

	FILE* metadata = fopen(filePath,"w+b");
	char* size = "SIZE=\n";
	char* block_count = "BLOCK_COUNT=\n";
	char* blocks = "BLOCKS=\n";
	char* caracter_llenado = "CARACTER_LLENADO=\n";
	char* md5 = "MD5_ARCHIVO=\n";
	fwrite(size, strlen(size), 1, metadata);
	fwrite(block_count, strlen(block_count), 1, metadata);
	fwrite(blocks, strlen(blocks), 1, metadata);
	fwrite(caracter_llenado, strlen(caracter_llenado), 1, metadata);
	fwrite(md5,strlen(md5),1,metadata);

	log_info(log_IMONGO, "<> SE CREO EL RECURSO %s.",recurso);
	fclose(metadata);
}


void sobreEscribirSuperBloque(char* path,int bloques)
{

	char* contenido = string_itoa(bloques);
	string_append(&contenido,"\n");

	pthread_mutex_lock(&semaforoSuperBloque);

	t_config* recurso = config_create(path);

	config_set_value(recurso,"BLOCKS",string_itoa(bloques));

	config_save(recurso);

	config_destroy(recurso);

	pthread_mutex_unlock(&semaforoSuperBloque);

}


void eliminarFile(char* path)
{
	pthread_mutex_lock(&semaforoFiles);

	remove(path);

	pthread_mutex_unlock(&semaforoFiles);
}

int obtenerSizeFile(char* path)
{
	t_config* recurso = config_create(path);

	char* bloquesitos = config_get_int_value(recurso,"SIZE");

	return bloquesitos;

	config_destroy(recurso);

}

char* buscarBloquesUsados(char* path)
{
	t_config* recurso = config_create(path);

	char* bloquesitos = config_get_string_value(recurso,"BLOCKS");

	return bloquesitos;

	config_destroy(recurso);

}

char* obtenerMD5(char* path)
{
	t_config* recurso = config_create(path);

	char* md5 = config_get_string_value(recurso,"MD5_ARCHIVO");

	return md5;

	config_destroy(recurso);

}
void reemplazarBloquesFile(char* path,char* bloquesUsados)
{

	pthread_mutex_lock(&semaforoFiles);

	t_config* recurso = config_create(path);

	config_set_value(recurso,"BLOCKS",bloquesUsados);

	config_save(recurso);

	config_destroy(recurso);

	pthread_mutex_unlock(&semaforoFiles);

}

char* agregarBloquesFile(char* path,char* bloquesUsados)
{
	char* bloquesT = string_new();

	pthread_mutex_lock(&semaforoFiles);

	t_config* recurso = config_create(path);

	char* bloquesitos = config_get_string_value(recurso,"BLOCKS");

	if(string_is_empty(bloquesitos))
	{
		string_append(&bloquesT,bloquesUsados);

	}else{

	string_append(&bloquesT,bloquesitos);

	string_append(&bloquesT,bloquesUsados);

	}

	config_set_value(recurso,"BLOCKS",bloquesT);

	config_save(recurso);

	config_destroy(recurso);

	pthread_mutex_unlock(&semaforoFiles);

	return bloquesT;

	free(bloquesT);
}

void crearMD5(char* filePath,char* palabra)
{
	pthread_mutex_lock(&semaforoFiles);

	t_config* recurso = config_create(filePath);

	config_set_value(recurso,"MD5_ARCHIVO",palabra);

	config_save(recurso);

	config_destroy(recurso);

	pthread_mutex_unlock(&semaforoFiles);

}



void agregarTamanioFile(char* path,int tamanio)
{
	//SEMAFORO RECURSO;
	pthread_mutex_lock(&semaforoFiles);

	t_config* recurso = config_create(path);

	int tamanio_actual = config_get_int_value(recurso,"SIZE");

	tamanio_actual += tamanio;

	config_set_value(recurso,"SIZE",string_itoa(tamanio_actual));

	config_save(recurso);

	config_destroy(recurso);

	pthread_mutex_unlock(&semaforoFiles);
}



void sacarBloque(char* id,char* path)
{
	char* bloquesEnUso = buscarBloquesUsados(path);
	char* palabra = malloc(strlen(bloquesEnUso));
	strcpy(palabra,"\0");
	char* palabra_momentanea = malloc(2);
	strcpy(palabra_momentanea,"\0");

	int i = 0;
	int j= 0;
	while(i < strlen(bloquesEnUso))
	{
		palabra_momentanea[0] = bloquesEnUso[i];
		if(palabra_momentanea[0] != id[0])
		{
			strncat(palabra,palabra_momentanea,1);
			i++;
			j++;
		}else
		{
			i += 2;
		}
	}
	reemplazarBloquesFile(path,palabra);
	char* md5 = str2md5(palabra,strlen(palabra));
	crearMD5(path,md5);
	free(palabra_momentanea);
	free(palabra);
}

void decrementarTamanioFile(char* path,int tamanio)
{
	//SEMAFORO RECURSO;
	pthread_mutex_lock(&semaforoFiles);

	t_config* recurso = config_create(path);

	int tamanio_actual = config_get_int_value(recurso,"SIZE");

	tamanio_actual -= tamanio;

	if(tamanio_actual < 0)
	{
		tamanio_actual=0;
	}

	config_set_value(recurso,"SIZE",string_itoa(tamanio_actual));

	config_save(recurso);

	config_destroy(recurso);

	pthread_mutex_unlock(&semaforoFiles);
}
void agregarBlockCountFile(char* path,int tamanio)
{
	//SEMAFORO RECURSO;
	pthread_mutex_lock(&semaforoFiles);

	t_config* recurso = config_create(path);

	int tamanio_actual = config_get_int_value(recurso,"BLOCK_COUNT");

	tamanio_actual += tamanio;

	config_set_value(recurso,"BLOCK_COUNT",string_itoa(tamanio_actual));

	config_save(recurso);

	config_destroy(recurso);

	pthread_mutex_unlock(&semaforoFiles);

}
void agregarCaracterFile(char* path,char caracter)
{

	pthread_mutex_lock(&semaforoFiles);

	t_config* recurso = config_create(path);

	char* palabra = malloc(2);
	palabra[0]=caracter;

	config_set_value(recurso,"CARACTER_LLENADO",palabra);

	config_save(recurso);

	config_destroy(recurso);

	pthread_mutex_unlock(&semaforoFiles);
	free(palabra);
}




