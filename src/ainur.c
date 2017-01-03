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
#include <SDL2/SDL_ttf.h>

#include "ainurio.h"
#include "debug.h"
#include "font.h"
#include "image.h"
#include "lkernel.h"
#include "screen.h"

/* initialize the ainur engine struct */
struct engine ainur = { NULL, NULL, NULL, NULL, NULL };



/**
 * @brief Cleanup protocols: should be used to free heap variables
 *        and quit other tasks (SDL, Lua).
 */
static void ainur_close()
{
    //functions are order dependent (reverse of loading)
    screen_freeMain();
    font_close();
    tile_close();
    image_close();
    screen_close();
    lkernel_close();
    return;
}



static int ainur_init(void) {
    lkernel_init();     //initialize Lua
    screen_init();      //initialize SDL2
    image_init();       //initialize IMG (SDL2 extension)
    tile_init();        //initialize tiles
    font_init();        //initialize TTF (SDL2 extension)

    return 1;
}

//TODO: static function to load setting from home directory



/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/



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
        if (strcmp(argv[arg], "--debug") == 0) {
            debug_debugOn();
        }
        #endif /*DEBUGGING*/
        #ifdef VERBOSE
        else if (strcmp(argv[arg], "--verbose") == 0) {
            debug_verboseOn();
        }
        #endif /*VERBOSE*/
    }

    //must be registered befor initialization.
    atexit(ainur_close);  //register cleanup code

    ainur_init();

    screen_initMain("Testing...", 400, 400);    //open the main window


    while(1) {
        //ainurio_SDLreceive();   //receive key input
        ainurio_interpretInput(); //interpret keystroke
        SDL_Delay(16);          //delay/pause to save CPU
    }

    exit(EXIT_SUCCESS); //call cleanup code
}

