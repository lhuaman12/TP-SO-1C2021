#ifndef SRC_ARCHIVOS_H_
#define SRC_ARCHIVOS_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>
#include<sys/stat.h>


#include"servidor.h"

void crearBitacora(char* nombre);
char* crearRecurso(char* recurso);
void crearArchivoMetadata(char* filePath,char* tamanio,char* cant_bloques,char* bloques, char* caracter);
void sobreEscribirSuperBloque(char* path,int bloques);
void sobreEscribirRecurso(char* path,int bloques);

#endif /* SRC_ARCHIVOS_H_ */
