/*
 * font.c
 *
 *  Created on: Apr 27, 2015
 *      Author: oceaquaris
 */


#include "font.h"

#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_ttf.h>

#include "ainur.h"
#include "color.h"
#include "debug.h"
#include "file.h"



//static functions
static SDL_Color *hexcodetorgba(const char *rgbaCode);
static unsigned char hex_to_int(char tens_16, char ones_16);

/**
 * @brief Initializes SDL_ttf library.
 */
int font_initTTF(void)
{
    if( TTF_Init() < 0 ) {
        #ifdef DEBUGGING
        if(debug_getDebugStatus()) {
            debug_fprintf("font_initTTF() => TTF_Init() error: %s", TTF_GetError());
        }
        #endif /*DEBUGGING*/
        #ifdef VERBOSE
        if(debug_getVerboseStatus()) {
            debug_printf("font_initTTF() => TTF_Init() error: %s", TTF_GetError());
        }
        #endif /*VERBOSE*/

        exit(EXIT_FAILURE); //close the program
    }

    return 0;
}


/**
 * @brief Loads a truetype font with a specified size.
 *
 * @param filename
 *        Name of the .ttf file to open
 * @param ptsize
 *        Size of the font to display.
 *
 * @return A loaded TTF_Font (heap variable). Loaded font needs to be
 * 		   TTF_CloseFont()ed.
 */
TTF_Font *font_load(const char *filename, int ptsize)
{
    if(!filename) { //test pointers
        #ifdef DEBUGGING
        if(debug_getDebugStatus()) {
            debug_fprintf("font_load() => formal param \'filename\': %s\n", ERROR_NULL_STRING);
        }
        #endif /*DEBUGGING*/
        #ifdef VERBOSE
        if(debug_getVerboseStatus()) {
            debug_printf("font_load() => formal param \'filename\': %s\n", ERROR_NULL_STRING);
        }
        #endif /*VERBOSE*/

        return NULL;
    }

    //test to see if file exists
    if( !file_exists(filename) ) {
        //file_exists will print an error message
        return NULL;
    }

    TTF_Font *output;
    output = TTF_OpenFont(filename, ptsize);

    if(!output) {
        #ifdef DEBUGGING
        if(debug_getDebugStatus()) {
            debug_fprintf("font_load() => Unable to load font %s: %s\n", filename, TTF_GetError());
        }
        #endif /*DEBUGGING*/
        #ifdef VERBOSE
        if(debug_getVerboseStatus()) {
            debug_printf("font_load() => Unable to load font %s: %s\n", filename, TTF_GetError());
        }
        #endif /*VERBOSE*/
    }

    return output;
}


/**
 *
 */
int font_initMain()
{
	ainur.font = font_load("VL-Gothic-Regular.ttf", 15);
	if( !(ainur.font) ) {
		#ifdef DEBUGGING
		if(debug_getDebugStatus()) {
			debug_fprintf("font_initMain() => Unable to load default font.\n");
		}
		#endif /*DEBUGGING*/
		#ifdef VERBOSE
		if(debug_getVerboseStatus()) {
			debug_printf("font_initMain() => Unable to load default font.\n");
		}
		#endif /*VERBOSE*/

		exit(EXIT_FAILURE); //exit due to failure
	}

	return 0;
}


void font_freeMain()
{
	if( !(ainur.font) ) { //font wasn't initialized.
		return;
	}
	TTF_CloseFont(ainur.font);
	return;
}

/**
 * @brief Creates an SDL_Surface with text using text blending
 *
 * @param text
 *        Text to draw as image
 * @param font
 *        Font to use for text
 * @param fontColor
 *        Color of the font
 *
 * @return An image of text
 * @note Needs to be SDL_FreeSurface()ed.
 */
//TODO: clean up this code for debug
SDL_Surface *font_draw(const char *text,
					   TTF_Font *font,
					   SDL_Color fontColor)
{
	if(!text || !font) {
		return NULL;
	}

	SDL_Surface *output;
	output = TTF_RenderText_Blended(font, text, fontColor);

	if(!output) {

	}
	return output;
}

/**
 * @brief Draws text using the main/default text for the ainur engine
 *
 * @param text
 *        Text to draw as image
 * @param color
 *        Color of the font
 *
 * @return An image of text
 * @note Needs to be SDL_FreeSurface()ed.
 */
SDL_Surface *font_drawColor(const char *text, SDL_Color color)
{
    if(!text) {
        //TODO: debug stuff
        return NULL;
    }

    SDL_Surface *output;
    output = TTF_RenderText_Blended(ainur.font, text, color);

    if(!output) {
        //TODO: debug stuff
    }
    return output;
}

SDL_Surface *font_drawDefault(const char *text)
{
    if(!text) {
        //TODO
        return NULL;
    }

    SDL_Surface *output;
    output = TTF_RenderText_Blended(ainur.font, text, RGBA_WHITE);

    if(!output) {
        //TODO
    }
    return output;
}

static SDL_Color *hexcodetorgba(const char *rgbaCode)
{
    if( strlen(rgbaCode) != 8 ) {
        //TODO: error codes
        return NULL;
    }

    SDL_Color *output;
    output = (SDL_Color *)malloc(sizeof(*output));
    output->r = hex_to_int(rgbaCode[0], rgbaCode[1]);
    output->g = hex_to_int(rgbaCode[2], rgbaCode[3]);
    output->b = hex_to_int(rgbaCode[4], rgbaCode[5]);
    output->a = hex_to_int(rgbaCode[6], rgbaCode[7]);

    return output;
}

static unsigned char hex_to_int(char tens_16, char ones_16)
{
    unsigned char tens_10 = 0, ones_10 = 0;

    //convert tens_16 to tens_10
    if(tens_16 >= '0' && tens_16 <= '9') {
        tens_10 = tens_16 - '0';
    }
    else if(tens_16 >= 'a' && tens_16 <= 'f') {
        tens_10 = tens_16 - 'a' + 11;
    }
    else if(tens_16 >= 'A' && tens_16 <= 'F') {
        tens_10 = tens_16 - 'A' + 11;
    }

    //convert ones_16 to ones_10
    if(ones_16 >= '0' && ones_16 <= '9') {
        ones_10 = ones_16 - '0';
    }
    else if(ones_16 >= 'a' && ones_16 <= 'f') {
        ones_10 = ones_16 - 'a' + 11;
    }
    else if(ones_16 >= 'A' && ones_16 <= 'F') {
        ones_10 = ones_16 - 'A' + 11;
    }

    return (tens_10 * 16) + (ones_10 * 1);
}
