/*
 * discordiador.h
 *
 *  Created on: 26 abr. 2021
 *      Author: utnso
 */

#ifndef DISCORDIADOR_H_
#define DISCORDIADOR_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>
#include<string.h>

#include "utils.h"

// Por lo general, a menos que tengan que tener recursos compartidos, no es buena práctica usar variables globales. Nosotros lo estamos haciendo por la simplicidad del TP0 :)
t_log* discordiador_logger;
t_config* discordiador_config;

void iniciar_logger(void);
void leer_config(void);
void leer_consola(void);
void paquete(int);
void terminar_programa(int);

#endif /* DISCORDIADOR_H_ */
