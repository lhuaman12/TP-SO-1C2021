/*
 * estructuras.h
 *
 *  Created on: 23 jul. 2021
 *      Author: utnso
 */

#ifndef ESTRUCTURAS_H_
#define ESTRUCTURAS_H_
typedef struct {

	uint32_t idPatota; 				//Identificador de la patota
	uint32_t tareas; 			//Direccion logica del inicio de las tareas

}t_pcb;

typedef struct {

	uint32_t idTripulante; 				//ID del tripulante
	uint32_t posX; 				//Pos en eje x
	uint32_t posY; 				//Pos en eje y
	uint32_t proxInstruccion; 	//Id de la proxima instruccion
	uint32_t puntero_pcb; 		//Direccion logica del PCB del tripulante, su patota
	char estado; 				//Estado del tripulante (N/R/E/B)

}t_tcb;


#endif /* ESTRUCTURAS_H_ */
