#ifndef MAP_H
#define MAP_H


struct map {
    char *tag;
    unsigned int height;
    unsigned int width;
    //struct palette *paint_palette;
    char **floor;
    //char **wall;
    //int spawnc;
    //struct spawnable_item *items;
};

#endif /*MAP_H*/