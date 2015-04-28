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
#include <SDL2/SDL_ttf.h>


struct engine {
    SDL_Window *screen;		//main window
    TTF_Font *font;			//main font
    lua_State *lkernel;		//Lua kernel state
};
/*#ifdef VERBOSE
    SDL_Surface *verbose; //for possible use above
#endif VEROBSE*/

extern struct engine ainur;

#endif /* AINUR_H_ */
