/*
 * interfaz.h
 *
 *  Created on: 15 jun. 2021
 *      Author: utnso
 */

#ifndef INTERFAZ_H_
#define INTERFAZ_H_

#include<string.h>

#include"servidor.h"

typedef struct
{
	int size;
	char* bloquesUsados;
}t_bitacora;



//INTERFAZ DE MENSAJES
void generarOxigeno(int cantidad);
void consumirOxigeno(int cantidad);

void generarComida(int cantidad);
void consumirComida(int cantidad);

void generarBasura(int cantidad);
void consumirBasura(int cantidad);

char* obtenerBitacora(char* id);

void registrarMovimiento(int id,char* posX_Final,char* posY_Final);
void registrarInicioTarea(int id,char* tarea);
void registrarFinTarea(int id,char* tarea);
void registrarFinSabotaje(int id);
void registrarEnCamino(int id);

// FUNCIONES AUXILIARES

char cortarPalabras(char* palabra,int cantidad);
void guardarContenidoBitacora(char* path, char* contenido);
void guardarContenido(char* path, char* contenido);
bool existeArchivo(char* nombre);
char* sacarContenidoBloque(int id);
void consumirRecurso(char* recurso,int cantidad);
void guardarBitacora(char* id_tripulante,char* mensaje);
void generarRecurso(char* nombre, char caracter,int cantidad);

#endif /* INTERFAZ_H_ */
