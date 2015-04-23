/*
 * init.c
 *
 *  Created on: Apr 10, 2015
 *      Author: rs14
 */

#include "ainur.h"

#ifdef USE_SDL2
#include <SDL2/SDL.h>
#include <SDL2/SDL_Surface.h>
#else
#include <SDL/SDL.h>
#include <SDL/SDL_video.h>
#endif /*USE_SDL2*/
#include <stdlib.h>
#include <string.h>

#include "ainurio.h"
#include "debug.h"
#include "screen.h"

struct engine ainur;

/**
 * @brief Initializes SDL and creates a window.
 *
 * @return 0: Normal program execution;
 *         exit()s on execution failure;
 */
int screen_initMain(const char *title, int width, int height)
{
    if( SDL_Init(SDL_INIT_VIDEO) < 0 ) { //if SDL_Init() has an error (not equal to 0)
        #ifdef DEBUGGING
        if(debugging) {
            debug_fprintf("screen_initMain() => SDL_Init() error: %s\n", SDL_GetError());
        }
        #endif /*DEBUGGING*/
        #ifdef VERBOSE
        if(verbose) {
            debug_printf("screen_initMain() => SDL_Init() error: %s\n", SDL_GetError());
        }
        #endif /*VERBOSE*/

        exit(EXIT_FAILURE); //quit program & execute shutdown protocols
    }

    //create the main SDL_Surface
    ainur.screen = SDL_SetVideoMode(width, height, 0, SDL_HWPALETTE|SDL_DOUBLEBUF);

    if( !(ainur.screen) ) {
        #ifdef DEBUGGING
        if(debugging) {
            debug_fprintf("screen_initMain() => SDL_SetVideoMode() => couldn\'t set screen mode to %dx%d: %s\n",
                          width, height, SDL_GetError());
        }
        #endif /*DEBUGGING*/
        #ifdef VERBOSE
        if(verbose) {
            debug_printf("screen_initMain() => SDL_SetVideoMode() => couldn\'t set screen mode to %dx%d: %s\n",
                          width, height, SDL_GetError());
        }
        #endif /*VERBOSE*/

        exit(EXIT_FAILURE); //quit program & execute shutdown protocols
    }

    //set the title of a window
    SDL_WM_SetCaption(title, NULL);
    //atexit(SDL_Quit);

    return 0;
}



/**
 *
 * @note Should be opened before the main screen.
 */
/*#ifdef VERBOSE
int screen_initVerbose(int height, int width)
{
    if(verbose) {
        ainur.verbose = SDL_SetVideoMode(width, height, 0, SDL_HWPALETTE|SDL_DOUBLEBUF);
        if( !(ainur.verbose) ) {
            #ifdef DEBUGGING
            if(debugging) {
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
        if(debugging) {
            debug_fprintf("screen_dumpSDL_Surface() => formal parameter \'surface\': %s\n", ERROR_NULL_POINTER);
        }
        #endif /*DEBUGGING*/
        #ifdef VERBOSE
        if(verbose) {
            debug_printf("screen_dumpSDL_Surface() => formal parameter \'surface\': %s\n", ERROR_NULL_POINTER);
        }
        #endif /*VERBOSE*/

        return;
    }
    if(!label) {
        #ifdef DEBUGGING
        if(debugging) {
            debug_fprintf("screen_dumpSDL_Surface() => formal parameter \'label\': %s\n", ERROR_NULL_STRING);
        }
        #endif /*DEBUGGING*/
        #ifdef VERBOSE
        if(verbose) {
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
    #ifdef USE_SDL2
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

    //regular sdl section
    #else
    if(surface->flags & SDL_ANYFORMAT) {
        ainurio_shstrcat(buffer, "             SDL_ANYFORMAT: Allow any video depth/pixel-format\n");
    }
    if(surface->flags & SDL_ASYNCBLIT) {
        ainurio_shstrcat(buffer, "             SDL_ASYNCBLIT: Use asynchronous blits if possible\n");
    }
    if(surface->flags & SDL_DOUBLEBUF) {
        ainurio_shstrcat(buffer, "             SDL_DOUBLEBUF: Set up double-buffered video mode\n");
    }
    if(surface->flags & SDL_HWACCEL) {
        ainurio_shstrcat(buffer, "               SDL_HWACCEL: Blit uses hardware acceleration\n");
    }
    if(surface->flags & SDL_HWPALETTE) {
        ainurio_shstrcat(buffer, "             SDL_HWPALETTE: Surface has exclusive palette\n");
    }
    if(surface->flags & SDL_HWSURFACE) {
        ainurio_shstrcat(buffer, "             SDL_HWSURFACE: Surface is in video memory\n");
    }
    if(surface->flags & SDL_FULLSCREEN) {
        ainurio_shstrcat(buffer, "            SDL_FULLSCREEN: Surface is a full screen display\n");
    }
    if(surface->flags & SDL_NOFRAME){
        ainurio_shstrcat(buffer, "               SDL_NOFRAME: No window caption or edge frame\n");
    }
    if(surface->flags & SDL_OPENGL) {
        ainurio_shstrcat(buffer, "                SDL_OPENGL: Create an OpenGL rendering context\n");
    }
    if(surface->flags & SDL_OPENGLBLIT) {
        ainurio_shstrcat(buffer, "            SDL_OPENGLBLIT: Create an OpenGL rendering context and use it for blitting\n");
    }
    if(surface->flags & SDL_PREALLOC){
        ainurio_shstrcat(buffer, "              SDL_PREALLOC: Surface uses preallocated memory\n");
    }
    if(surface->flags & SDL_RESIZABLE) {
        ainurio_shstrcat(buffer, "             SDL_RESIZABLE: This video mode may be resized\n");
    }
    if(surface->flags & SDL_RLEACCEL) {
        ainurio_shstrcat(buffer, "              SDL_RLEACCEL: Surface is RLE encoded\n");
    }
    if(surface->flags & SDL_SRCALPHA) {
        ainurio_shstrcat(buffer, "              SDL_SRCALPHA: Blit uses source alpha blending\n");
    }
    if(surface->flags & SDL_SRCCOLORKEY) {
        ainurio_shstrcat(buffer, "           SDL_SRCCOLORKEY: Blit uses a source color key\n");
    }
    if(surface->flags & SDL_SWSURFACE) {
        ainurio_shstrcat(buffer, "             SDL_SWSURFACE: Surface is in system memory\n");
    }
    #endif /*USE_SDL2*/


    #ifdef DEBUGGING
    if(debugging) {
        debug_fprintf("Information on SDL_Surface %s:\n", label);

    }
    #endif /*DEBUGGING*/
    #ifdef VERBOSE
    if(verbose) {

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
