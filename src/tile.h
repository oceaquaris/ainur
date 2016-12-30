#ifndef TILE_H
#define TILE_H

#include <SDL2/SDL.h>


/**
 * @struct tile
 * 
 * @brief Coords to a region on an SDL_Surface
 */
struct tile {
    SDL_Surface *source;    //source surface/image
    SDL_Rect region;        //area on the sprite sheet
};

/*
 * Function declarations.
 */
extern struct tile *tile_create( SDL_Surface *src, int x, int y, int width, int height );
extern struct tile **tile_extractFromSurface( SDL_Surface *src, int std_h, int std_w );
extern struct tile **tile_extractFromImage(const char *filename, int std_h, int std_w );

//free'ing functions
extern void tile_free(struct tile *tile);
extern void tile_free_array(struct tile **tiles);
extern void tile_free_arrayRecursive(struct tile **tiles);

#endif /*TILE_H*/