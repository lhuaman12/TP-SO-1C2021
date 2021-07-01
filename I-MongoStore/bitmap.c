#include"bitmap.h"

int solicitarBloque() {
	log_debug(log_IMONGO, "<> START: Solicitar bloque <>");

	int bloqueEncontrado = -1;
	int cantidadBloques = bitarray_get_max_bit(bitarray);
	int i = 0;
	for (i = 0; i < cantidadBloques; i++) {
		if (!bitarray_test_bit(bitarray, i)) {
			bitarray_set_bit(bitarray, i);
			bloqueEncontrado = i;
			break;
		}
	}

	if(bloqueEncontrado == -1){
		log_debug(log_IMONGO, "No se han encontrado bloques libres.");
	}

	else{
		log_info(log_IMONGO, "Bloque libre encontrado: [%d]", bloqueEncontrado);

		FILE* bitmap = fopen(RUTA_BITMAP, "r+w");
		fseek(bitmap, bloqueEncontrado, SEEK_SET);
		fwrite("1", 1, 1, bitmap);
		fclose(bitmap);
	}

	//guardarBackup(bloquesUsados,bloqueEncontrado);

	log_debug(log_IMONGO, "<> END: Solicitar bloque <>");

	return bloqueEncontrado;
}

void solicitarBloquePorNumero(int id) {
	log_debug(log_IMONGO, "<> START: Solicitar bloque %d <>",id);

	int bloqueEncontrado = id;

	bitarray_set_bit(bitarray, bloqueEncontrado);

	FILE* bitmap = fopen(RUTA_BITMAP, "r+w");
	fseek(bitmap, bloqueEncontrado, SEEK_SET);
	fwrite("1", 1, 1, bitmap);
	fclose(bitmap);

	log_debug(log_IMONGO, "<> END: Solicitar bloque %d <>",id);
}

void liberarBloque(int bloqueALiberar){
	log_debug(log_IMONGO, "<> START: Liberar bloque <>");

	bitarray_clean_bit(bitarray, bloqueALiberar);

	FILE* bitmap = fopen(RUTA_BITMAP, "r+w");
	fseek(bitmap, bloqueALiberar, SEEK_SET);
	fwrite("0", 1, 1, bitmap);
	fclose(bitmap);

	log_debug(log_IMONGO, "<> END: Liberar bloque <>");
}

void guardarBackup(t_list* bitList,int bloque)
{
	bool criterio(int numero)
	{
		return numero == bloque;
	}
	if(list_find(bitList,criterio) == NULL)
	{
		log_warning(log_IMONGO,"SE GUARDO %d",bloque);
		list_add(bitList,bloque);
	}
	else
	{
		log_warning(log_IMONGO,"YA ESTA OCUPADO ESE BIT");
	}
}

void estaEnBitmap(int bloque)
{
	char valor;
	FILE* bitmap = fopen(RUTA_BITMAP, "r+w");
	fseek(bitmap, bloque - 1, SEEK_SET);
	fread(&valor,sizeof(char),1,bitmap);
	if(valor != '1')
	{
		log_debug(log_IMONGO,"<> SE ENCONTRO DISCREPANCIA EN %d, REPARANDO...",bloque);
		fwrite("1", 1, 1, bitmap);
	}
	fclose(bitmap);
}




