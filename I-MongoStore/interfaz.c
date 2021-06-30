
#include "interfaz.h"


char** cortarPalabras(char* palabra,int cantidad)
{
	//log_info(log_IMONGO,"ARRANCO A CORTAR LA PALABRA");
		char** palabras_respuesta;
		int cant_palabras_cortadas= strlen(palabra)/cantidad;
		int sobran_palabras= strlen(palabra)%cantidad;
		int i,offset=0;

		if(sobran_palabras)
			cant_palabras_cortadas++;
			palabras_respuesta=malloc((cant_palabras_cortadas+1)*sizeof(char*)); // uno mas para reservar el null al final
		for(i=0;i<cant_palabras_cortadas;i++){
			palabras_respuesta[i]=malloc(cantidad+1); // uno mas para el cero
			strncpy(palabras_respuesta[i],palabra+offset,cantidad);
			offset+=cantidad;
		}
		return palabras_respuesta;

}

bool existeArchivo(char* path)
{
	return access(path,F_OK) == 0;
}


void generarOxigeno(int cantidad)
{
	char* nombre = "Oxigeno";
	char* filePath = "/home/utnso/polus/Files/Oxigeno.ims";
	if(existeArchivo(filePath))
	{
		char* buffer = malloc(cantidad);
		buffer = string_repeat('O',cantidad);
		guardarContenido(filePath,buffer);

	}
	else{
		crearRecurso(filePath,nombre);
		char* buffer = malloc(cantidad+1);
		buffer = string_repeat('O',cantidad+1);
		guardarContenido(filePath,buffer);
	}
}


void guardarRecurso(char* recurso,char* contenido)
{
	//crearRecurso(recurso);
	//guardarContenido(filePath,contenido);
	//free(filePath);
}

void guardarContenido(char* path, char* contenido)
{
	log_debug(log_IMONGO,"<> ARRANCA A GUARDAR");
	int cantidadBloques = calcularBloquesPorContenido(contenido);
	char* bloquesGuardados = string_new();
	char** contendidoCortado = cortarPalabras(contenido,BLOCK_SIZE);
	int bloque = -1;
	for(int i = 0; i<cantidadBloques;i++)
	{
		bloque = solicitarBloque();
		if(bloque == -1)
		{
			log_error(log_IMONGO,"<> NO HAY SUFICIENTES BLOQUES PARA GUARDAR ESO");
		}
		cargarBLoqueEnMemoria(contendidoCortado[i],bloque);
		string_append_with_format(&bloquesGuardados,"%d,",bloque);
	}

	//agregarBloquesFile(path,bloquesGuardados);
	free(bloquesGuardados);
	free(contendidoCortado);
}


