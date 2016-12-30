/**
 * @file sprite.h
 * 
 * Created On: 2015-02-01
 *     Author: oceaquaris
 */

#include <stdarg.h>
#include <stdlib.h>

#include "debug.h"
#include "sprite.h"
#include "tile.h"


struct sprite *sprite_create(int argc, struct tile *tile, ...) {
    if(argc < 1) {
        #if defined(DEBUGGING) || defined(VERBOSE)
        debug_print("sprite_create: Argument count less than 1!\n");
        #endif /*defined DEBUGGING || defined VERBOSE*/
        return NULL;
    }
    if(argc > SPRITE_MAX_NUM_FRAMES) {
        #if defined(DEBUGGING) || defined(VERBOSE)
        debug_print("sprite_create: Argument count greater than %d!\n", SPRITE_MAX_NUM_FRAMES);
        #endif /*defined DEBUGGING || defined VERBOSE*/
        argc = 32;
    }
    
    //create an array of pointers to tiles
    struct tile **tiles;
    if( !(tiles = (struct tile **)malloc( sizeof(struct tile *) * argc )) ) {
        #if defined(DEBUGGING) || defined(VERBOSE)
        debug_print("sprite_create: local var 'tiles': %s\n", ERROR_MALLOC);
        #endif /*defined DEBUGGING || defined VERBOSE*/
        return NULL;
    }
    
    tiles[0] = tile;   //populate the first position (guaranteed) with tile t
   
    //begin variable args
    va_list args;
    va_start(args, tile);
    
    register int i;
    for(i = 1; i < argc; i++) {
        tiles[i] = va_arg(args, struct tile *);
    }
    
    va_end(args);
    
    //create the sprite struct
    struct sprite *output;
    if( !(output = (struct sprite *)malloc(sizeof(struct sprite))) ) {
        #if defined(DEBUGGING) || defined(VERBOSE)
        debug_print("sprite_create: local var 'output': %s\n", ERROR_MALLOC);
        #endif /*defined DEBUGGING || defined VERBOSE*/
        free(tiles);
        return output;  //aka NULL
    }
    
    //put our tile variables inside of sprite
    output->frames = argc;
    output->tiles = tiles;
    
    return output;
}

void sprite_freeSprite(struct sprite *s) {
    if(s)
        free(s);
    return;
}

void sprite_freeSprite_recursively(struct sprite *s) {
    if(!s) {
        //debug statement...
        return;
    }
    
    register int i;
    for(i = 0; i < s->frames; i++) {
        if(s->tiles[i]) {
            tile_free(s->tiles[i]);
        }
    }
    
    free(s);
    return;
}