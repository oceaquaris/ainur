/*
 * font.h
 *
 *  Created on: Apr 27, 2015
 *      Author: rs14
 */

#ifndef SRC_FONT_H_
#define SRC_FONT_H_

#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_ttf.h>

int font_initTTF(void);
TTF_Font *font_load(const char *filename, int ptsize);
int font_initMain();
void font_freeMain();
SDL_Surface *font_draw(const char *text,
					   TTF_Font *font,
					   SDL_Color fontColor,
					   SDL_Color bgColor);

#endif /* SRC_FONT_H_ */
