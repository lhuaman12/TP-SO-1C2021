#include"blocks.h"

int calcularPosicionBloque(int id_bloque)
{
	return (BLOCK_SIZE + 1)*id_bloque;
}


void guardarEnBloque(char* contenido,int id_bloque)
{
	int tamanio = strlen(contenido);

	pthread_mutex_lock(&semaforoBlock);

	FILE* bloque = fopen(RUTA_BLOCKS,"r+b");
	int posicion = calcularPosicionBloque(id_bloque);
	fseek(bloque,posicion,SEEK_SET);
	fwrite(contenido,tamanio,1,bloque);
	fclose(bloque);

	pthread_mutex_unlock(&semaforoBlock);

}

void cargarBLoqueEnMemoria(char* contenido,int id_bloque)
{
	log_debug(log_IMONGO,"<>START: Guardar en bloque en memoria");
	t_bloque* bloqueNuevo = malloc(sizeof(t_bloque));

	bloqueNuevo->id = id_bloque;
	bloqueNuevo->size = BLOCK_SIZE;
	bloqueNuevo->contenido = malloc(bloqueNuevo->size);
	strcpy(bloqueNuevo->contenido,contenido);
	bloqueNuevo->libre = bloqueNuevo->size - sizeof(contenido);

	list_add(bloques,bloqueNuevo);
	log_debug(log_IMONGO,"<>END: Guardar en bloque en memoria");
}


void copiarAMemoria()
{

	int posicion = 0;
	//char* bloqueVacio = malloc(BLOCK_SIZE);

	char* bloqueVacio = crearBufferInicial(BLOCK_SIZE);

	char buffer[BLOCK_SIZE];
	FILE* metadata = fopen(RUTA_BLOCKS,"r+");

	for(int i = 0; i<BLOCKS;i++)
	{
		posicion = calcularPosicionBloque(i);
		fseek(metadata,posicion,SEEK_SET);
		fread(buffer,BLOCK_SIZE,1,metadata);

		if(strcmp(buffer,bloqueVacio)!=0)
		{
			strcpy(buffer,limpiarContenido(buffer));
			log_debug(log_IMONGO,"<> Se cargo en memoria %d EL %s",i,buffer);
			cargarBLoqueEnMemoria(buffer,i);
			log_debug(log_IMONGO,"<> Se cargo en bitmap %d",i);
			solicitarBloquePorNumero(i);
		}

	}

	fclose(metadata);
	//free(bloqueVacio);
	//free(buffer);

}

void copiarADisco()
{

	log_debug(log_IMONGO,"<> START: Guardar En disco");

	t_bloque* bloque;
	t_list_iterator* iterador =list_iterator_create(bloques);


	pthread_mutex_lock(&semaforoBlock);

	FILE* bloques = fopen(RUTA_BLOCKS,"w+b");
	//char* buffer = malloc(sizeof(BLOCK_SIZE));
	char* buffer = crearBufferInicial(BLOCK_SIZE);
		for(int i = 0; i<BLOCKS; i++)
		{
			fwrite(buffer,BLOCK_SIZE,1,bloques);
			fwrite("-",1,1,bloques);
		}
	free(buffer);
	fclose(bloques);

	pthread_mutex_unlock(&semaforoBlock);

	while(list_iterator_has_next(iterador))
	{
		 bloque = list_iterator_next(iterador);
		 guardarEnBloque(bloque->contenido,bloque->id);
	}

	log_debug(log_IMONGO,"<> END: Guardar En disco");
}

void liberarBloqueMemoria(int id_bloque)
{
	void liberarMemoria(t_bloque* bloque)
	{
		free(bloque->contenido);
		free(bloque);
	}
	bool criterioBusqueda(t_bloque* bloque)
	{
		return bloque->id == id_bloque;
	}
	list_remove_and_destroy_by_condition(bloques, criterioBusqueda, liberarMemoria);
}






int calcularBloquesPorContenido(char* contenido)
{
	int numero = strlen(contenido) / BLOCK_SIZE;
	int resto = strlen(contenido) % BLOCK_SIZE;
	if(resto != 0)
	{
		return numero + 1;
	}
	else
	{
		return numero;
	}
}

int calcularBloquesPorTamanio(int tamanio)
{
	int numero = tamanio / BLOCK_SIZE;
	int resto = tamanio % BLOCK_SIZE;
	if(resto != 0)
	{
		return numero + 1;
	}
	else
	{
		return numero;
	}
}


void sacarCantidad(int id,int cantidad,char* path)
{

	bool criterioNombre(t_bloque* bloque)
	{
		return bloque->id == id;
	}


	if(cantidad == BLOCK_SIZE)
	{
		borrarContenido(string_itoa(id));
		sacarBloque(string_itoa(id),path);

	}else
	{
		t_bloque* bloqueNuevo = malloc(sizeof(t_bloque));
		bloqueNuevo = list_find(bloques,criterioNombre);
		strcpy(bloqueNuevo->contenido,string_substring_until(bloqueNuevo->contenido,BLOCK_SIZE - cantidad));
	}

}



char* limpiarContenido(char* buffer)
{
	int contador = 0;
	char* palabra = malloc(BLOCK_SIZE+1);

	for(int i =0; i<strlen(buffer);i++)
	{
		if(buffer[i]=='X')
		{
			contador++;
		}
	}

	strcpy(palabra,string_substring_until(buffer,strlen(buffer)-contador));

	return palabra;
	free(palabra);
}

int buscarSizeContenido(char* id)
{

	bool criterioNombre(t_bloque* bloque)
				{
					return bloque->id == atoi(id);
				}
	t_bloque* bloqueNuevo = malloc(sizeof(t_bloque));
	bloqueNuevo = list_find(bloques,criterioNombre);

	return strlen(bloqueNuevo->contenido);

}

void borrarContenido(char* id)
{

	bool criterioNombre(t_bloque* bloque)
				{
					return bloque->id == atoi(id);
				}
	list_remove_by_condition(bloques,criterioNombre);
	liberarBloque(atoi(id));

}

int contarComas(char* palabra)
{
	int total = 0;
	for(int i=0; i<strlen(palabra); i++)
	{
		if(palabra[i]==',')
		{
			total++;
		}
	}
	return total;
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
	log_warning(log_IMONGO,"Hay %d bloques",bloques - 1);
	return bloques - 1;
}




