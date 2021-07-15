/*
 * md5.h
 *
 *  Created on: 13 jul. 2021
 *      Author: utnso
 */

#ifndef MD5_H_
#define MD5_H_

#include "servidor.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/md5.h>


char *str2md5(const char *str, int length);


#endif /* MD5_H_ */
