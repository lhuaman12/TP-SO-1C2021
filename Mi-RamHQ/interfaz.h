/*
 * interfaz.h
 *
 *  Created on: 23 jul. 2021
 *      Author: utnso
 */

#ifndef INTERFAZ_H_
#define INTERFAZ_H_

#include "estructurasMemoria.h"
#include "estructuras.h"
#include <math.h>

int elegirEsquema();
/////////////////////////////////////////////////////////////////////////////////////////////////////INICIAR PATOTA
void iniciarPatota(int socket_cliente);

int s_iniciarPatota(int cantTripus, int idPatota, char* tareas, int tamanioTotal);

int p_iniciarPatota(int cantTripus, int idPatota, char* tareas, int tamanioTotal);

////////////////////////////////////////////////////////////////////////////////////////INICIAR TRIPULANTE
void iniciarTripulante(int tid,int pid,int posx, int posy);

void s_iniciarTripulante(t_tcb* tripulanteNuevo,int idPato);

void p_iniciarTripulante(t_tcb* tripulanteNuevo, int idPatota);
/////////////////////////////////////////////////////////////////////////////EXPULSAR TRIPULANTE

void expulsarTripulante(int socket_cliente);

void s_expulsarTripulante(int idTripu);

void sacarTripulanteListaGlobal(int idTripulanteAEliminar);

char intToChar(int numero);

/////////////////////////////////////////////////////////////////////////////////ACTUALIZAR UBICACION
void actualizarUbicacion(int socket_cliente);

/////////////////////////////////////////////////////////////////////////////////TOTO ES UN CAPO

t_tcb* buscarTcb(int tid);
void actualizarTripulante(t_tcb* tcb);
char* buscarTareaEsquema(t_tcb* tcb,int pid);
void expulsarTripulanteID(int tid);

//////////////////////////////////////////////////////////////////////////////////////////ENVIAR OPERACION

void enviarOperacion(int socket_cliente);

char* s_enviarOp(t_tcb* tcb);

char* p_enviarOp(t_tcb* tcb,int idPatota);

//t_paquete* armarPaquete(char* tarea, int esLaUltima);

///////////////////////////////////////////////////////////////////////////////////////////////////CAMBIAR ESTADO
void cambiarEstado(int socket_cliente);


#endif /* INTERFAZ_H_ */
