#include"archivos.h"

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

void crearArchivo(char* filePath){
	char* carpetaPokemon = string_substring_until(filePath, strlen(filePath) - strlen("Metadata.bin"));
	log_debug(logger, "Se procede a crear la carpeta del metadata: %s", carpetaPokemon);

	mkdir(carpetaPokemon, S_IRWXU);

	FILE* metadata = fopen(filePath,"w+");

	char* directory = "DIRECTORY=N\n";
	char* size = "SIZE=0\n";
	char* blocks = "BLOCKS=[]\n";
	char* open = "OPEN=Y";

	log_debug(logger, "Se procede a escribir los atributos iniciales");
	fwrite(directory, strlen(directory), 1, metadata);
	fwrite(size, strlen(size), 1, metadata);
	fwrite(blocks, strlen(blocks), 1, metadata);
	fwrite(open, strlen(open), 1, metadata);

	log_info(logger, "Se creo el metadata del pokemon.");
	free(carpetaPokemon);
	fclose(metadata);
}

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