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
 * @return IMAGE_SUCCESS: program success
 *         exit()s program on failure.
 */
int image_initIMG(void) {
    int flags  = IMG_INIT_PNG;
    int loaded = IMG_Init(IMG_INIT_PNG); //0x00000002
    
    if( (loaded&flags) != flags) { //if IMG_Init() fails
        #if defined(DEBUGGING) || defined(VERBOSE)
        debug_print("image_initIMG: IMG_Init error: Failed to initialize required png support.\n"\
                    "image_initIMG: IMG_Init error: %s\n", IMG_GetError());
        #endif /*defined DEBUGGING || defined VERBOSE*/
        
        exit(EXIT_FAILURE); //close program and free everything.
    }

    return IMAGE_SUCCESS; //success
}


/**
 * @brief Loads an image and converts it to a usable SDL_Surface.
 * @note Error checking:
 *          filename is valid
 *
 * @param filename
 *        Path to the file/file name
 *
 * @return An SDL_Surface with the loaded pixels.
 *
 * @note As always, output SDL_Surface needs to be SDL_FreeSurface()ed.
 */
SDL_Surface *image_load(const char *filename) {
    if( !file_exists(filename) ) {
        #if defined(DEBUGGING) || defined(VERBOSE)
        debug_print("image_load: formal param 'filename'(%s): %s\n", filename, ERROR_NO_FILE);
        #endif /*defined DEBUGGING || defined VERBOSE*/

        return NULL;
    }

    SDL_Surface *temp = IMG_Load(filename);     //Load an SDL_Surface using the filename given.

    if(!temp) {
        #if defined(DEBUGGING) || defined(VERBOSE)
        debug_print("image_load: IMG_Load error: %s\n", IMG_GetError());
        #endif /*defined DEBUGGING || defined VERBOSE*/
        
        return temp;    //aka NULL
    }

    SDL_Surface *output;    //Declare an SDL_Surface (for output by function)

    SDL_SetColorKey(temp,
                    SDL_TRUE, 
                    SDL_MapRGB(temp->format, 0, 0, 0)   ); /* Make the background transparent */

    output = SDL_ConvertSurfaceFormat(temp,
                                      SDL_PIXELTYPE_UNKNOWN,
                                      0 ); /* Convert the image to the screen's native format */

    if(!output) {
        #if defined(DEBUGGING) || defined(VERBOSE)
        debug_print("image_load: local var 'output': %s\n"\
                    "            SDL error: %s\n", ERROR_NULL_SDL_SURFACE, SDL_GetError() );
        #endif /*defined DEBUGGING || defined VERBOSE*/

        return output;  //aka NULL
    }

    SDL_FreeSurface(temp);  //Free our temporary variable

    return output;
}
