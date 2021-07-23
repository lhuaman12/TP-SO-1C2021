
#include "interfaz.h"


void generarOxigeno(int cantidad)
{
	generarRecurso("Oxigeno",'O',cantidad);
}
void consumirOxigeno(int cantidad)
{
	consumirRecurso("Oxigeno",cantidad);
}
void generarComida(int cantidad)
{
	generarRecurso("Comida",'C',cantidad);
}
void consumirComida(int cantidad)
{
	consumirRecurso("Comida",cantidad);
}
void generarBasura(int cantidad)
{
	generarRecurso("Basura",'B',cantidad);
}
void consumirBasura(int cantidad)
{
	consumirRecurso("Basura",cantidad);
}
void registrarMovimiento(int id,char* posX_Final,char* posY_Final)
{
	char* palabra = malloc(50);
	strcpy(palabra,"Se mueve ");
	string_append_with_format(&palabra,"a %s|%s",posX_Final,posY_Final);
	guardarBitacora(string_itoa(id),palabra);
	free(palabra);
}

void registrarInicioTarea(int id,char* tarea)
{
	char* palabra = malloc(70);
	strcpy(palabra,"Comienza ejecucion de tarea ");
	string_append(&palabra,tarea);
	guardarBitacora(string_itoa(id),palabra);
	free(palabra);
}
void registrarFinTarea(int id,char* tarea)
{
	char* palabra = malloc(70);
	strcpy(palabra,"Fin ejecucion de tarea ");
	string_append(&palabra,tarea);
	guardarBitacora(string_itoa(id),palabra);
	free(palabra);
}


void registrarFinSabotaje(int id)
{
	char* palabra = malloc(30);
	strcpy(palabra,"Se resuelve el sabotaje");

	guardarBitacora(string_itoa(id),palabra);
	free(palabra);
}

void registrarEnCamino(int id)
{
	char* palabra = malloc(60);
	strcpy(palabra,"Se corre en panico hacia la ubicacion del sabotaje");

	guardarBitacora(string_itoa(id),palabra);
	free(palabra);

}


void guardarBitacora(char* id_tripulante,char* mensaje)
{
	char* filePath = malloc(70);
	strcpy(filePath,RUTA_BITACORA);

	string_append_with_format(&filePath,"Tripulante%s.ims",id_tripulante);

	if(existeArchivo(filePath))
	{
		guardarContenidoBitacora(filePath,mensaje);
	}
	else{
		crearBitacora(filePath);
		guardarContenidoBitacora(filePath,mensaje);
	}

	free(filePath);

}


char* obtenerBitacora(char* id)
{

	char* path = malloc(70);
	strcpy(path,RUTA_BITACORA);
	string_append_with_format(&path,"Tripulante%s.ims",id);

	char* lista_mensajes = malloc(900);
	strcpy(lista_mensajes,"");

	char* buffer = malloc(BLOCK_SIZE);

	//t_bitacora* nuevaBitacora = malloc(sizeof(t_bitacora));

	char* bloquesEnUso = buscarBloquesUsados(path);
	//1,2,3,4,5,

	char** bloquesTotales = string_split(bloquesEnUso,",");
	//{"1","2","3"..}
	int i = 0;

	int tamanio = contarComas(bloquesEnUso);

	for(i = 0; i<tamanio; i++)
	{

		buffer = sacarContenidoBloque(atoi(bloquesTotales[i]));
		string_append(&lista_mensajes,buffer);
	}

	log_info(log_IMONGO,"LISTA: %s",lista_mensajes);


	return lista_mensajes;

	free(lista_mensajes);
	free(buffer);
	free(path);
}

char* sacarContenidoBloque(int id)
{
	bool criterioNombre(t_bloque* bloque)
			{
				return bloque->id == id;
			}
	t_bloque* bloqueNuevo = malloc(sizeof(t_bloque));
	bloqueNuevo = list_find(bloques,criterioNombre);

	//LIMPIAR bloqueNuevo->contenido;

	return bloqueNuevo->contenido;
}


bool existeArchivo(char* path)
{
	return access(path,F_OK) == 0;
}


void generarRecurso(char* nombre, char caracter,int cantidad)
{
	char* filePath = malloc(70);
	strcpy(filePath,RUTA_FILES);
	string_append_with_format(&filePath,"%s.ims",nombre);


	if(existeArchivo(filePath))
	{
		char* buffer = malloc(cantidad);
		buffer = string_repeat(caracter,cantidad);
		guardarContenido(filePath,buffer);
		//free(buffer);

	}
	else{
		crearRecurso(filePath,nombre);
		char* buffer = malloc(cantidad+1);
		buffer = string_repeat(caracter,cantidad+1);
		//agregarCaracterFile(filePath,caracter);
		guardarContenido(filePath,buffer);
		//free(buffer);
	}
	free(filePath);
}

