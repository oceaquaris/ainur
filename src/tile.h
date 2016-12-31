#ifndef TILE_H
#define TILE_H

#include <SDL2/SDL.h>

#include "image.h"


/**
 * @struct tile
 * 
 * @brief Coords to a region on an image struct.
 */
struct tile {
    struct image *src;  //source of the image
    SDL_Rect rect;      //area on the image that corresponds to this tile
    char *tag;          //tag under which this tile is listed; used for sorting, finding
};

/*
 * Function declarations.
 */
extern struct tile ** tile_bsearch          (const char *tag);
extern struct tile *  tile_create           (const char *image_tag, int x, int y, int width, int height, const char *tag);
extern struct tile *  tile_create_fromImage (struct image *src, int x, int y, int width, int height, const char *tag);
extern void           tile_free             (struct tile *tile);
extern size_t         tile_num_registered   (void);
extern void           tile_qsort            (void);

extern struct tile **tile_extractFromSurface( SDL_Surface *src, int std_h, int std_w );
extern struct tile **tile_extractFromImage(const char *filename, int std_h, int std_w );

//free'ing functions
extern void tile_free(struct tile *tile);
extern void tile_free_array(struct tile **tiles);
extern void tile_free_arrayRecursive(struct tile **tiles);

#endif /*TILE_H*/
