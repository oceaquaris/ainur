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
    SDL_Surface *surface;
    char *tag;
    char *filename;
};

/*
 * Function declarations.
 */
extern struct image * image_bsearch      (const char *tag);
extern void           image_free         (struct image *image);
extern int            image_initIMG      (void);
extern struct image * image_load         (const char *filename, const char *tag);
extern SDL_Surface *  image_load_surface (const char *filename);
extern size_t         image_num_loaded   (void);
extern void           image_qsort        (void);

#endif /* IMAGE_H_ */
