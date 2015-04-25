/*
 * ainur.h
 *
 *  Created on: Apr 10, 2015
 *      Author: oceaquaris
 */

#ifndef AINUR_H_
#define AINUR_H_

#include <lua.h>
#include <SDL2/SDL_video.h>


struct engine {
    SDL_Window *screen;
/*#ifdef VERBOSE
    SDL_Surface *verbose;
#endif VEROBSE*/
    lua_State *lkernel;
};

extern struct engine ainur;

#endif /* AINUR_H_ */
