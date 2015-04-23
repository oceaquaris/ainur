/**
 * @file ainur.c
 *
 *  Created on: Mar 11, 2015
 *      Author: rs14
 *
 * @brief Begins the game screen, etc.
 */

#include "ainur.h" //order dependent!! Needs to be first for #define USE_SDL2

//External libraries...
#ifdef USE_SDL2
#include <SDL2/SDL.h>
#else
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#endif /*USE_SDL2*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

//Internal libraries...
#include "ainurio.h"
#include "debug.h"
#include "file.h"
#include "image.h"
#include "lkernel.h"
#include "mem.h"
#include "screen.h"

static void ainur_cleanEngine();


/**
 * @brief The main function. Begins game.
 *
 * @param argc
 *        Argument count inputed from the command line.
 * @param argv
 *        Arguments as strings.
 */
int main(int argc, char *argv[])
{
    int arg;
    for (arg = 1; arg < argc; arg++) {
        #ifdef DEBUGGING //if compiled with debug options
        if (strcmp(argv[arg], "-debug") == 0) {
            debugging = 1;                              //set option to 1 (true)
            debug_logbook = fopen(logbook_name, "w");   //assign pointers and create file
            fclose(debug_logbook);                      //close file for safety
            //time(&current_time);                        //get the current time for debug
        }
        #endif /*DEBUGGING*/
        #ifdef VERBOSE
        else if (strcmp(argv[arg], "-verbose") == 0) {
            verbose = 1;
        }
        #endif /*VERBOSE*/
    }

    lkernel_init();

    screen_initMain("Testing...", 400, 400);
    image_loadInit();

    atexit(ainur_cleanEngine);

    while(1) {
        ainurio_SDLreceive();
        SDL_Delay(16);
    }

    exit(EXIT_SUCCESS); //call cleanup code
}

/**
 * @brief Cleanup protocols: should be used to free heap variables
 *        and quit other tasks (SDL, Lua).
 */
static void ainur_cleanEngine()
{
    SDL_Quit();
    IMG_Quit();
    lkernel_close();
    return;
}
