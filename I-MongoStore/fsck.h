/*
 * fsck.h
 *
 *  Created on: 24 jun. 2021
 *      Author: utnso
 */

#ifndef FSCK_H_
#define FSCK_H_

#include "servidor.h"

void repararSuperBloque();
void repararBitmap();
void responder_bitacora(Tripulante* trip);
#endif /* FSCK_H_ */
