/*
 * interfaz.c
 *
 *  Created on: 15 jun. 2021
 *      Author: utnso
 */
/*

void guardarRecursos(char* recurso, int cantidad,char* nombre)
{
	char* path = string_from_format("%s/Files/Bitacora/%s/Metadata.bin",PUNTO_MONTAJE,nombre);
	crearArchivo(path);

	t_config* metadata = config_create(path);

	config_set_value(metadata,"CARACTER_LLENADO",recurso);

	int peso = obtenerBytes(cantidad);

	config_set_value(metadata,"SIZE",peso);

	char* contenido = string_repeat(recurso,cantidad);

	char** bloquesTotales;

	guardarDatosEnBlocks(contenido,bloquesTotales);


}
*/
int obtenerBytes(int cantidad)
{
	return cantidad * 8;
}
