#ifndef ESTRUCTURAS_H_
#define ESTRUCTURAS_H_


// las estructuras van a ser globales para poder ser consultadas luego por consola


// SOLO PARA DISCORDIADOR
typedef enum {
	NEW,READY,EXEC,BLOQ,EXIT
}t_estado;


//// enviar a ram
typedef enum {
	INICIAR_PATOTA,
	ENVIAR_POSICION,
	SOLICITAR_TAREA,
	EXPULSAR
}t_mensajes_para_ram;

//// recibir de ram
typedef enum {
	CONFIRMACION_INICIAR_PATOTA,
	ERROR_AL_CREAR_PATOTA
}t_mensajes_de_ram_hq;


typedef struct{

}t_tarea;

typedef struct{
	int32_t x;
	int32_t y;
}t_posicion;

typedef struct{
	int32_t TID;
	int32_t PID; // a que patota pertenece el tripulante
	t_tarea* tarea_actual;
	t_estado estado;
	t_posicion* posicion;
}t_tripulante;


/// las tareas no los necesito se los mando a ram y el me los va dando
typedef struct{
	int32_t PID;
	t_tripulante** tripulantes;
	int32_t cantidad_tripulantes;
}t_patota;


typedef struct{
	t_tripulante* tripulante;

}cola_tripulantes;



#endif /* ESTRUCTURAS_H_ */