void guardarContenido(char* path, char* contenido)
{
	log_debug(log_IMONGO,"<> ARRANCA A GUARDAR");

	int cantidadBloques = calcularBloquesPorContenido(contenido);
	char* bloquesGuardados = string_new();
	int i,offset=0;
	char* contenidoCortado[cantidadBloques];

	for(i=0;i<cantidadBloques;i++){

		contenidoCortado[i]=malloc(BLOCK_SIZE);
		strncpy(contenidoCortado[i],contenido+offset,BLOCK_SIZE);
		contenidoCortado[i] = string_substring_until(contenidoCortado[i],BLOCK_SIZE);
		offset+=BLOCK_SIZE;
	}
	int bloque = -1;
	for(int i = 0; i<cantidadBloques;i++)
	{
		bloque = solicitarBloque();
		if(bloque == -1)
		{
			log_error(log_IMONGO,"<> NO HAY SUFICIENTES BLOQUES PARA GUARDAR ESO");
		}
		cargarBLoqueEnMemoria(contenidoCortado[i],bloque);
		string_append_with_format(&bloquesGuardados,"%d,",bloque);
		free(contenidoCortado[i]);
	}

	char* bloquesT = agregarBloquesFile(path,bloquesGuardados);
	char* md5 = str2md5(bloquesT,strlen(bloquesT));
	crearMD5(path,md5);
	agregarBlockCountFile(path,cantidadBloques);
	agregarTamanioFile(path,strlen(contenido));
	free(bloquesGuardados);
}
/*
void guardarContenido(char* path, char* contenido)
{
	log_debug(log_IMONGO,"<> ARRANCA A GUARDAR");
	int cantidadBloques = calcularBloquesPorContenido(contenido);
	char* bloquesGuardados = string_new();
	int i,offset=0;
	char* contenidoCortado[cantidadBloques];

	for(i=0;i<cantidadBloques;i++){

		contenidoCortado[i]=malloc(BLOCK_SIZE);
		strncpy(contenidoCortado[i],contenido+offset,BLOCK_SIZE);
		contenidoCortado[i] = string_substring_until(contenidoCortado[i],BLOCK_SIZE);
		offset+=BLOCK_SIZE;
	}
	int bloque = -1;
	for(int i = 0; i<cantidadBloques;i++)
	{
		bloque = solicitarBloque();
		if(bloque == -1)
		{
			log_error(log_IMONGO,"<> NO HAY SUFICIENTES BLOQUES PARA GUARDAR ESO");
		}
		cargarBLoqueEnMemoria(contenidoCortado[i],bloque);
		string_append_with_format(&bloquesGuardados,"%d,",bloque);
		free(contenidoCortado[i]);
	}

	agregarBloquesFile(path,bloquesGuardados);
	agregarBlockCountFile(path,cantidadBloques);
	agregarTamanioFile(path,strlen(contenido));
	free(bloquesGuardados);
}
*/



void guardarContenidoBitacora(char* path, char* contenido)
{
	log_debug(log_IMONGO,"<> ARRANCA A GUARDAR BITACORA");
	int cantidadBloques = calcularBloquesPorContenido(contenido);
	char* bloquesGuardados = string_new();
	int i,offset=0;
	char* contenidoCortado[cantidadBloques];

	for(i=0;i<cantidadBloques;i++){

		contenidoCortado[i]=malloc(BLOCK_SIZE);
		strncpy(contenidoCortado[i],contenido+offset,BLOCK_SIZE);
		contenidoCortado[i] = string_substring_until(contenidoCortado[i],BLOCK_SIZE);
		offset+=BLOCK_SIZE;
	}
	int bloque = -1;
	for(int i = 0; i<cantidadBloques;i++)
	{
		bloque = solicitarBloque();
		if(bloque == -1)
		{
			log_error(log_IMONGO,"<> NO HAY SUFICIENTES BLOQUES PARA GUARDAR ESO");
		}
		cargarBLoqueEnMemoria(contenidoCortado[i],bloque);
		string_append_with_format(&bloquesGuardados,"%d,",bloque);
		free(contenidoCortado[i]);
	}

	agregarBloquesFile(path,bloquesGuardados);
	agregarTamanioFile(path,strlen(contenido));
	free(bloquesGuardados);
}



void consumirRecurso(char* recurso,int cantidad)
{
	char* path = malloc(70);
	strcpy(path,RUTA_FILES);
	string_append_with_format(&path,"%s.ims",recurso);

	int cantidadActual = cantidad;

	int bloquesABorrar = calcularBloquesPorTamanio(cantidad);

	char* bloquesEnUso = buscarBloquesUsados(path);

	char** bloquesTotales = string_split(bloquesEnUso,",");

	int cantidades[bloquesABorrar];

	if(bloquesABorrar > 1)
	{
		for(int j = 0;j<bloquesABorrar-1;j++)
		{
		cantidades[j] = BLOCK_SIZE;
		}
		cantidades[bloquesABorrar-1]=cantidad%BLOCK_SIZE;

	}else
	{
		cantidades[bloquesABorrar-1]=cantidad%BLOCK_SIZE;
	}

	for(int i = 0; i<bloquesABorrar;i++)
	{
		sacarCantidad(atoi(bloquesTotales[i]),cantidades[i],path);
	}

	char* bloquesActualizados = buscarBloquesUsados(path);

	decrementarTamanioFile(path,cantidad);

	free(path);

}





