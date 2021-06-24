#include"archivos.h"
/*
int checkingOpenFile(char* filePath){

	t_config* configFile = config_create(filePath);
	char* openFile = config_get_string_value(configFile, "OPEN");
	int retorno;

	if (strcmp("Y", openFile) == 0){ //ARCHIVO ABIERTO
		retorno = 1;
	}
	else{ //ARCHIVO CERRADO
		retorno = 0;
	}

	config_destroy(configFile);
	return retorno;
}

void cambiarAAbierto(char* filePath){
	log_debug(logger, "Antes de crear config en cambiarAAbierto");
	log_debug(logger, "path de config: %s", filePath);
	t_config* metadataArchivoAAbrir = config_create(filePath);
	log_debug(logger, "Despues de crear config en cambiarAAbierto");
	config_set_value(metadataArchivoAAbrir, "OPEN", "Y");
	config_save(metadataArchivoAAbrir);
	config_destroy(metadataArchivoAAbrir);
}

void cambiarACerrado(char* filePath){
	log_debug(logger, "Antes de crear config en cambiarACerrado");
	log_debug(logger, "path de config: %s", filePath);
	t_config* metadataArchivoACerrar = config_create(filePath);
	log_debug(logger, "Despues de crear config en cambiarACerrado");
	config_set_value(metadataArchivoACerrar, "OPEN", "N");
	config_save(metadataArchivoACerrar);
	config_destroy(metadataArchivoACerrar);
}
*/
void crearArchivo(char* filePath){
	char* carpeta = string_substring_until(filePath, strlen(filePath) - strlen("Metadata.bin"));
	log_debug(log_IMONGO, "Se procede a crear la carpeta del metadata: %s", carpeta);

	mkdir(carpeta, S_IRWXU);

	FILE* metadata = fopen(filePath,"w+");

	char* size = "SIZE=0\n";
	char* block_count = "BLOCK_COUNT=0\n";
	char* blocks = "BLOCKS=[]\n";
	char* caracter_llenado = "CARACTER_LLENADO=N\n";
	char* md5 = "MD5_ARCHIVO=0\n";

	log_debug(log_IMONGO, "Se procede a escribir los atributos iniciales");
	fwrite(size, strlen(size), 1, metadata);
	fwrite(block_count, strlen(block_count), 1, metadata);
	fwrite(blocks, strlen(blocks), 1, metadata);
	fwrite(caracter_llenado, strlen(caracter_llenado), 1, metadata);
	fwrite(md5,strlen(md5),1,metadata);

	log_info(log_IMONGO, "Se creo el metadata del pokemon.");
	free(carpeta);
	fclose(metadata);
}
/*
void crearArchivoMetadata(char* filePath,char* tamanio,char* cant_bloques,char* bloques, char* caracter){
	char* carpeta = string_substring_until(filePath, strlen(filePath) - strlen("Metadata.bin"));
	log_debug(log_IMONGO, "Se procede a crear la carpeta del metadata: %s", carpeta);
	mkdir(carpeta, S_IRWXU);

	FILE* metadata = fopen(filePath,"w+");

	char* size = string_new();
	string_append_with_format(&size,"SIZE=%s\n",tamanio);

	char* block_count = string_new();
	string_append_with_format(&size,"BLOCK_COUNT=%s\n",cant_bloques);

	char* blocks = string_new();
	string_append_with_format(&size,"BLOCKS=%s\n",bloques);

	char* caracter_llenado = string_new();
	string_append_with_format(&size,"CARACTER_LLENADO=%s\n",caracter);

	char* md5 = "MD5_ARCHIVO=0\n";

	log_debug(log_IMONGO, "Se procede a escribir los atributos iniciales");
	fwrite(size, strlen(size), 1, metadata);
	fwrite(block_count, strlen(block_count), 1, metadata);
	fwrite(blocks, strlen(blocks), 1, metadata);
	fwrite(caracter_llenado, strlen(caracter_llenado), 1, metadata);
	fwrite(md5,strlen(md5),1,metadata);

	log_info(log_IMONGO, "Se creo el metadata del recurso.");
	fclose(metadata);
}
*/
void crearArchivoMetadata(char* filePath,char* tamanio,char* cant_bloques,char* bloques, char* caracter){

	FILE* metadata = fopen(filePath,"wb+");

	char* size = string_new();
	string_append_with_format(&size,"SIZE=%s\n",tamanio);

	char* block_count = string_new();
	string_append_with_format(&size,"BLOCK_COUNT=%s\n",cant_bloques);

	char* blocks = string_new();
	string_append_with_format(&size,"BLOCKS=[%s]\n",bloques);

	char* caracter_llenado = string_new();
	string_append_with_format(&size,"CARACTER_LLENADO=%s\n",caracter);

	char* md5 = "MD5_ARCHIVO=0\n";

	log_debug(log_IMONGO, "Se procede a escribir los atributos iniciales");
	fwrite(size, strlen(size), 1, metadata);
	fwrite(block_count, strlen(block_count), 1, metadata);
	fwrite(blocks, strlen(blocks), 1, metadata);
	fwrite(caracter_llenado, strlen(caracter_llenado), 1, metadata);
	fwrite(md5,strlen(md5),1,metadata);

	log_info(log_IMONGO, "Se creo el metadata del recurso.");
	fclose(metadata);
}

void sobreEscribirSuperBloque(char* path,int bloques)
{
	FILE* metadata = fopen(path,"r+");
	//fseek(metadata,7,SEEK_SET);
	//fwrite("      ",6,1,metadata);
	char* contenido = string_itoa(bloques);
	string_append(&contenido,"\n");
	fseek(metadata,7,SEEK_SET);
	fwrite(contenido,strlen(contenido),1,metadata);
	fclose(metadata);

}




/*
int checkArchivoExiste(char* filePath){

	log_debug(logger, "Chequeando si existe el archivo %s", filePath);
	if(access(filePath, F_OK) != -1 ) { // El archivo existe. Se prosigue a verificar si está en uso. DONE
		log_debug(logger, "Archivo existe.");
		return 1;
	}
	else{
		log_debug(logger, "Archivo NO existe.");
		return 0;
	}
}

void pedirArchivoParaUso(char* filePath){

	if(access(filePath, F_OK) != -1 ) { // El archivo existe. Se prosigue a verificar si está en uso. DONE
		log_debug(logger, "Archivo existe.");
		int abierto = 1;

		while (abierto){
			pthread_mutex_lock(&semaforoOpen); // Impido que dos hilos accedan. DONE
			abierto = checkingOpenFile(filePath);
			if (!abierto){ // Está cerrado -> abro y le cambio el OPEN = Y. DONE
				cambiarAAbierto(filePath);
				log_debug(logger, "Se logró abrir el archivo.");
				pthread_mutex_unlock(&semaforoOpen);
			}
			else{ //Si está abierto que el hilo duerma. DONE
				pthread_mutex_unlock(&semaforoOpen);
				log_debug(logger, "Archivo abierto. Comienzo a dormir por %d segundos para luego reintentar.", TIEMPO_DE_REINTENTO_OPERACION);
				sleep(TIEMPO_DE_REINTENTO_OPERACION);
			}
		}
	}

	else { //	En caso de no existir se deberá crear. DONE
		log_debug(logger, "Archivo no existe. Se procede a crearlo.");
		crearArchivo(filePath);
	}
}
*/

