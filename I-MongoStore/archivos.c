#include"archivos.h"

void crearBitacora(char* nombre)
{
	char* filePath = PUNTO_MONTAJE;
	string_append_with_format(&filePath,"/Files/Bitacora/%s.ims",nombre);

	FILE* metadata = fopen(filePath,"w+b");

	char* size = "SIZE=0\n";
	char* blocks = "BLOCKS=[]\n";

	fwrite(size, strlen(size), 1, metadata);
	fwrite(blocks, strlen(blocks), 1, metadata);


	log_info(log_IMONGO, "<> SE CREO LA BITACORA DE: %s.",nombre);
	fclose(metadata);
}

void crearRecurso(char* filePath, char* recurso){
	FILE* metadata = fopen(filePath,"w+b");
	char* size = "SIZE=\n";
	char* block_count = "BLOCK_COUNT=\n";
	char* blocks = "BLOCKS=\n";
	char* caracter_llenado = "CARACTER_LLENADO=\n";
	char* md5 = "MD5_ARCHIVO=\n";
	fwrite(size, strlen(size), 1, metadata);
	fwrite(block_count, strlen(block_count), 1, metadata);
	fwrite(blocks, strlen(blocks), 1, metadata);
	fwrite(caracter_llenado, strlen(caracter_llenado), 1, metadata);
	fwrite(md5,strlen(md5),1,metadata);

	log_info(log_IMONGO, "<> SE CREO EL RECURSO %s.",recurso);
	fclose(metadata);
}

/*
char* obtenerBitacora(char* id)
{
	char* filePath = PUNTO_MONTAJE;
	string_append_with_format(&filePath,"/Files/Bitacora/Tripulante%s.ims",id);

	t_config* bitacora = config_create(filePath);
	char* bloques[] = config_get_array_value(bitacora,"BLOCKS");
	int tamanio = sizeof(bloques)/sizeof(bloques[0]);

	char* mensaje = string_new();
	char* mensaje_local;

	for(int i=0;i<tamanio;i++)
	{
		mensaje_local = leerBloque();
		string_append()
	}

	return mensaje;

}
*/

/*
void crearArchivoMetadata(char* filePath,char* tamanio,char* cant_bloques,char* bloques, char* caracter){
	char* carpeta = string_substring_until(filePath, strlen(filePath) - strlen("Metadata.bin"));
	log_debug(log_IMONGO, "Se procede a crear la carpeta del metadata: %s", carpeta);
	mkdir(carpeta, S_IRWXU);

	FILE* metadata = fopen(filePath,"w+");

	char* size = string_new();
	string_append_with_format(&size,"SIZE=%s\n",tamanio);

	char* block_count = string_new();
	string_append_with_format(&size,"BLOCK_COUNT=%s\n",cant_bloques);

	char* blocks = string_new();
	string_append_with_format(&size,"BLOCKS=%s\n",bloques);

	char* caracter_llenado = string_new();
	string_append_with_format(&size,"CARACTER_LLENADO=%s\n",caracter);

	char* md5 = "MD5_ARCHIVO=0\n";

	log_debug(log_IMONGO, "Se procede a escribir los atributos iniciales");
	fwrite(size, strlen(size), 1, metadata);
	fwrite(block_count, strlen(block_count), 1, metadata);
	fwrite(blocks, strlen(blocks), 1, metadata);
	fwrite(caracter_llenado, strlen(caracter_llenado), 1, metadata);
	fwrite(md5,strlen(md5),1,metadata);

	log_info(log_IMONGO, "Se creo el metadata del recurso.");
	fclose(metadata);
}
*/
void crearArchivoMetadata(char* filePath,char* tamanio,char* cant_bloques,char* bloques, char* caracter){

	FILE* metadata = fopen(filePath,"wb+");

	char* size = string_new();
	string_append_with_format(&size,"SIZE=%s\n",tamanio);

	char* block_count = string_new();
	string_append_with_format(&size,"BLOCK_COUNT=%s\n",cant_bloques);

	char* blocks = string_new();
	string_append_with_format(&size,"BLOCKS=[%s]\n",bloques);

	char* caracter_llenado = string_new();
	string_append_with_format(&size,"CARACTER_LLENADO=%s\n",caracter);

	char* md5 = "MD5_ARCHIVO=0\n";

	log_debug(log_IMONGO, "Se procede a escribir los atributos iniciales");
	fwrite(size, strlen(size), 1, metadata);
	fwrite(block_count, strlen(block_count), 1, metadata);
	fwrite(blocks, strlen(blocks), 1, metadata);
	fwrite(caracter_llenado, strlen(caracter_llenado), 1, metadata);
	fwrite(md5,strlen(md5),1,metadata);

	log_info(log_IMONGO, "Se creo el metadata del recurso.");
	fclose(metadata);
}

void sobreEscribirSuperBloque(char* path,int bloques)
{
	FILE* metadata = fopen(path,"r+");

	char* contenido = string_itoa(bloques);
	string_append(&contenido,"\n");
	fseek(metadata,7,SEEK_SET);
	fwrite(contenido,strlen(contenido),1,metadata);
	fclose(metadata);

}
void agregarBloquesFile(char* path,char* bloquesUsados)
{
	log_debug(log_IMONGO,"Se inicia la agregacion insta");
	t_config* recurso = config_create(path);
	char* bloquesitos = config_get_string_value(recurso,"BLOCKS");
	string_append(&bloquesitos,bloquesUsados);

	log_info(log_IMONGO,"SON_: %s",bloquesitos);

	config_set_value(recurso,"BLOCKS",bloquesitos);
}


void sobreEscribirRecurso(char* path,int bloques)
{
	FILE* metadata = fopen(path,"r+");
	char* contenido = string_itoa(bloques);
	fseek(metadata,5,SEEK_SET);
	fwrite(contenido,strlen(contenido),1,metadata);
	fclose(metadata);

}


