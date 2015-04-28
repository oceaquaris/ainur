/*
 * image.c
 *
 *  Created on: Apr 21, 2015
 *      Author: oceaquaris
 */


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <stdio.h>

#include "ainur.h"
#include "debug.h"
#include "image.h"
#include "file.h"

/**
 * @brief Initializes SDL's image loading library.
 *
 * @return 0: program success
 *         exit()s program on failure.
 */
int image_initIMG()
{
    if( !IMG_Init(IMG_INIT_PNG /*0x00000002*/) ) { //if IMG_Init() fails
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

    //Load an SDL_Surface using the filename given.
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

        if(file_exists(filename)) {
            //an error message will be printed by file_exists() if filename doesn't exist
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

    //Declare an SDL_Surface (for output by function)
    SDL_Surface *output;

    /* Make the background transparent */
    SDL_SetColorKey(temp, SDL_TRUE, SDL_MapRGB(temp->format, 0, 0, 0));

    /* Convert the image to the screen's native format */
    output = SDL_ConvertSurfaceFormat(temp, SDL_PIXELTYPE_UNKNOWN, 0 );

    if(!output) {
        #ifdef DEBUGGING
        if(debugging) {
            debug_fprintf("image_load() => local var \'output\': %s\n", ERROR_NULL_SDL_SURFACE);
            debug_fprintf("                %s\n", SDL_GetError());
        }
        #endif /* DEBUGGING */
        #ifdef VERBOSE
        if(verbose) {
            debug_printf("image_load() => local var \'output\': %s\n", ERROR_NULL_SDL_SURFACE);
            debug_printf("                %s\n", SDL_GetError());
        }
        #endif /*VERBOSE*/

        return NULL;
    }

    //Free our temporary variable
    SDL_FreeSurface(temp);

    return output;
}
