/*
 * image.c
 *
 *  Created on: Apr 21, 2015
 *      Author: rs14
 */

#include "ainur.h"

#ifdef USE_SDL2
#include <SDL/SDL2.h>
#else
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#endif /*USE_SDL2*/
#include <stdlib.h>
#include <stdio.h>

#include "debug.h"
#include "image.h"
#include "file.h"

/**
 * @brief Initializes SDL's image loading library.
 *
 * @return 0: program success
 *         exit()s program on failure.
 */
int image_loadInit()
{
    if( !IMG_Init(IMG_INIT_PNG) ) { //if IMG_Init() fails
        #ifdef DEBUGGING
        if(debugging) {
            debug_fprintf("image_load() => IMG_Init() error: %s\n", IMG_GetError());
        }
        #endif /* DEBUGGING */
        #ifdef VERBOSE
        if(verbose) {
            debug_printf("image_load() => IMG_Init() error: %s\n", IMG_GetError());
        }
        #endif /*VERBOSE*/

        exit(EXIT_FAILURE); //close program and free everything.
    }

    return 0; //success
}


/**
 * @brief Loads an image and converts it to a usable SDL_Surface.
 *
 * @param filename
 *        Path to the file/file name
 *
 * @return An SDL_Surface with the loaded pixels.
 *
 * @note As always, output SDL_Surface needs to be SDL_FreeSurface()ed.
 */
SDL_Surface *image_load(const char *filename)
{
    if(!filename) {
        #ifdef DEBUGGING
        if(debugging) {
            debug_fprintf("image_load() => formal param \'filename\': %s\n", ERROR_NULL_STRING);
        }
        #endif /* DEBUGGING */
        #ifdef VERBOSE
        if(verbose) {
            debug_printf("image_load() => formal param \'filename\': %s\n", ERROR_NULL_STRING);
        }
        #endif /*VERBOSE*/

        return NULL;
    }

    SDL_Surface *temp = IMG_Load(filename);
    if(!temp) {
        #ifdef DEBUGGING
        if(debugging) {
            debug_fprintf("image_load() => local var \'temp\': %s\n", ERROR_NULL_SDL_SURFACE);
            debug_fprintf("                Testing if \'filename\' exists...\n");
        }
        #endif /* DEBUGGING */
        #ifdef VERBOSE
        if(verbose) {
            debug_printf("image_load() => local var \'temp\': %s\n", ERROR_NULL_SDL_SURFACE);
            debug_printf("                Testing if \'filename\' exists...\n");
        }
        #endif /*VERBOSE*/

        if(file_exists(filename)) { //error message will be printed in file_exists()
            //if the file exists, there must by an SDL error...
            #ifdef DEBUGGING
            if(debugging) {
                debug_fprintf("image_load() => \'temp\' SDL error: %s\n", IMG_GetError());
            }
            #endif /* DEBUGGING */
            #ifdef VERBOSE
            if(verbose) {
                debug_printf("image_load() => \'temp\' SDL error: %s\n", IMG_GetError());
            }
            #endif /*VERBOSE*/
        }

        return NULL;
    }

    SDL_Surface *output;

    /* Make the background transparent */
    SDL_SetColorKey(temp, (SDL_SRCCOLORKEY|SDL_RLEACCEL), SDL_MapRGB(temp->format, 0, 0, 0));

    /* Convert the image to the screen's native format */
    output = SDL_DisplayFormat(temp);

    SDL_FreeSurface(temp);

    if(!output) {
        #ifdef DEBUGGING
        if(debugging) {
            debug_fprintf("image_load() => local var \'output\': %s\n", ERROR_NULL_SDL_SURFACE);
        }
        #endif /* DEBUGGING */
        #ifdef VERBOSE
        if(verbose) {
            debug_printf("image_load() => local var \'output\': %s\n", ERROR_NULL_SDL_SURFACE);
        }
        #endif /*VERBOSE*/

        return NULL;
    }

    return output;
}

