/**
 * @file ainur.c
 *
 *  Created on: Mar 11, 2015
 *      Author: oceaquaris
 *
 * @brief Begins the game screen, etc.
 */


//External libraries...
#include "ainur.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

#include "ainurio.h"
#include "debug.h"
#include "image.h"
#include "lkernel.h"
#include "screen.h"

static void ainur_cleanEngine();

/**
 * @brief The main function. Opens a window. Begins game.
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

    lkernel_init();		//initialize Lua
    screen_initSDL();	//initialize SDL2
    image_loadInit();	//initialize IMG (SDL2 extension)

    screen_initMain("Testing...", 400, 400);	//open the main window

    atexit(ainur_cleanEngine); 	//register cleanup code

    while(1) {
        ainurio_SDLreceive();	//receive key input
        ainurio_interpretKey(); //interpret keystroke
        SDL_Delay(16);			//delay/pause to save CPU
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
