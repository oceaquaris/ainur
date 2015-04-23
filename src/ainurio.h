/*
 * input.h
 *
 *  Created on: Apr 17, 2015
 *      Author: rs14
 */

#ifndef AINURIO_H_
#define AINURIO_H_

#include <stdarg.h>

int ainurio_SDLreceive();
char *ainurio_lhstrcat(unsigned int argc, ...);
char *ainurio_shstrcat(char *destination, const char *source);

#endif /* AINURIO_H_ */
