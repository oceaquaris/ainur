/**
 * @file tile.c
 *
 * Field Overview:
 *  static:
 *      tile_compare_bsearch
 *      tile_compare_qsort
 *  extern:
 */

#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>

#include "ainur.h"
#include "debug.h"
#include "image.h"
#include "tile.h"



/**
 * @brief Wrapper function that performs a comparison between a key and
 *        the 'tag' of a tile struct.
 *
 * @param pkey
 *        A pointer to a (char *) key to be compared.
 * @param pelem
 *        A pointer to a (struct tile *) to be compared.
 *
 * @return < 0: pkey has a lower 'tag' string value than pelem.
 *         = 0: pkey and pelem have the same 'tag' label.
 *         > 0: pkey has a higher 'tag' string value than pelem.
 */
static int tile_compare_bsearch(const void *pkey, const void *pelem) {
    return strcmp( ((char *)pkey), ((struct tile **)pelem)[0]->tag );
}



/**
 * @brief Wrapper function that performs a comparison on the 'tag's of two
 *        struct tiles *'s.
 *
 * @param p1
 *        A pointer to a (struct tile *) to be compared.
 * @param p2
 *        A pointer to a (struct tile *) to be compared.
 *
 * @return < 0: p1 has a lower 'tag' string value than p2.
 *         = 0: p1 and p2 have the same 'tag' label.
 *         > 0: p1 has a higher 'tag' string value than p2.
 */
static int tile_compare_qsort(const void *p1, const void *p2) {
    return strcmp( ((struct tile **)p1)[0]->tag, ((struct tile **)p2)[0]->tag );
}



/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/



/**
 * @brief A wrapper function to perform a binary search on 'tiles' in the ainur engine.
 *        Retrieve an element, if one exists, that has the 'tag', 'tag' within it.
 *
 * @param tag
 *        The 'tag' of the (struct tile *) to search for.
 *
 * @return A pointer to the array position with the correct 'tag', or NULL (no match).
 */
struct tile **tile_bsearch(const char *tag) {
    if(!tag) { return NULL; }

    return (struct tile **)bsearch( tag,
                                    ainur.tiles,
                                    tile_numRegistered(),
                                    sizeof(struct tile *),
                                    tile_compare_bsearch );
}



/**
 * @brief Wrapper function to perform cleanup protocols for tile functionalities.
 */
void tile_close(void) {
    tile_freeAll();
    return;
}



/**
 * 
 */
struct tile *tile_create(const char *image_tag, int x, int y, int width, int height, const char *tag) {
    struct image **image = image_bsearch(image_tag);
    if(!image) {
        dbgprint("tile_create: Unable to create tile: %s\n"\
                 "             Image not found.\n", tag);

        return NULL;
    }

    return tile_create_fromImage( *image, x, y, width, height, tag );
}



/**
 * @brief Creates a tile struct containing a source image surface and coordinates
 *        for a section of the surface to use as the image for the "tile."
 *
 * @param src
 *        Source struct image *, where the region we want as the tile image resides.
 * @param x
 *        X coordinate of the upper left corner of the tile rectangle (SDL_Rect).
 * @param y
 *        Y coordinate of the upper left corner of the tile rectangle (SDL_Rect).
 * @param width
 *        Width of the tile rectangle (SDL_Rect).
 * @param height
 *        Height of the tile rectangle (SDL_Rect).
 * @param tag
 *        Tag under which this tile is listed. This 'tag' must be unique.
 *
 * @return A tile struct with a source image and coordinates to the tile.
 */
struct tile *tile_create_fromImage( struct image *src, int x, int y, int width, int height, const char *tag ) {
    //'tag' cannot be NULL!!!
    if(!tag) {
        dbgprint("tile_create_fromImage: Unable to create tile: formal param 'tag' is NULL.\n");

        return NULL;
    }

    //'tag' must be unique!!!
    if( tile_bsearch(tag) ) {
        dbgprint("tile_create_fromImage: Unable to create tile: 'tag' %s is not unique.\n", tag);

        return NULL;
    }

    struct tile *output;

    //allocate mem for tile; check if allocation was successful
    if( !(output = (struct tile *)malloc(sizeof(struct tile))) ) {
        dbgprint("tile_create_fromImage: Unable to allocate memory for new (struct image *): %s\n", tag);

        return NULL;
    }

    //put coordinates inside rect
    output->rect.x = x;
    output->rect.y = y;
    output->rect.w = width;
    output->rect.h = height;
    //put image source in tile struct
    output->src = src;

    //attempt to allocate memory for 'tag' string
    size_t length = strlen(tag);
    if( !(output->tag = malloc( sizeof(char) * (length + 1) )) ) {
        dbgprint("tile_create_fromImage: Unable to allocate memory for new (struct tile *): %s\n", tag);

        tile_free(output);
        return NULL;
    }
    //copy 'tag' to output->tag
    memcpy(output->tag, tag, sizeof(char) * (length + 1));

    //attempt to allocate more memory for the tile array
    length = tile_numRegistered();
    if( !(ainur.tiles = realloc(ainur.tiles, sizeof(struct tile *) * (length + 2))) ) {
        dbgprint("tile_create_fromImage: Unable to allocate more memory for ainur.(struct tile **tiles).\n");

        tile_free(output);
        return NULL;
    }

    ainur.tiles[length] = output;     //add the new tile to the end of the array
    ainur.tiles[length + 1] = NULL;   //add new NULL terminator

    tile_qsort();   //resort the list

    return output;
}



/**
 * @brief Free's a tile struct.
 * @param tile
 *        tile struct to be free'd.
 */
void tile_free(struct tile *tile) {
    if(!tile) { return; }

    if(tile->tag) {
        free(tile->tag);
    }

    free(tile);
    return;
}



/**
 * @brief Free all tiles within ainur.tiles and free the ainur.tiles array, itself.
 */
void tile_freeAll(void) {
    if(!ainur.tiles) { return; }

    size_t len;
    for(len = tile_numRegistered(); len > 0; len--) {
        tile_free(ainur.tiles[len-1]);
    }

    free(ainur.tiles);
    return;
}



/**
 * @brief Allocate memory for ainur.tiles; attach NULL terminator.
 *
 * @return TILE_SUCCESS if initalization was successful;
 *         TILE_FAILURE if initalization did not succeed.
 */
int tile_init(void) {
    if(ainur.tiles) {
        return TILE_SUCCESS;
    }

    if( !(ainur.tiles = malloc(sizeof(struct tile *))) ) {
        dbgprint("tile_init: Unable to allocate memory for ainur.(struct tile **tiles).\n");

        return TILE_FAILURE;
    }
    ainur.tiles[0] = NULL;    //add NULL terminator

    return TILE_SUCCESS;
}



/**
 * @brief Determine the number of tile structs in ainur.tiles.
 *
 * @return The number of tile structs in ainur.tiles before the NULL terminator.
 */
size_t tile_numRegistered(void) {
    size_t i;
    for(i = 0; ainur.tiles[i]; i++);
    return i;
}



/**
 * @brief A wrapper function to perform a quicksort (qsort()) on ainur.tiles.
 */
void tile_qsort(void) {
    qsort( ainur.tiles,
           tile_numRegistered(),
           sizeof(struct tile *),
           tile_compare_qsort );
    return;
}
