
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
		palabras_respuesta[i+1]=NULL;
		return palabras_respuesta;

}


void guardarContenido(char* contenido)
{
	char* path = "/home/utnso/polus/Files/Oxigeno.ims";
	int cantidadBloques = calcularBloquesPorContenido(contenido);
	char* bloquesGuardados = string_new();
	char** contendidoCortado = cortarPalabras(contenido,BLOCK_SIZE);
	for(int i = 0; i<cantidadBloques;i++)
	{
		int bloque = solicitarBloque();
		guardarEnBloque(contendidoCortado[i],bloque);
		string_append_with_format(&bloquesGuardados,"%s,",bloque);
	}
	//crearArchivoMetadata(path,string_itoa(strlen(contenido)),string_itoa(cantidadBloques),bloquesGuardados,"O");
}


