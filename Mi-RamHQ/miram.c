/*
 * servidor.c
 *
 *  Created on: 23 jul. 2021
 *      Author: utnso
 */

#include"miram.h"

int main(void){

	cargar_configuracion();

	logger = log_create("miRam.log", "Mi-RAM",1, LOG_LEVEL_DEBUG);

	//signal(SIGUSR1, &signalCompactacion);
	signal(SIGUSR1, &signalDump);

	int feedback = iniciarMemoria();

	if(!feedback){
		return 0;
	}


	//crearMapaVacio();

	prender_server();

	/*
	while (1)
			{

				sleep(1);

				char* leido = readline(">");
				char** split = string_split(leido, " ");
				if (string_equals_ignore_case(split[0], "exit"))
				{
					free(leido);
					//liberarStringArray(split);
					break;
				}else if(string_equals_ignore_case(split[0],"showS"))
				{
					showbitarr(bitMapSegment);
				}else if(string_equals_ignore_case(split[0],"showMP"))
				{
					showbitarr(frames_ocupados_ppal);
				}else if(string_equals_ignore_case(split[0],"showMV"))
				{
					showbitarr(frames_ocupados_virtual);
				}else if(string_equals_ignore_case(split[0],"tripulantesSeg"))
				{
					void mostrarTripulantes(t_tabla_tripulante* unaTabla)
					{
						log_info(logger,"En el segmento es %d esta: \n", unaTabla->seg_tcb->id);
						//mostrarTripulante(unaTabla->idTripulante);
					}

					list_iterate(tablaDeSegmentosDeTripulantes, (void*)mostrarTripulantes);

				}else if(string_equals_ignore_case(split[0],"tripulantesPag"))
				{
					void mostrarT(t_tabla_pagina* unaTabla)
					{
						mostrarListaTripulantesPag(unaTabla->idTripus);
					}

					list_iterate(tablaDePaginas, (void*) mostrarT);

				}else if(string_equals_ignore_case(split[0],"patotas"))
				{
					void mostrarPatotas(t_tabla_patota* unaTabla)
						{
							printf("La pcb de la patota %d esta entre %d y  %d \n", unaTabla->idPatota, unaTabla->pcb->base,(unaTabla->pcb->base + unaTabla->pcb->limite) );
							printf("Las tareas de la patota %d esta entre %d y  %d \n", unaTabla->idPatota, unaTabla->tareas->base,(unaTabla->tareas->base + unaTabla->tareas->limite) );


						}
					//pthread_mutex_lock(&mutexListaDePatotas);
					list_iterate(tablaDeSegmentosDePatotas, (void*)mostrarPatotas);
					//pthread_mutex_unlock(&mutexListaDePatotas);

				}else if(string_equals_ignore_case(split[0],"dump"))
				{
					dump();
				}else if(string_equals_ignore_case(split[0],"compactar"))
				{
					compactacion();
				}else if(string_equals_ignore_case(split[0],"tablas"))
				{
					void mostrarPaginas(t_tabla_pagina* unaTabla)
					{
						list_iterate(unaTabla->paginas, (void*)mostrarPagina);
					}
					list_iterate(tablaDePaginas, (void*)mostrarPaginas);
				}
				free(leido);
				liberarStringArray(split);
			}
			*/

	liberarMemoria();
	//liberar_conexion(server_fd);
	//liberarConfig();
	return EXIT_SUCCESS;

}


void prender_server()
{

	int puerto_escucha = PUERTO_MIRAM;
	SOCKET_MIRAM = crearSocket();
	log_info(logger,"<> SERVIDOR LISTO....");
	asignar_escuchas(SOCKET_MIRAM,puerto_escucha,atender_tripulante);

	// SE ESCUCHA AL MISMO TIEMPO VARIOS CLIENTES, PARA PODER RECIBIR
	// DISTINTOS TIPOS DE MEMSAJE, SE AGREGAN LA FUNCION ENVIAR MENSAJE Y RECIBIR MENSAJE EN RESPECTIVOS DOCUMENTOS
}

void* atender_tripulante(Tripulante* trip)
{


	while(1)
		{
		int cod_op = recibir_operacion(trip->conexion);
			switch(cod_op)
			{
				case ENVIAR_PROXIMA_TAREA:
					enviarOperacion(trip->conexion);
					break;
				case POSICION_TRIPULANTE_ACTUALIZADA:
					actualizarUbicacion(trip->conexion);
					break;
				case INICIAR_PATOTAS:
					iniciarPatota(trip->conexion);
					break;
				case EXPULSAR_TRIPULANTES:
					expulsarTripulante(trip->conexion);
					break;
				case CAMBIO_ESTADO:
					cambiarEstado(trip->conexion);
					break;
			    case MENSAJE:
					//recibir_y_guardar_mensaje(trip->conexion);
					//enviar_mensaje_por_codigo("CHAU",MENSAJE,trip->conexion);
					break;
				case -1:
					//log_error(trip->log, "El cliente se desconecto. Terminando servidor");
					break;

			}

		}


}

void cargar_configuracion(void)
{
	miRam_config = config_create("../miRam.config"); //Leo el archivo de configuracion

	if (miRam_config == NULL) {
		perror("Archivo de configuracion de MI-RAM no encontrado");
		exit(EXIT_FAILURE);
	}

	PUERTO_MIRAM =  config_get_int_value(miRam_config,"PUERTO");
	IP_MIRAM = config_get_string_value(miRam_config,"IP");
	TAM_MEM = config_get_int_value(miRam_config,"TAMANIO_MEMORIA");
	ESQUEMA_MEM =config_get_string_value(miRam_config, "ESQUEMA_MEMORIA");
	TAM_PAG = config_get_int_value(miRam_config,"TAMANIO_PAGINA");
	TAM_SWAP =config_get_int_value(miRam_config, "TAMANIO_SWAP");
	PATH_SWAP= config_get_string_value(miRam_config,"PATH_SWAP");
	ALGORITMO_VM = config_get_string_value(miRam_config,"ALGORITMO_REEMPLAZO");
	CRITERIO_SELECCION = config_get_string_value(miRam_config,"CRITERIO_SELECCION");


}



void signalCompactacion(int sig){
	log_info(logger, "Recibi la señial de compactar, compactando...");
	//compactacion();
}
void signalDump(int sig){
	log_info(logger,"Recibi la señal de hacer Dump, creando dump..");
	dump();
}


void dump(){

	if(string_equals_ignore_case(ESQUEMA_MEM, "SEGMENTACION")){
		dumpSegmentacion();
	}else if(string_equals_ignore_case(ESQUEMA_MEM, "PAGINACION")){
		dumpPaginacion();
	}
}
