/*
 * font.c
 *
 *  Created on: Apr 27, 2015
 *      Author: oceaquaris
 */


#include "font.h"

#include <stdlib.h>
#include <SDL2/SDL_ttf.h>

#include "ainur.h"
#include "debug.h"
#include "file.h"


//external struct
struct engine ainur;

/**
 * @brief Initializes SDL_ttf library.
 */
int font_initTTF(void)
{
	if( TTF_Init() < 0 ) {
		#ifdef DEBUGGING
		if(debugging) {
			debug_fprintf("font_initTTF() => TTF_Init() error: %s", TTF_GetError());
		}
		#endif /*DEBUGGING*/
		#ifdef VERBOSE
		if(verbose) {
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
		if(debugging) {
			debug_fprintf("font_load() => formal param \'filename\': %s\n", ERROR_NULL_STRING);
		}
		#endif /*DEBUGGING*/
		#ifdef VERBOSE
		if(verbose) {
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
		if(debugging) {
			debug_fprintf("font_load() => Unable to load font %s: %s\n", filename, TTF_GetError());
		}
		#endif /*DEBUGGING*/
		#ifdef VERBOSE
		if(verbose) {
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
		if(debugging) {
			debug_fprintf("font_initMain() => Unable to load default font.\n");
		}
		#endif /*DEBUGGING*/
		#ifdef VERBOSE
		if(verbose) {
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


//TODO: clean up this code for debug
SDL_Surface *font_draw(const char *text,
					   TTF_Font *font,
					   SDL_Color fontColor,
					   SDL_Color bgColor)
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
