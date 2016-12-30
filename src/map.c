
#include "map.h"

#include <stdlib.h>
#include <string.h>

struct map *map_createNewMap(const char *tag,
                             unsigned int height,
                             unsigned int width,
                             //struct palette *paint_palette,
                             const char **floor
                             //char **wall,
                             //int spawnc,
                             //struct spawnable_item *items
            )
{
    struct map *output;
    output = (struct map *)malloc(sizeof(struct map));
    
    //populate output->tag
    int size = sizeof(char) * strlen(tag);
    output->tag = (char *)malloc( size );
    memcpy(output->tag, tag, size);
    
    //populate height and width
    output->height = height;
    output->width = width;
    
    //populate output->floor
    char **tempfloor;
    tempfloor = (char **)malloc(sizeof(char *) * height);
    int i;
    for(i = 0; i < height; i++) {
        int msize = sizeof(char) * strlen(floor[i]);
        tempfloor[i] = (char *)malloc( msize );
        memcpy(tempfloor[i], floor[i], msize);
    }
    output->floor = tempfloor;
    
    return output;
}

void map_freeMap(struct map *m)
{
    
}