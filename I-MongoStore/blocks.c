#include"blocks.h"

int calcularPosicionBloque(int id_bloque)
{
	return (BLOCK_SIZE + 1)*id_bloque;
}


void guardarEnBloque(char* contenido,int id_bloque)
{
	log_debug(log_IMONGO,"<>START: Guardar en bloque");
	int tamanio = strlen(contenido);
	FILE* bloque = fopen(RUTA_BLOCKS,"r+b");
	int posicion = calcularPosicionBloque(id_bloque);
	fseek(bloque,posicion,SEEK_SET);
	fwrite(contenido,tamanio,1,bloque);
	fclose(bloque);
	log_debug(log_IMONGO,"<>END: Guardar en bloque");
}

int calcularBloquesPorContenido(char* contenido)
{
	return strlen(contenido) / BLOCK_SIZE;
}

int contarBloques()
{
	log_debug(log_IMONGO,"<>START: Contar bloques");
	FILE* bloque = fopen(RUTA_BLOCKS,"r+b");
	int bloques = 0;
	char valor;
	while(!feof(bloque))
	{
		fread(&valor,sizeof(char),1,bloque);
		if(valor == '-')
		{
			bloques++;
		}
	}
	log_debug(log_IMONGO,"<>END: Contar bloques");
	return bloques - 1;
}




