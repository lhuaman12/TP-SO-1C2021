/*
 * conexiones.c
 *
 *  Created on: 14 may. 2021
 *      Author: utnso
 */


#include "conexiones.h"
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "netdb.h"


//1- el SERVIDOR y CLIENTE crean su socket. si se creo bien me devuelve el socket, sino devuelve -1
int crearSocket() {
	return socket(AF_INET, SOCK_STREAM, 0);
}

//2- el SERVIDOR hace el bind y listen. Si escucha bien te devuelve true, sino false
bool escuchaEn(int socketListener, int puerto) {
	struct sockaddr_in direccion;
	direccion.sin_family = AF_UNSPEC; //
	direccion.sin_addr.s_addr = INADDR_ANY;
	direccion.sin_port = htons(puerto);

	if (bind(socketListener, (struct sockaddr *) &direccion, sizeof(struct sockaddr)) == -1) {
		//no se pudo ejecutar el bind en el socket
		return false;
	}

	return listen(socketListener, 100) != -1;
}

//3- el CLIENTE hace el connect con el servidor que ya esta escuchando. Si se conecta bien te devuelve true, sino false
//por cada conectarA tiene que haber un aceptarConexion
bool conectarA(int socketServidor, String ip, int puerto) {
	struct sockaddr_in direccion;
	direccion.sin_family = AF_INET;
	direccion.sin_addr.s_addr = inet_addr(ip);
	direccion.sin_port = htons(puerto);

	//si devuelve 0 entonces se conecto bien y devuelve true
	return connect(socketServidor, (struct sockaddr*) &direccion, sizeof(struct sockaddr)) == 0;
}

//4- el SERVIDOR recibe el socket que esta escuchando
//devuelve un nuevo socket que representa la conexion con el cliente que se acaba de conectar
//se crean tantas conexiones (nuevos sockets) como tantas peticiones de conexion
//tiene que estar en un select para que no quede el proceso bloqueado esperando hasta que tenga una conexion que escuchar
int aceptarConexion(int socketListener) {
	return accept(socketListener, NULL, NULL);
}


//devuelve el stream completo para que no se corten en el recv
void* recibirDatos(int socket, int sizeStream) {
	int tamanioAcumulado = 0;
	void* datos = malloc(sizeStream);

	while(tamanioAcumulado < sizeStream) {
		//donde se va a almacenar la cantidad de bytes recuperados del socket
		int tamanioDatosRecibidos = recv(socket, datos + tamanioAcumulado, sizeStream - tamanioAcumulado, MSG_NOSIGNAL);

		if(tamanioDatosRecibidos <= 0) {
			free(datos);
			return NULL;
		} else {
			tamanioAcumulado += tamanioDatosRecibidos;
		}
	}

	return datos;
}




