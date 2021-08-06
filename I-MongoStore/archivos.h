#ifndef SRC_ARCHIVOS_H_
#define SRC_ARCHIVOS_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>
#include<sys/stat.h>
#include<openssl/md5.h>

#include"servidor.h"

void crearBitacora(char* path);
void crearRecurso(char* filePath, char* recurso);
void crearArchivoMetadata(char* filePath,char* tamanio,char* cant_bloques,char* bloques, char* caracter);
void sobreEscribirSuperBloque(char* path,int bloques);
void sobreEscribirRecurso(char* path,int bloques);
char* buscarBloquesUsados(char* path);

char* agregarBloquesFile(char* path,char* bloquesUsados);
void agregarTamanioFile(char* path,int tamanio);
void agregarCaracterFile(char* path,char caracter);
void agregarBlockCountFile(char* path,int tamanio);
void agregarMD5File(char* path,char* MD5);
void sacarBloque(char* id,char* path);
void reemplazarBloquesFile(char* path,char* bloquesUsados);

void eliminarFile(char* path);
void decrementarTamanioFile(char* path,int tamanio);
void crearMD5(char* filePath,char* palabra);
char* obtenerMD5(char* path);
int obtenerSizeFile(char* path);


#endif /* SRC_ARCHIVOS_H_ */
