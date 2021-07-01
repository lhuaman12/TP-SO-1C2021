#include "servidor.h"


int main(void)
{
	iniciar_log();
	leer_config();
	iniciar_filesystem();

	generarOxigeno(7);
	//generarOxigeno(2);

	copiarADisco();
	//prender_server();
	//log_info(log_IMONGO,"GOLA");

	//char palabra_cortada = cortarPalabras("holaCOMOandas",4);
	//log_info(log_IMONGO,"Palabra: %s",palabra_cortada[0]);
	//log_info(log_IMONGO,"Palabra: %s",palabra_cortada[1]);
}

void prender_server()
{
	int puerto_escucha = atoi(PUERTO_ESCUCHA_SERVER);
	int socket_interno = crearSocket();
	log_info(log_IMONGO,"SERVIDOR LISTO");
	//pthread_create(&hiloSignal,NULL,comunicarDiscordiador,NULL);
	//pthread_detach(hiloSignal);
	asignar_escuchas(socket_interno,puerto_escucha,atender_tripulante);
}


void iniciar_filesystem()
{
	char* buffer;
	printf("\n<> Desea crear un nuevo FILE SYSTEM?");
	buffer = readline(">");

	if(strcmp(buffer,"SI")==0)
	{
		crear_super_bloque();
		leer_super_bloque();
		init_directorios();
		//init_semaforos();
		init_bitmap();
		init_bloques_vacio();

	}else
	{
		leer_super_bloque();
		init_directorios();
		//init_semaforos();
		init_bitmap();
		init_bloques_usado();
	}


}

void init_bitmap()
{
	bloquesUsados = list_create();

	log_debug(log_IMONGO,"<> INICIO CREACION BITMAP");

	FILE* bitmapFile = fopen(RUTA_BITMAP,"w+b");

	int cantidad_bits = BLOCKS;

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

	//free(bufferBitArray);

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
					    case OBTENER_BITACORA:
					    	responder_bitacora(trip);
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


void init_directorios()
{
	mkdir(RUTA_FILES,S_IRWXU);
	mkdir(RUTA_BITACORA,S_IRWXU);
}


char* crearBufferInicial(uint32_t numero)
{
	return string_repeat('X',numero);
}
void init_bloques_vacio()
{
	log_debug(log_IMONGO,"<> START: Creacion de bloques");

	bloques = list_create();

	copiarADisco();

	log_debug(log_IMONGO,"<>END: Creacion de bloques");

}

void init_bloques_usado()
{
	log_debug(log_IMONGO,"<> START: Lectura de bloques");

	bloques = list_create();

	FILE* bloques = fopen(RUTA_BLOCKS,"r+b");

	fclose(bloques);

	log_warning(log_IMONGO,"Hay %d bloques",BLOCKS);

	copiarAMemoria();

	log_debug(log_IMONGO,"<>END: Lectura de bloques");
}

void iniciar_log()
{
	log_IMONGO = log_create("imongo.log", "I-MONGO-STORE", 1, LOG_LEVEL_DEBUG);
}

void leer_config()
{
	config_IMONGO = config_create("./imongo.config");
	//PUNTO_MONTAJE = config_get_string_value(config_IMONGO,"PUNTO_MONTAJE");
	PUNTO_MONTAJE = "/home/utnso/polus";
	RUTA_BITMAP = string_from_format("%s/Bitmap.ims",PUNTO_MONTAJE);
	RUTA_SUPER_BLOQUE = string_from_format("%s/SuperBloque.ims",PUNTO_MONTAJE);
	RUTA_FILES = string_from_format("%s/Files/",PUNTO_MONTAJE);
	RUTA_BLOCKS = string_from_format("%s/Blocks.ims",PUNTO_MONTAJE);
	RUTA_BITACORA = string_from_format("%s/Files/Bitacora/",PUNTO_MONTAJE);
	NUEVO_FS = 0;
}

void crear_super_bloque()
{
	char blocks[] = "BLOCKS=\n";
	char block_size[]= "BLOCK_SIZE=\n";
	char bitmap[] = "BITMAP=\n";

	FILE* super_bloque = fopen(RUTA_SUPER_BLOQUE,"w+b");
	fwrite(blocks, strlen(blocks), 1, super_bloque);
	fwrite(block_size, strlen(block_size), 1, super_bloque);
	fwrite(bitmap, strlen(bitmap), 1, super_bloque);
	fclose(super_bloque);

	escribir_super_bloque();
}

void escribir_super_bloque()
{
	char* bloques;
	char* tamanio;

	printf("\n<> Ingrese la cantidad de bloques\n");
	bloques = readline(">");
	printf("\n<> Ingrese el tamaño de los bloques\n");
	tamanio = readline(">");


	super_config = config_create(RUTA_SUPER_BLOQUE);
	config_set_value(super_config,"BLOCKS",bloques);
	config_set_value(super_config,"BLOCK_SIZE",tamanio);

	config_save(super_config);
	config_destroy(super_config);

/*
	FILE* super_bloque= fopen(RUTA_SUPER_BLOQUE,"r+b");
	fseek(super_bloque,7,SEEK_SET);
	fwrite(bloques,strlen(bloques),1,super_bloque);
	fwrite("\n",sizeof(char),1,super_bloque);
	fclose(super_bloque);
*/
}

void leer_super_bloque()
{
	super_config = config_create(RUTA_SUPER_BLOQUE);
	BLOCKS = config_get_int_value(super_config,"BLOCKS");
	BLOCK_SIZE = config_get_int_value(super_config,"BLOCK_SIZE");
	//BITMAP = config_get_string_value(super_config,"BITMAP");
	config_destroy(super_config);

}



