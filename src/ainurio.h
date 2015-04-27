/*
 * input.h
 *
 *  Created on: Apr 17, 2015
 *      Author: oceaquaris
 */

#ifndef AINURIO_H_
#define AINURIO_H_

#include <stdio.h>

//receive a maximum of 16 keys pressed simultaneously
#define AINURIO_MAX_SDLRECEIVE	16

int ainurio_SDLreceive();
void ainurio_interpretKey();
char *ainurio_lhstrcat(unsigned int argc, ...);
char *ainurio_shstrcat(char *destination, const char *source);
char *ainurio_rawInput(FILE *input, unsigned int size);

#endif /* AINURIO_H_ */
