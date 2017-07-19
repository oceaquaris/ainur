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

#include "image.h"
#include "tile.h"



/**
 * @struct arguments
 *         Stores command line argument settings.
 * @var debug
 *      Whether we are debugging.
 * @var verbose
 *      Whether we want verbose.
 */
struct arguments {
#ifdef DEBUGGING
    int debug;
#endif
#ifdef VERBOSE
    int verbose;
#endif
};



/**
 * @struct engine
 *         Stores important data needed by the game.
 * @var screen
 *      Pointer to the main window (SDL_Window) created.
 * @var font
 *      Pointer to the main font loaded.
 * @var lkernel
 *      Pointer to the Lua kernel state used to read .lua files.
 * @var images
 *      Pointer to an array of pointers to image structs.
 *      @note The array is NULL terminated.
 *      Contains all loaded images.
 * @var tiles
 *      Pointer to an array of pointers to tile structs.
 *      @note The array is NULL terminated.
 *      Contains all created tiles.
 */
struct engine {
    SDL_Window *screen;         //main window
    TTF_Font *font;             //main font
    lua_State *lkernel;         //Lua kernel state
    struct image **images;
    struct tile **tiles;
/*#ifdef VERBOSE
    SDL_Surface *verbose; //for possible use in engine
#endif VEROBSE*/
};


extern struct engine ainur;     /* This struct is initialized in ainur.c */

#define LKERNEL ainur.lkernel

#endif /* AINUR_H_ */
