

#include "Mi-RamHQ.h"

int main(){

	iniciar_logger();
	iniciar_config();
	reservar_memoria();
	iniciar_escucha_a_tripulantes();
//	iniciar_mapa("Nivel Base",4,4);

}

void iniciar_logger()
{
	miRam_logger=log_create("miRam.log", "Mi-Ram", 1, LOG_LEVEL_INFO);
	log_info(miRam_logger,"Ya me inicie");

}



void iniciar_config()
{
	miRam_config = config_create("/home/utnso/tp-2021-1c-bastardosSinGloria/Mi-RamHQ/miRam.config");
}




void reservar_memoria()
{
 ram=malloc(config_get_int_value(miRam_config,"TAMANIO_MEMORIA_RAM"));
 log_info(miRam_logger, "Reserve el espacio para la memoria principal");
}



int iniciar_mapa(char* nombreNivel,int columnas,int filas)
{
	NIVEL* nivel;

	nivel_gui_inicializar();

	nivel_gui_get_area_nivel(&columnas, &filas);

	nivel = nivel_crear(nombreNivel);

	while ( 1 ) {
			nivel_gui_dibujar(nivel);

			int key = getch();

			switch( key ) {


				case 'Q':
				case 'q':
					nivel_destruir(nivel);
					nivel_gui_terminar();
					return EXIT_SUCCESS;
				break;
			}

	}


}


int iniciar_escucha_a_tripulantes()
{
	int conexion_escucha;

	int puerto_escucha = atoi(config_get_string_value(miRam_config,"PUERTO_ESCUCHA"));
	miRam_logger = log_create("log.log", "Servidor Mi-Ram", 1, LOG_LEVEL_DEBUG);
	int conexion_server = crearSocket();
	log_info(miRam_logger,"MI-RAM LISTA PARA RECIBIR");
	conexion_escucha = escuchar_puerto(conexion_server,puerto_escucha);

		if(conexion_escucha == -1)
		{
			log_info(miRam_logger,"ERROR: LA CONEXION ES -1");
		}

		while(1)
		{
			int cod_op = recibir_operacion(conexion_escucha);
			switch(cod_op)
			{

			case MENSAJE:
				recibir_mensaje_encriptado(conexion_escucha,miRam_logger);
				break;

			case -1:
				log_error(miRam_logger, "El cliente se desconecto. Terminando servidor");
				return EXIT_FAILURE;

			default:
				log_warning(miRam_logger, "Operacion desconocida. No quieras meter la pata");
				break;
			}
		}
		return EXIT_SUCCESS;

}





