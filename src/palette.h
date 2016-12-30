#ifndef PALETTE_H
#define PALETTE_H

#include "sprite.h"
#include "tile.h"


#define FLOOR_PALETTE_SIZE  64
enum floor_palette {
    //Defaults for all and groupings
    //Can also serve as special sprite indices
    FLOOR_DEFAULT_ALL = 0,
    FLOOR_DEFAULT_1   = 1,
    FLOOR_DEFAULT_2   = 2,
    FLOOR_DEFAULT_3   = 3,
    FLOOR_DEFAULT_4   = 4,
    FLOOR_DEFAULT_5   = 5,
    FLOOR_DEFAULT_6   = 6,
    FLOOR_DEFAULT_7   = 7,
    
    //First group of floor sprites
    FLOORG1_0   = 8,
    FLOORG1_1   = 9,
    FLOORG1_2   = 10,
    FLOORG1_3   = 11,
    FLOORG1_4   = 12,
    FLOORG1_5   = 13,
    FLOORG1_6   = 14,
    FLOORG1_7   = 15,
    
    //Second group of floor sprites
    FLOORG2_0   = 16,
    FLOORG2_1   = 17,
    FLOORG2_2   = 18,
    FLOORG2_3   = 19,
    FLOORG2_4   = 20,
    FLOORG2_5   = 21,
    FLOORG2_6   = 22,
    FLOORG2_7   = 23,
    
    //Third group of floor sprites
    FLOORG3_0   = 24,
    FLOORG3_1   = 25,
    FLOORG3_2   = 26,
    FLOORG3_3   = 27,
    FLOORG3_4   = 28,
    FLOORG3_5   = 29,
    FLOORG3_6   = 30,
    FLOORG3_7   = 31,
    
    //Fourth group of floor sprites
    FLOORG4_0   = 32,
    FLOORG4_1   = 33,
    FLOORG4_2   = 34,
    FLOORG4_3   = 35,
    FLOORG4_4   = 36,
    FLOORG4_5   = 37,
    FLOORG4_6   = 38,
    FLOORG4_7   = 39,
    
    //Fifth group of floor sprites
    FLOORG5_0   = 40,
    FLOORG5_1   = 41,
    FLOORG5_2   = 42,
    FLOORG5_3   = 43,
    FLOORG5_4   = 44,
    FLOORG5_5   = 45,
    FLOORG5_6   = 46,
    FLOORG5_7   = 47,
    
    //Sixth group of floor sprites
    FLOORG6_0   = 48,
    FLOORG6_1   = 49,
    FLOORG6_2   = 50,
    FLOORG6_3   = 51,
    FLOORG6_4   = 52,
    FLOORG6_5   = 53,
    FLOORG6_6   = 54,
    FLOORG6_7   = 55,
    
    //Seventh group of floor sprites
    FLOORG7_0   = 56,
    FLOORG7_1   = 57,
    FLOORG7_2   = 58,
    FLOORG7_3   = 59,
    FLOORG7_4   = 60,
    FLOORG7_5   = 61,
    FLOORG7_6   = 62,
    FLOORG7_7   = 63
};

#define WALL_PALETTE_SIZE   64
enum wall_palette {
    //Defaults for all and groupings
    //Can also serve as special sprite indices
    WALL_DEFAULT_ALL = 0,
    WALL_DEFAULT_1   = 1,
    WALL_DEFAULT_2   = 2,
    WALL_DEFAULT_3   = 3,
    WALL_DEFAULT_4   = 4,
    WALL_DEFAULT_5   = 5,
    WALL_DEFAULT_6   = 6,
    WALL_DEFAULT_7   = 7,
    
    //First group of wall sprites
    WALLG1_0   = 8,
    WALLG1_1   = 9,
    WALLG1_2   = 10,
    WALLG1_3   = 11,
    WALLG1_4   = 12,
    WALLG1_5   = 13,
    WALLG1_6   = 14,
    WALLG1_7   = 15,
    
    //Second group of wall sprites
    WALLG2_0   = 16,
    WALLG2_1   = 17,
    WALLG2_2   = 18,
    WALLG2_3   = 19,
    WALLG2_4   = 20,
    WALLG2_5   = 21,
    WALLG2_6   = 22,
    WALLG2_7   = 23,
    
    //Third group of wall sprites
    WALLG3_0   = 24,
    WALLG3_1   = 25,
    WALLG3_2   = 26,
    WALLG3_3   = 27,
    WALLG3_4   = 28,
    WALLG3_5   = 29,
    WALLG3_6   = 30,
    WALLG3_7   = 31,
    
    //Fourth group of wall sprites
    WALLG4_0   = 32,
    WALLG4_1   = 33,
    WALLG4_2   = 34,
    WALLG4_3   = 35,
    WALLG4_4   = 36,
    WALLG4_5   = 37,
    WALLG4_6   = 38,
    WALLG4_7   = 39,
    
    //Fifth group of wall sprites
    WALLG5_0   = 40,
    WALLG5_1   = 41,
    WALLG5_2   = 42,
    WALLG5_3   = 43,
    WALLG5_4   = 44,
    WALLG5_5   = 45,
    WALLG5_6   = 46,
    WALLG5_7   = 47,
    
    //Sixth group of wall sprites
    WALLG6_0   = 48,
    WALLG6_1   = 49,
    WALLG6_2   = 50,
    WALLG6_3   = 51,
    WALLG6_4   = 52,
    WALLG6_5   = 53,
    WALLG6_6   = 54,
    WALLG6_7   = 55,
    
    //Seventh group of wall sprites
    WALLG7_0   = 56,
    WALLG7_1   = 57,
    WALLG7_2   = 58,
    WALLG7_3   = 59,
    WALLG7_4   = 60,
    WALLG7_5   = 61,
    WALLG7_6   = 62,
    WALLG7_7   = 63
};

#define OVERLAY_PALETTE_SIZE    1
enum overlay_palette {
    OVERLAY_DEFAULT_ALL = 0
};

/**
 * @struct palette
 * 
 * @brief Represents a 
 */
struct palette {
    const int floor_palette_size;
    struct sprite *floor_palette[FLOOR_PALETTE_SIZE];       //array of pointers to floor sprites
    
    const int wall_palette_size;
    struct sprite *wall_palette[WALL_PALETTE_SIZE];         //array of pointers to wall sprites
    
    const int overlay_palette_size;
    struct sprite *overlay_palette[OVERLAY_PALETTE_SIZE];   //array of pointers to overlay sprites
};


#endif /*PALETTE_H*/