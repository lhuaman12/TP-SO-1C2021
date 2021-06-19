
#ifndef UTILS_CONSOLA_H_
#define UTILS_CONSOLA_H_

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>
#include <stdlib.h>
#include <commons/string.h>
#include "./estructuras.h"
#include "discordiador.h"
#include <commons/collections/list.h>


void consola_discordiador();
void liberar_recursos(char **input);
void ejecutar_comando(char** lectura);
void iniciar_patota(char ** argumentos);
void nueva_patota(char *cantidad_tripulantes,char** posiciones);
t_tripulante* crear_tripulante(int32_t pid, char* posicion);
t_posicion* obtener_posicion(char* posicion);
char** buscar_tareas(char* path);
int validar_entrada(char **palabras_separadas);
void listar_tripulantes();
void imprimir_estado_patota(t_patota* patota);
//void ejecutar_iniciar_patota(t_patota* patota,int socket);


void expulsar_tripulante(char* id_tripulante);
void actualizar_posiciones_tripulante(char *id_tripulante,char* posicionX,char* posicionY);
void enviar_tarea_a_tripulante(char *id_tripulante);

#endif /* UTILS_CONSOLA_H_ */
