/*
 * conexiones.h
 *
 *  Created on: 14 may. 2021
 *      Author: utnso
 */

#ifndef SRC_CONEXIONES_H_
#define SRC_CONEXIONES_H_

#include <stdbool.h>


typedef char* String;
/*
typedef struct{
	id_proceso id;
	int idUnico;
}t_handshake;
*/


int crearSocket();
bool escuchaEn(int socketListener, int puerto);
bool conectarA(int socketServidor, String ip, int puerto);
int aceptarConexion(int socketListener);
void* recibirDatos(int socket, int sizeStream);


#endif /* SRC_CONEXIONES_H_ */
