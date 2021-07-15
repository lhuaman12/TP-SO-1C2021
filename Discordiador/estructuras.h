#ifndef ESTRUCTURAS_H_
#define ESTRUCTURAS_H_
#include <commons/collections/queue.h>
#include <semaphore.h>

// las estructuras van a ser globales para poder ser consultadas luego por consola


typedef enum {
	NEW,READY,EXEC,BLOCKED,EXIT
}t_estado;


//// enviar a ram
typedef enum {
	INICIAR_PATOTA,
	//ENVIAR_POSICION,
	EXPULSAR_TRIPULANTE
}t_mensajes_para_ram;


typedef enum{
	SOLICITAR_TAREA,   // ram
	ENVIAR_POSICION_ACTUAL, //mongo,ram
}mensajes_submodulo_tripulante;

typedef enum {
	ENVIAR_POSICION,
	COMIENZA_TAREA,
	FINALIZA_TAREA

}mensajes_para_mongo;

//// recibir de ram
typedef enum {
	CONFIRMACION_INICIAR_PATOTA,
	ERROR_AL_CREAR_PATOTA
}t_mensajes_de_ram_hq;


typedef struct{
	char* tarea_string;
	char** tarea;
	int32_t rafaga;

}t_tarea;

typedef struct{
	int32_t x;
	int32_t y;
}t_posicion;

typedef struct{
	int32_t TID;
	int32_t PID; // a que patota pertenece el tripulante
	char* tarea_actual; // empieza en null
	t_estado estado; //empieza en NEW en discordiador al pasar a ram pasa a ready
	t_posicion* posicion;
	int32_t socket_ram;
	int32_t socket_mongo_hq;
	sem_t semaforo_tripulante;
	sem_t esperar_ejecucion_tripulante;
	int32_t es_expulsado; //TODO: envolver en otra estructura los estados
	int32_t rafaga_io_restantes;
	int32_t no_tiene_tareas;
	int32_t pide_bloqueo;
	int32_t es_elegido_para_sabotaje;
	pthread_mutex_t mutex;

}t_tripulante;


/// las tareas no los necesito se los mando a ram y el me los va dando
typedef struct{
	int32_t PID;
	t_tripulante** tripulantes;
	int32_t cantidad_tripulantes;
}t_patota;
////

typedef struct{
	t_tripulante* tripulante;
	t_posicion* posicion;
	sem_t semaforo;
}t_tripulante_sabotaje;

/// estructuras para FIFO

typedef struct{
	t_queue* cola_tripulantes_new;
	t_queue* cola_tripulantes_ready;
	t_queue* cola_tripulantes_block;
	t_queue* cola_tripulantes_block_emergencia;
	t_list* tripulantes_exec;
}t_estructura_fifo;

t_estructura_fifo* estructura_planificacion;


char** tareas_io;
char** tareas_aux;

////////////////////////

#endif /* ESTRUCTURAS_H_ */
