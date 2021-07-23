
#ifndef CONECTIONS_H_
#define CONECTIONS_H_

//normales
#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<string.h>

//commons
#include<commons/string.h>
#include<commons/log.h>
#include<commons/config.h>
#include<readline/readline.h>

//shared
#include "utils_servidor.h"
#include "utils_cliente.h"
#include "estructurasCliente-Servidor.h"
#include "conexiones.h"

void conectar_envio(int codigoDeConexion,char* ip,int PUERTO_PARA_ENVIAR);

int escuchar_puerto(int conexion_server,int puerto,t_log* logger);

void recibir_mensaje_encriptado(int cliente_fd,t_log* logg);

void asignar_escuchas(int conexion_server,int puerto, void* atender(Tripulante* trip));

void aceptar_tripulante(int conexion, void* atender(Tripulante* trip));

void* leer_mensajes(int socket_interno);

void chat(int);

void terminar_programa(int,t_config*,t_log*);

void recibir_pcb(int cliente,t_log* logg);












#endif /* CONECTIONS_H_ */
