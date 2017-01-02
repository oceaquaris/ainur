/*
 * init.c
 *
 *  Created on: Apr 10, 2015
 *      Author: oceaquaris
 */


#include "screen.h"

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>

#include "ainur.h"
#include "ainurio.h"
#include "debug.h"



/**
 * @brief Initializes SDL2
 *
 * @return 0: normal execution
 */
int screen_initSDL()
{
    if( SDL_Init(SDL_INIT_EVERYTHING) < 0 ) { //if SDL_Init() has an error (not equal to 0)
        #if defined(DEBUGGING) || defined(VERBOSE)
        debug_print("screen_initSDL() => SDL_Init() error: %s\n", SDL_GetError());
        #endif
//         #ifdef DEBUGGING
//         if(debug_getDebugStatus()) {
//             debug_fprintf("screen_initSDL() => SDL_Init() error: %s\n", SDL_GetError());
//         }
//         #endif /*DEBUGGING*/
//         #ifdef VERBOSE
//         if(debug_getVerboseStatus()) {
//             debug_printf("screen_initSDL() => SDL_Init() error: %s\n", SDL_GetError());
//         }
//         #endif /*VERBOSE*/

        exit(EXIT_FAILURE); //quit program & execute shutdown protocols
    }

    return 0;
}

/**
 * @brief Initializes SDL and creates a window.
 *
 * @return 0: Normal program execution;
 *         exit()s on execution failure;
 */
int screen_initMain(const char *title, int width, int height)
{
    //create the main SDL_Surface
    //ainur.screen = SDL_SetVideoMode(width, height, 0, SDL_HWPALETTE|SDL_DOUBLEBUF);
    ainur.screen = SDL_CreateWindow(title,
                                    SDL_WINDOWPOS_CENTERED,
                                    SDL_WINDOWPOS_CENTERED,
                                    width,
                                    height,
                                    SDL_WINDOW_OPENGL);

    if( !(ainur.screen) ) {
        #ifdef DEBUGGING
        if(debug_getDebugStatus()) {
            debug_fprintf("screen_initMain() => SDL_CreateWindow() => couldn\'t set screen mode to %dx%d: %s\n",
                          width, height, SDL_GetError());
        }
        #endif /*DEBUGGING*/
        #ifdef VERBOSE
        if(debug_getVerboseStatus()) {
            debug_printf("screen_initMain() => SDL_CreateWindow() => couldn\'t set screen mode to %dx%d: %s\n",
                          width, height, SDL_GetError());
        }
        #endif /*VERBOSE*/

        exit(EXIT_FAILURE); //quit program & execute shutdown protocols
    }

    //atexit(SDL_Quit);

    return 0;
}


/**
 * @brief Free the main SDL_Window
 */
void screen_freeMain()
{
	if(!(ainur.screen)) { //if screen was never initialized
		return;
	}
	SDL_DestroyWindow(ainur.screen);
	return;
}


/**
 *
 * @note Should be opened before the main screen.
 */
/*#ifdef VERBOSE
int screen_initVerbose(int height, int width)
{
    if(debug_getVerboseStatus()) {
        ainur.verbose = SDL_SetVideoMode(width, height, 0, SDL_HWPALETTE|SDL_DOUBLEBUF);
        if( !(ainur.verbose) ) {
            #ifdef DEBUGGING
            if(debug_getDebugStatus()) {
                debug_fprintf("screen_initVerbose() => SDL_SetVideoMode() => couldn\'t set screen mode to %dx%d: %s\n",
                              width, height, SDL_GetError());
            }
            #endif DEBUGGING

            exit(EXIT_FAILURE); //quit program & execute shutdown protocols
        }

        SDL_WM_SetCaption(title, NULL);
    }

    return 0;
}
#endif VERBOSE*/



/**
 *
 */
void screen_dumpSDL_Surface(const char *label, SDL_Surface *surface)
{
    if(!surface) {
        #ifdef DEBUGGING
        if(debug_getDebugStatus()) {
            debug_fprintf("screen_dumpSDL_Surface() => formal parameter \'surface\': %s\n", ERROR_NULL_POINTER);
        }
        #endif /*DEBUGGING*/
        #ifdef VERBOSE
        if(debug_getVerboseStatus()) {
            debug_printf("screen_dumpSDL_Surface() => formal parameter \'surface\': %s\n", ERROR_NULL_POINTER);
        }
        #endif /*VERBOSE*/

        return;
    }
    if(!label) {
        #ifdef DEBUGGING
        if(debug_getDebugStatus()) {
            debug_fprintf("screen_dumpSDL_Surface() => formal parameter \'label\': %s\n", ERROR_NULL_STRING);
        }
        #endif /*DEBUGGING*/
        #ifdef VERBOSE
        if(debug_getVerboseStatus()) {
            debug_printf("screen_dumpSDL_Surface() => formal parameter \'label\': %s\n", ERROR_NULL_STRING);
        }
        #endif /*VERBOSE*/

        label = "Unlabeled SDL_Surface";
    }

    char buffer[100] = {0}; //beginning buffer for the string.
    //begin writing information to string...
    sprintf(buffer, "Information on SDL_Surface %s:\n", label);

    /*
     * @brief Beginning of section testing for flags on the SDL_Surface
     *
     * @note  There are two different sets of flags (which I could find) for SDL and SDL2
     */
    ainurio_shstrcat(buffer, "         flags:\n");
    //sdl2 section
    if(surface->flags & SDL_DONTFREE) {
        ainurio_shstrcat(buffer, "              SDL_DONTFREE: Surface is referenced internally\n");
    }
    if(surface->flags & SDL_PREALLOC){
        ainurio_shstrcat(buffer, "              SDL_PREALLOC: Surface uses preallocated memory\n");
    }
    if(surface->flags & SDL_RLEACCEL) {
        ainurio_shstrcat(buffer, "              SDL_RLEACCEL: Surface is RLE encoded\n");
    }
    if(surface->flags & SDL_SWSURFACE) {
        ainurio_shstrcat(buffer, "             SDL_SWSURFACE: Surface is in system memory\n");
    }


    #ifdef DEBUGGING
    if(debug_getDebugStatus()) {
        debug_fprintf("Information on SDL_Surface %s:\n", label);

    }
    #endif /*DEBUGGING*/
    #ifdef VERBOSE
    if(debug_getVerboseStatus()) {

    }
    #endif /*VERBOSE*/
}

void screen_dumpSDL_PixelFormat(const char *label, SDL_PixelFormat *pixelformat)
{
    //todo
}

void screen_dumpSDL_Rect(const char *label, SDL_Rect *rect)
{
    //todo
}
