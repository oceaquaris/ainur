/*
 * image.h
 *
 *  Created on: Apr 21, 2015
 *      Author: oceaquaris
 */

#ifndef IMAGE_H_
#define IMAGE_H_

#define IMAGE_SUCCESS   1
#define IMAGE_FAILURE   0

struct image {
    SDL_Surface *surfaces; //null terminated
    char *tag;
    char *filename;
    unsigned int width;
    unsigned int height;
    unsigned int columns;
    unsigned int rows;

};

/*
 * Function declarations.
 */
int image_initIMG(void);

SDL_Surface *image_load(const char *filename);

#endif /* IMAGE_H_ */
