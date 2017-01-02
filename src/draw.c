/**
 * @file draw.c
 */

#include <SDL/SDL.h>

#include "draw.h"

static unsigned int frameLimit = /*SDL_GetTicks() +*/ 16; //initial fps ~= 60fps

/**
 * @brief 
 */
void draw_delay(unsigned int targetDelay) {
    unsigned int ticks = SDL_GetTicks();

    if (frameLimit < ticks) {
        return;
    }

    SDL_Delay(frameLimit - ticks); //Delay to save CPU time
    
    frameLimit = SDL_GetTicks() + targetDelay; //set frameLimit to a targeted refresh time
    return;
}
