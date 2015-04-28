/*
 * image.h
 *
 *  Created on: Apr 21, 2015
 *      Author: oceaquaris
 */

#ifndef IMAGE_H_
#define IMAGE_H_

struct image {
    SDL_Surface *surfaces; //null terminated
    char *tag;
    char *filename;
    unsigned int width;
    unsigned int height;
    unsigned int columns;
    unsigned int rows;

};

SDL_Surface *image_load(const char *filename);
int image_initIMG();

#endif /* IMAGE_H_ */
