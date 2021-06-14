#include "servidor.h"


int main(void)
{


	iniciar_log();
	leer_config();
	iniciar_filesystem();
	prender_server();


	//crearArchivo("/home/utnso/polus/Files/Tripulante/Metadata.bin");

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
	//leer_metadata();
	//init_semaforos();
	init_bitmap();
	//init_estructura_file_blocks();
	//init_blocks();

}

void init_bitmap()
{
	log_info(log_IMONGO,"<> INICIO CREACION BITMAP");
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

	log_info(log_IMONGO,"<> FIN CREACION BITMAP");

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
	BLOCKS = 8;
	BLOCK_SIZE = 130;


}

