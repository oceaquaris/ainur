#ifndef SPRITE_H
#define SPRITE_H

#include "tile.h"

#define SPRITE_MAX_NUM_FRAMES   32

struct sprite {
    unsigned int frames;    //number of frames in the sprite
    struct tile **tiles;    //an array of pointers to tile structs
};


#endif /*SPRITE_H*/