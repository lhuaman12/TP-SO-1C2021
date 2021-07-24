
#include "conections.h"


void conectar_envio(int codigoDeConexion,char* ip,int PUERTO_PARA_ENVIAR)
{
	if(conectarA(codigoDeConexion,ip,PUERTO_PARA_ENVIAR))
			{
				//log_info(logg,"SE CONECTO UN CLIENTE");
			}else
			{
				//log_info(logg,"ERROR AL CONECTAR");
			}
}

int escuchar_puerto(int conexion_server,int puerto, t_log* logger)
{
	int conexion_escucha;
			if(escuchaEn(conexion_server,puerto))
			{
				log_info(logger,"TRIPULANTE ESCUCHANDO");

				while(1)
				{
					conexion_escucha = aceptarConexion(conexion_server);
					if(conexion_escucha != -1)
					{
							log_info(logger,"<> CONEXION ACEPTADA");
							return conexion_escucha;
					}

				}

			}else
			{
				log_info(logger,"ERROR: NO ESCUCHA");
				return -1;
			}
}


void asignar_escuchas(int conexion_server,int puerto, void* atender(Tripulante* trip))
{
		escuchaEn(conexion_server,puerto);
		while(1)
					{
						aceptar_tripulante(conexion_server,atender);
					}
}



void recibir_mensaje_encriptado(int cliente_fd,t_log* logg)
{
	char* mensaje;
	char** mensaje_decriptado;

	mensaje = recibir_y_guardar_mensaje(cliente_fd);

	mensaje_decriptado = string_split(mensaje,",");


	int hora = atoi(mensaje_decriptado[0]);
	char* lugar = mensaje_decriptado[1];
	int cant_participantes = atoi(mensaje_decriptado[2]);

	hora += 18;

	//"2" -> 2
	//STRING -> INT

	log_info(logg,"La hora es %d", hora);
	log_info(logg,"La ubicacion es %s", lugar);
	log_info(logg,"La cantidad de tripulantes es %d", cant_participantes);


}





void aceptar_tripulante(int conexion, void* atender(Tripulante* trip))
{
	Tripulante* nuevo_tripulante = crearTripulante();
	nuevo_tripulante->conexion = aceptarConexion(conexion);
	if(nuevo_tripulante->conexion != -1)
	{
		//log_info(nuevo_tripulante->log,"NUEVO TRIPULANTE");
		pthread_create(&(nuevo_tripulante->hilo),NULL,(void*)atender,nuevo_tripulante);
		pthread_detach(nuevo_tripulante->hilo);
	}
}


void chat(int conexion)
{
	char* leido;
	leido = readline(">");

	while(strcmp(leido,"\0") != 0) {
			enviar_mensaje(leido,conexion);
			free(leido);
			leido = readline(">");
		}
	enviar_mensaje(leido,conexion);
	free(leido);

}


void terminar_programa(int conexion,t_config* unConfig,t_log* unLogger)
{
	liberar_conexion(conexion);
	log_destroy(unLogger);
	config_destroy(unConfig);
}




















