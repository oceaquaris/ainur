/*
 * init.h
 *
 *  Created on: Apr 10, 2015
 *      Author: oceaquaris
 */

#ifndef SCREEN_H_
#define SCREEN_H_

#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_surface.h>

int screen_initSDL();
int screen_initMain(const char *title, int width, int height);
void screen_dumpSDL_Surface(const char *label, SDL_Surface *surface);
void screen_dumpSDL_PixelFormat(const char *label, SDL_PixelFormat *pixelformat);
void screen_dumpSDL_Rect(const char *label, SDL_Rect *rect);


#endif /* SCREEN_H_ */
