#include"bitmap.h"

int solicitarBloque() {
	log_debug(logger, "<> START: Solicitar bloque <>");

	int bloqueEncontrado = -1;
	int cantidadBloques = bitarray_get_max_bit(bitarray);

	for (int i = 0; i < cantidadBloques; i++) {
		if (!bitarray_test_bit(bitarray, i)) {
			bitarray_set_bit(bitarray, i);
			bloqueEncontrado = i;
			break;
		}
	}

	if(bloqueEncontrado == -1){
		log_debug(logger, "No se han encontrado bloques libres.");
	}

	else{
		log_info(logger, "Bloque libre encontrado: [%d]", bloqueEncontrado);

		FILE* bitmap = fopen(PATH_BITMAP, "r+w");
		fseek(bitmap, bloqueEncontrado, SEEK_SET);
		fwrite("1", 1, 1, bitmap);
		fclose(bitmap);
	}

	log_debug(logger, "<> END: Solicitar bloque <>");

	return bloqueEncontrado;
}

void liberarBloque(int bloqueALiberar){
	log_debug(logger, "<> START: Liberar bloque <>");

	bitarray_clean_bit(bitarray, bloqueALiberar);

	FILE* bitmap = fopen(PATH_BITMAP, "r+w");
	fseek(bitmap, bloqueALiberar, SEEK_SET);
	fwrite("0", 1, 1, bitmap);
	fclose(bitmap);

	log_debug(logger, "<> END: Liberar bloque <>");
}