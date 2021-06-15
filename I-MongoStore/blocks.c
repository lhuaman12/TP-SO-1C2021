#include"blocks.h"

int getBlockQuantity(char** arrayDeBlocks) {
	int i;
	for (i = 0; arrayDeBlocks[i] != NULL; i++);
	return i;
}
/*
char* getDatosDeBlocks(char** arrayDeBlocks, int size) {
	log_debug(logger, "<> START: getDatosDeBlocks | Size del archivo: %d <>",
			size);
	int blockQuantity = getBlockQuantity(arrayDeBlocks);
	log_debug(logger, "Cantidad de bloques a concatenar: %d", blockQuantity);
	char* contenidoDeArchivo = malloc(size);
	int offset = 0;

	int sizeRestante = size;

	for (int i = 0; i < blockQuantity; i++) {

		char* path = getPathDeBlock(atoi(arrayDeBlocks[i]));
		log_debug(logger, "Abriendo el block %s", path);
		FILE* blockActual = fopen(path, "rb+");

		void* contenidoDeBloqueActual;

		/* Lo que hay que leer es menor a tamaño de bloque

		if (sizeRestante < BLOCK_SIZE) {
			log_debug(logger, "Debemos leer %d bytes del bloque", sizeRestante);
			contenidoDeBloqueActual = malloc(sizeRestante);
			fread(contenidoDeBloqueActual, 1, sizeRestante, blockActual);
			fclose(blockActual);

			memcpy(contenidoDeArchivo + offset, contenidoDeBloqueActual,
					sizeRestante);
		}

		/* Lo que hay que leer es mayor a un bloque
		else {
			log_debug(logger, "Debemos leer %d bytes del bloque", BLOCK_SIZE);
			contenidoDeBloqueActual = malloc(BLOCK_SIZE);
			fread(contenidoDeBloqueActual, 1, BLOCK_SIZE, blockActual);
			fclose(blockActual);

			memcpy(contenidoDeArchivo + offset, contenidoDeBloqueActual,
					BLOCK_SIZE);

			sizeRestante -= BLOCK_SIZE;
			offset += BLOCK_SIZE;
		}

//		log_debug(logger, "Contenido del bloque actual: %s",contenidoDeBloqueActual);

		free(contenidoDeBloqueActual);
		free(path);
	}

//	log_debug(logger, "Contenido total del archivo:");
//	log_debug(logger, contenidoDeArchivo);

	log_debug(logger, "<> END: getDatosDeBlocks <>");

	return contenidoDeArchivo;
}
*/
void guardarDatosEnBlocks(char* contenidoNuevo, char** arrayDeBlocks) {
	int bytesGuardados = 0;
	int cantidadDeBloques = getBlockQuantity(arrayDeBlocks);

	for (int i = 0; i < cantidadDeBloques; i++) {
		log_debug(logger, "Guardando datos en bloque n° %s.", arrayDeBlocks[i]);

		int cuantoEscribir = BLOCK_SIZE - (strlen(contenidoNuevo) - bytesGuardados);

		char* datosAGuardar;

		if (cuantoEscribir > 0) {
			datosAGuardar = string_substring(contenidoNuevo, bytesGuardados, strlen(contenidoNuevo) - bytesGuardados);
		} else {
			datosAGuardar = string_substring(contenidoNuevo, bytesGuardados, BLOCK_SIZE);
		}

		log_debug(logger, "Bytes a guardar: %d", strlen(datosAGuardar));

		char* path = getPathDeBlock(atoi(arrayDeBlocks[i]));
		FILE* bloqueACrear = fopen(path, "wb+");
		fwrite(datosAGuardar, strlen(datosAGuardar), 1, bloqueACrear);
		fclose(bloqueACrear);

		free(datosAGuardar);
		free(path);

		bytesGuardados += BLOCK_SIZE;
	}
}

char* getPathDeBlock(int bloque) {
	char* path = string_new();
	string_append(&path, RUTA_BLOCKS);
	char* numeroEnString = string_itoa(bloque);
	string_append(&path, numeroEnString);
	string_append(&path, ".bin");

	free(numeroEnString);
	return path;
}

