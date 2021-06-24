/*
 * fsck.c
 *
 *  Created on: 24 jun. 2021
 *      Author: utnso
 */

#include "fsck.h"

void repararSuperBloque()
{
	int bloquesActuales = contarBloques();
	sobreEscribirSuperBloque(RUTA_SUPER_BLOQUE,bloquesActuales);
}

