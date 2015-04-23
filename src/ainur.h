/*
 * ainur.h
 *
 *  Created on: Apr 10, 2015
 *      Author: rs14
 */

#ifndef AINUR_H_
#define AINUR_H_

#include <SDL/SDL.h>
#include <lua.h>

#include "debug.h"

struct engine {
    SDL_Surface *screen;
/*#ifdef VERBOSE
    SDL_Surface *verbose;
#endif VEROBSE*/
    lua_State *lkernel;
};

//#define USE_SDL2

extern struct engine ainur;

#endif /* AINUR_H_ */
