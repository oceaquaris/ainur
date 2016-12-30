/**
 * @file tile.c
 */

#include <stdlib.h>
#include <SDL2/SDL.h>

#include "debug.h"
#include "image.h"
#include "tile.h"

/**
 * @brief Creates a tile struct containing a source image surface and coordinates
 *        for a section of the surface to use as the image for the "tile."
 * 
 * @param src
 *        Source SDL_Surface, where the region we want as the tile image resides.
 * @param x
 *        X coordinate of the upper left corner of the tile rectangle (SDL_Rect).
 * @param y
 *        Y coordinate of the upper left corner of the tile rectangle (SDL_Rect).
 * @param width
 *        Width of the tile rectangle (SDL_Rect).
 * @param height
 *        Height of the tile rectangle (SDL_Rect).
 * 
 * @return A tile struct with a source image and coordinates to the tile.
 */
struct tile *tile_create( SDL_Surface *src, int x, int y, int width, int height ) {
    struct tile *output;
    SDL_Rect rect;
    
    //allocate mem for tile; check if allocation was successful
    if( !(output = (struct tile *)malloc(sizeof(struct tile))) ) { //TODO: debug
        return NULL;
    }
    
    //put variables inside rect
    rect.x = x;
    rect.y = y;
    rect.w = width;
    rect.h = height;
    
    output->source = src;   //SDL_Surface should already be a heap variable
    output->region = rect;
    
    return output;
}



/**
 * @brief Extract tiles from an SDL_Surface.
 * @note  Will ignore "scraps" left over on sides of image if surface size
 *        is not divisible by std_h and/or std_w
 * 
 * @param src
 *        Source SDL_Surface to cut.
 * @param std_h
 *        Standard tile height.
 * @param std_w
 *        Standard tile width.
 * 
 * @return An array of tiles, array is NULL terminated.
 */
struct tile **tile_extractFromSurface( SDL_Surface *src, int std_h, int std_w ) {
    if(!src) {
        #if defined(DEBUGGING) || defined(VERBOSE)
        debug_print("tile_extractFromSurface: %s\n", ERROR_NULL_SDL_SURFACE);
        #endif /*defined DEBUGGING || defined VERBOSE*/
        
        return NULL;
    }
    
    int tiles_h = src->h / std_h,
        tiles_w = src->w / std_w;
    
    struct tile **tiles;
    
    if( !(tiles = (struct tile **)malloc(sizeof(struct tile *) * ((tiles_h * tiles_w)+1))) ) {
        //TODO debug
        return tiles; //aka NULL
    }
    
    /*Loop proceeds as follows
     * [0][1][2][3][4]
     * [5][6][7][8][9]
     * ...
     */
    register int i, j, index = 0;
    for(i = 0; i < tiles_h; i++) { //height, y value
        for(j = 0; j < tiles_w; j++) { //width, x value
            tiles[index++] = tile_create(src, j*std_w, i*std_h, std_w, std_h);
        }
    }
    tiles[index] = NULL; //end with a NULL
    
    return tiles;
}



/**
 * @brief Extract tiles from an image file.
 * 
 * @param filename
 *        Name of file to load.
 * @param std_h
 *        Standard tile height.
 * @param std_w
 *        Standard tile width.
 * 
 * @return An array of tiles, array is NULL terminated.
 */
struct tile **tile_extractFromImage(const char *filename, int std_h, int std_w ) {
    SDL_Surface *surf = image_load(filename);
    if(!surf) {
        #if defined(DEBUGGING) || defined(VERBOSE)
        debug_print("tile_extractFromImage: local var 'surf': %s\n", ERROR_NULL_SDL_SURFACE);
        #endif /*defined DEBUGGING || defined VERBOSE*/
        
        return NULL;
    }
    
    return tile_extractFromSurface(surf, std_h, std_w);
}



/**
 * @brief Free's a tile struct.
 * @param tile
 *        tile struct to be free'd.
 */
void tile_free(struct tile *tile) {
    if(!tile)
        return;
    
    if(tile->source) {
        SDL_FreeSurface(tile->source);
    }
    
    free(tile);
    return;
}



/**
 * @brief Free's ONLY the array containing tiles. Does NOT free tile structs, themselves.
 * 
 * @param tiles
 *        Array of tile structs.
 */
void tile_free_array(struct tile **tiles) {
    if(!tiles)
        return;
    
    free(tiles);
    return;
}



/**
 * @brief Free's an array of tile structs and the tile structs, themselves.
 * 
 * @param tiles
 *        Array of tile structs to be free'd.
 *        @note Array must be NULL terminated!
 */
void tile_free_arrayRecursive(struct tile **tiles) {
    if(!tiles)
        return;
    
    register int i;
    for(i = 0; tiles[i]; i++) {
        tile_free(tiles[i]);
    }
    
    free(tiles);
    return;
}