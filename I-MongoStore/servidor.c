#include "servidor.h"


int main(void)
{


	iniciar_log();
	leer_config();
	iniciar_filesystem();
	prender_server();

}

void prender_server()
{
	int puerto_escucha = atoi(PUERTO_ESCUCHA_SERVER);
	int socket_interno = crearSocket();
	log_info(log_IMONGO,"SERVIDOR LISTO");
	asignar_escuchas(socket_interno,puerto_escucha);
}

void iniciar_filesystem()
{
	leer_super_bloque();
	//init_semaforos();
	init_bitmap();
	init_directorios();
	init_bloques();

}

void init_bitmap()
{
	log_debug(log_IMONGO,"<> INICIO CREACION BITMAP");
	FILE* bitmapFile = fopen(RUTA_BITMAP,"w+");

	int cantidad_bits = BLOCKS / 8;

	for(int i = 0; i<BLOCKS; i++)
	{
		fwrite("0",1,1, bitmapFile);
	}
	fclose(bitmapFile);

	bufferBitArray = malloc(cantidad_bits);
	bitarray = bitarray_create_with_mode(bufferBitArray, cantidad_bits, LSB_FIRST);

	for(int i = 0; i<BLOCKS;i++)
	{
		bitarray_clean_bit(bitarray,i);
	}

	log_debug(log_IMONGO,"<> FIN CREACION BITMAP");

}



void init_directorios()
{
	mkdir(RUTA_FILES,S_IRWXU);
	mkdir(RUTA_BLOCKS,S_IRWXU);
	mkdir(RUTA_BITACORA,S_IRWXU);
}

void init_bloques()
{
	log_debug(log_IMONGO,"<> START: Creacion de bloques");
	for(int i = 0; i < BLOCKS; i++)
	{
		char* path = string_new();
		string_append(&path, RUTA_BLOCKS);

		char* numeroEnString = string_itoa(i);
		string_append(&path, numeroEnString);
		string_append(&path, ".bin");

		FILE* bloqueACrear = fopen(path,"wb");
		fclose(bloqueACrear);

		free(numeroEnString);
		free(path);
	}
	log_debug(log_IMONGO,"<> FIN: Creacion de bloques, se crearon %d",BLOCKS);
}

void iniciar_log()
{
	log_IMONGO = log_create("log.log", "I-MONGO-STORE", 1, LOG_LEVEL_DEBUG);
}

void leer_config(void)
{
	config_IMONGO = config_create("../imongo.config");


	PUNTO_MONTAJE = config_get_string_value(config_IMONGO,"PUNTO_MONTAJE");
	RUTA_BITMAP = string_from_format("%s/Metadata/Bitmap.bin",PUNTO_MONTAJE);
	RUTA_SUPER_BLOQUE = string_from_format("%s/SuperBloque.ims",PUNTO_MONTAJE);
	RUTA_FILES = string_from_format("%s/Files/",PUNTO_MONTAJE);
	RUTA_BLOCKS = string_from_format("%s/Blocks/",PUNTO_MONTAJE);
	RUTA_BITACORA = string_from_format("%s/Files/Bitacora/",PUNTO_MONTAJE);

}

void leer_super_bloque()
{
	super_config = config_create(RUTA_SUPER_BLOQUE);
	BLOCKS = config_get_int_value(super_config,"BLOCKS");
	BLOCK_SIZE = config_get_int_value(super_config,"BLOCK_SIZE");
	config_destroy(super_config);
	log_info(log_IMONGO,"<> TERMINO DE LEER SUPER BLOQUE");

}


