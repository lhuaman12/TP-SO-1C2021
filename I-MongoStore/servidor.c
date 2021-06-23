#include "servidor.h"


int main(void)
{
	iniciar_log();
	leer_config();
	iniciar_filesystem();
	//prender_server();
	//log_info(log_IMONGO,"gola");


	/*CUANDDO HAGO UN LOG SE ROMPE O SE OCUPA LA MEMORIA, Y NO ME ANDA BIEN ESTA FUNCION :c*/

	//char** palabra_cortada = cortarPalabras("holacomo",4);
	//printf("PALABRA: %s\n",palabra_cortada[0]);
	//printf("PALABRA: %s\n",palabra_cortada[1]);
}

void prender_server()
{
	int puerto_escucha = atoi(PUERTO_ESCUCHA_SERVER);
	int socket_interno = crearSocket();
	log_info(log_IMONGO,"SERVIDOR LISTO");
	asignar_escuchas(socket_interno,puerto_escucha,atender_tripulante);
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

void* atender_tripulante(Tripulante* trip)
{
	while(1)
		{
		int cod_op = recibir_operacion(trip->conexion);
						switch(cod_op)
						{
					    case MENSAJE:
							recibir_mensaje_encriptado(trip->conexion,trip->log);
							break;
						case -1:
							log_error(trip->log, "El cliente se desconecto. Terminando servidor");
							break;
						default:
							//log_warning(trip->log, "Operacion desconocida. No quieras meter la pata");
							break;
						}
		}
}
void expulsar_un_tripulante(Tripulante* trip)
{
	char* id = recibir_id(trip->conexion);
	log_debug(trip->log,"SE EXPULSO EL TRIPULANTE %s",id);
}

void init_directorios()
{
	mkdir(RUTA_FILES,S_IRWXU);
	mkdir(RUTA_BITACORA,S_IRWXU);
}


char* crearBufferInicial(int numero)
{
	return string_repeat('X',numero);
}


void init_bloques()
{
	log_debug(log_IMONGO,"<> START: Creacion de bloques");
	FILE* bloques = fopen(RUTA_BLOCKS,"wb");

	char* buffer = crearBufferInicial(BLOCK_SIZE);
	for(int i = 0; i<BLOCKS; i++)
	{
		fwrite(buffer,BLOCK_SIZE,1,bloques);
		fwrite(" ",1,1,bloques);
	}
	fclose(bloques);
}

void iniciar_log()
{
	log_IMONGO = log_create("imongo.log", "I-MONGO-STORE", 1, LOG_LEVEL_INFO);
}

void leer_config()
{
	config_IMONGO = config_create("../imongo.config");

	PUNTO_MONTAJE = config_get_string_value(config_IMONGO,"PUNTO_MONTAJE");
	RUTA_BITMAP = string_from_format("%s/Metadata/Bitmap.bin",PUNTO_MONTAJE);
	RUTA_SUPER_BLOQUE = string_from_format("%s/SuperBloque.ims",PUNTO_MONTAJE);
	RUTA_FILES = string_from_format("%s/Files/",PUNTO_MONTAJE);
	RUTA_BLOCKS = string_from_format("%s/Blocks.ims",PUNTO_MONTAJE);
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



