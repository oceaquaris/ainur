/*
 * input.h
 *
 *  Created on: Apr 17, 2015
 *      Author: oceaquaris
 */

#ifndef AINURIO_H_
#define AINURIO_H_

#include <stdio.h>

int ainurio_SDLreceive();
char *ainurio_lhstrcat(unsigned int argc, ...);
char *ainurio_shstrcat(char *destination, const char *source);
char *ainurio_rawInput(FILE *input, unsigned int size);

#endif /* AINURIO_H_ */
