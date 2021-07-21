
#ifndef UTILS_CONSOLA_H_
#define UTILS_CONSOLA_H_

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>
#include <stdlib.h>
#include <commons/string.h>
#include <commons/collections/list.h>
#include "./estructuras.h"
#include "discordiador.h"
#include <commons/collections/queue.h>
#include <unistd.h>
#include <time.h>

// para pausar el planificador
pthread_mutex_t mutex_pausa;
pthread_cond_t planificacion_pausa;
int levantar_hilo_planificacion; //comandos
int pausado; // esto solo para comandos
int planificador_pausado; //esto va en el hilo
/////
typedef struct{
	int hay_sabotaje;
	t_posicion* posicion;
}t_sabotaje;

t_sabotaje* sabotaje;

///
char** normalizar_tarea(char* tarea);
void submodulo_tripulante(t_tripulante* tripulante);
void consola_discordiador();
void liberar_recursos(char **input);
void ejecutar_comando(char** lectura);
void iniciar_patota(char ** argumentos);
void nueva_patota(char *cantidad_tripulantes,char** posiciones,char* tareas);
char* unir_tripulantes(char* cant_tripulantes,char** posiciones);
t_tripulante* crear_tripulante(int32_t pid, char* posicion);
t_posicion* obtener_posicion(char* posicion);
char* buscar_tareas(char* path);
char* contarTareas(char* palabra);
int validar_entrada(char **palabras_separadas);
void listar_tripulantes();
void imprimir_estado_patota(t_patota* patota);
void iniciar_planificacion_fifo();
void obtener_tarea_en_ram(t_tripulante* tripulante,int* referencia_tarea);
char** normalizar_tarea(char* tarea);
int es_una_tarea_io(char* tarea);
void desplazar_tripulante(t_tripulante* tripulante,t_posicion* posicion);
void resolver_tarea_io(t_tripulante* tripulante,int rafaga_de_io);
void resolver_tarea_cpu(t_tripulante* tripulante,int rafaga_de_cpu);
void iniciar_planificacion();
bool tripulante_finaliza(t_tripulante* tripulante);
bool tripulante_pide_bloqueo(t_tripulante* tripulante);
void log_tripulantes_pidiendo_bloqueo(t_tripulante* tripulante);
void ejecutar_tripulante(t_tripulante* tripulante);
void log_tripulante_sin_tareas(t_tripulante* tripulante);
void pausar_planificacion();
void expulsar_tripulante(char* tid_tripulante);
bool ordenar_tripulante_segun_tid(t_tripulante* tripulante1,t_tripulante* tripulante2);
void iniciar_resolucion_sabotaje();
t_tripulante* tripulante_mas_cercano_al_sabotaje(t_tripulante* tripulante1,t_tripulante* tripulante2);
t_queue* list_to_queue(t_list* list);
t_list* queue_to_list(t_queue* queue);
void resolver_sabotaje(t_tripulante* tripulante,t_posicion* posicion);
void desplazar_tripulante_a_sabotaje(t_tripulante_sabotaje* tripulante_sabotaje);
void destruir_recursos_tripulante(t_tripulante* tripulante);
void salir_expulsado(t_tripulante* tripulante);
void planificacion_round_robin();
bool tripulante_sin_quantum(t_tripulante* tripulante);
void ejecutar_tripulante_restando_quantum(t_tripulante* tripulante);
#endif /* UTILS_CONSOLA_H_ */
