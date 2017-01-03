/*
 * image.c
 *
 *  Created on: Apr 21, 2015
 *      Author: oceaquaris
 *
 * Field Overview:
 *  static:
 *      image_compare
 *  extern:
 *      image_bsearch
 *      image_free
 *      image_freeAll
 *      image_freeTag
 *      image_init
 *      image_load
 *      image_loadSDL_Surface
 *      image_numLoaded
 *      image_qsort
 */


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <stdio.h>

#include "ainur.h"
#include "debug.h"
#include "image.h"
#include "file.h"



/**
 * @brief Wrapper function that performs a comparison on the strings two struct image *'s.
 *
 * @param p1
 *        A pointer to a (struct image *) to be compared.
 * @param p2
 *        A pointer to a (struct image *) to be compared.
 *
 * @return < 0: p1 has a lower 'tag' string value than p2.
 *         = 0: p1 and p2 have the same 'tag' label.
 *         > 0: p1 has a higher 'tag' string value than p2.
 */
static int image_compare(const void *p1, const void *p2) {
    return strcmp( ((struct image **)p1)[0]->tag, ((struct image **)p2)[0]->tag );
}



/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/



/**
 * @brief A wrapper function to perform a binary search on 'images' in the ainur engine.
 *        Retrieve an element, if one exists, that has the 'tag', 'tag' within it.
 * @param tag
 *        The 'tag' of the (struct image *) to search for.
 * @return A pointer to the array position with the correct 'tag', or NULL (no match).
 */
struct image **image_bsearch(const char *tag) {
    if(!ainur.images) { return NULL; }  //make sure 'images' isn't NULL

    return (struct image **)bsearch( tag,
                                     ainur.images,
                                     image_numLoaded(),
                                     sizeof(struct image *),
                                     image_compare );
}



/**
 * @brief Wrapper function to perform cleanup protocols for image functionalities.
 */
void image_close(void) {
    //order dependent
    image_freeAll();
    IMG_Quit();
    return;
}



/**
 * @brief Free an image struct.
 *
 * @param image
 *        Pointer to image struct to free.
 */
void image_free(struct image *image) { //FIXME: may want to make this static & eliminate 'images' checking
    if( !image ) { return; }    //check to see if our image is valid

    //free elements if available
    if( image->tag ) {
        free(image->tag);
    }
    if( image->filename ) {
        free(image->filename);
    }
    SDL_FreeSurface(image->surface);

    return;
}



/**
 * @brief Free all images in the ainur engine.
 */
void image_freeAll(void) {
    if(!ainur.images) { return; } //test to see if 'images' is valid

    size_t len;
    for(len = image_numLoaded(); len > 0; len--) {
        image_free(ainur.images[len-1]);
    }

    free(ainur.images);
    return;
}



/**
 * @brief Free an image struct with tag, 'tag'.
 * @param tag
 *        Tag of the image struct to free.
 */
void image_freeTag(const char *tag) {
    struct image **image;
    image = image_bsearch(tag); //find the image
    if(!image) {
        return;
    }

    size_t length = image_numLoaded();     //find the number of elements in 'images'

    image_free(image[0]);   //free the image that we found.

    image[0] = ainur.images[length - 1];  //copy a reference to the last image in 'images' where our freed image was.
    ainur.images[length - 1] = NULL;      //make the last reference in 'images' NULL

    //attempt to resize the 'images' array
    if( !( ainur.images = realloc(ainur.images, length * (sizeof(struct image *) ) ) ) ) {
        #if defined(DEBUGGING) || defined(VERBOSE)
        debug_print("image_freeTag: Unable to reallocate enough memory to resize static struct images **images.\n");
        #endif /*defined DEBUGGING || defined VERBOSE*/

        image_freeAll(); //free up memory
        return;
    }

    image_qsort();  //resort the list
    return;
}



/**
 * @brief Initializes SDL's image loading library.
 *
 * @return IMAGE_SUCCESS: program success
 *         exit()s program on failure.
 */
int image_init(void) {
    //order dependent
    int flags  = IMG_INIT_PNG;
    int loaded = IMG_Init(IMG_INIT_PNG); //0x00000002

    if( (loaded&flags) != flags) { //if IMG_Init() fails
        #if defined(DEBUGGING) || defined(VERBOSE)
        debug_print("image_init: IMG_Init error: Failed to initialize required png support.\n"\
                    "image_init: IMG_Init error: %s\n", IMG_GetError());
        #endif /*defined DEBUGGING || defined VERBOSE*/

        exit(EXIT_FAILURE); //close program and free everything.
    }

    //attempt to allocate memory for the 'images' array
    if( !(ainur.images = malloc(sizeof(struct image *))) ) {
        #if defined(DEBUGGING) || defined(VERBOSE)
        debug_print("image_init: IMG_Init error: Unable to allocate memory for struct tile **tiles.\n");
        #endif /*defined DEBUGGING || defined VERBOSE*/

        exit(EXIT_FAILURE); //close program and free everything.
    }

    //make the first element in the 'images' array to be a NULL terminator
    ainur.images[0] = NULL;

    return IMAGE_SUCCESS; //success
}



/**
 * @brief Load an image as an SDL_Surface from a 'filename' and stash it into an
 *        image struct. Stash the image struct into the 'images' variable in the
 *        ainur engine.
 * @param filename
 *        Name/path of the file to load.
 * @param tag
 *        Tag under which this image should be stored (cannot be NULL!).
 * @return A pointer to the newly created image struct.
 */
struct image *image_load(const char *filename, const char *tag) {
    //'tag' cannot be NULL!!!
    if(!tag) {
        #if defined(DEBUGGING) || defined(VERBOSE)
        debug_print("image_load: Unable to load image: %s.\n"\
                    "            formal param 'tag' is NULL.\n", filename);
        #endif /*defined DEBUGGING || defined VERBOSE*/

        return NULL;
    }

    //'tag' must be unique!!!
    if( image_bsearch(tag) ) {
        #if defined(DEBUGGING) || defined(VERBOSE)
        debug_print("image_load: Unable to load image: %s.\n"\
                    "            formal param 'tag' is not unique.\n", filename);
        #endif /*defined DEBUGGING || defined VERBOSE*/

        return NULL;
    }

    //first, attempt to load an SDL_Surface.
    SDL_Surface *surface = image_loadSDL_Surface(filename);
    if(!surface) {
        #if defined(DEBUGGING) || defined(VERBOSE)
        debug_print("image_load: Unable to load image: %s.\n", filename);
        #endif /*defined DEBUGGING || defined VERBOSE*/

        return NULL;
    }

    //second, attempt to create a new struct image.
    struct image *load = NULL;

    //attempt to allocate memory for the struct image
    if( !(load = malloc( sizeof(struct image) ))  ) {
        #if defined(DEBUGGING) || defined(VERBOSE)
        debug_print("image_load: Unable to allocate enough memory for new struct image: %s.\n", tag);
        #endif /*defined DEBUGGING || defined VERBOSE*/

        return load; //aka NULL
    }

    size_t length = strlen(tag);    //'length' will store the lengths of 'tag' and 'filename'

    //attempt to allocate memory for the (char *) field 'tag' in the struct image
    if ( !(load->tag = malloc( sizeof(char) * (length + 1) )) ) {
        #if defined(DEBUGGING) || defined(VERBOSE)
        debug_print("image_load: Unable to allocate enough memory for (%s)->tag.\n", tag);
        #endif /*defined DEBUGGING || defined VERBOSE*/

        image_free(load);   //free up memory
        return NULL;
    }
    //copy 'tag' to load->tag
    memcpy( load->tag, tag, sizeof(char) * (length + 1) );

    length = strlen(filename);  //reassign the length to that of 'filename'

    //attempt to allocate memory for the (char *) field 'filename' in the struct image
    if( !(load->filename = malloc( sizeof(char) * (length + 1) )) ) {
        #if defined(DEBUGGING) || defined(VERBOSE)
        debug_print("image_load: Unable to allocate enough memory for (%s)->filename.\n", tag);
        #endif /*defined DEBUGGING || defined VERBOSE*/

        image_free(load);   //free up memory
        return NULL;
    }
    //copy 'filename' to load->filename
    memcpy( load->filename, tag, sizeof(char) * (length + 1) );

    //third, attempt to extend the length of the statically allocated struct image **images
    int numloads = image_numLoaded();
    if ( !( ainur.images = realloc(ainur.images, (numloads + 2) * (sizeof(struct image *) ) ) ) ) {
        #if defined(DEBUGGING) || defined(VERBOSE)
        debug_print("image_load: Unable to reallocate enough memory to resize ainur.(struct images **images).\n");
        #endif /*defined DEBUGGING || defined VERBOSE*/

        free(load); //free up memory
        return NULL;
    }

    ainur.images[numloads] = load;        //insert new image into array
    ainur.images[numloads + 1] = NULL;    //attach new NULL terminator

    image_qsort();  //sort the array for future use.

    return load;    //return a pointer to the newly created and archived struct image.
}



/**
 * @brief Loads an image and converts it to a usable SDL_Surface.
 * @note Error checking:
 *          filename is valid
 *
 * @param filename
 *        Path to the file/file name
 *
 * @return An SDL_Surface with the loaded pixels.
 *
 * @note As always, output SDL_Surface needs to be SDL_FreeSurface()ed.
 */
SDL_Surface *image_loadSDL_Surface(const char *filename) {
    if( !file_exists(filename) ) {
        #if defined(DEBUGGING) || defined(VERBOSE)
        debug_print("image_loadSDL_Surface: formal param 'filename'(%s): %s\n", filename, ERROR_NO_FILE);
        #endif /*defined DEBUGGING || defined VERBOSE*/

        return NULL;
    }

    SDL_Surface *temp = IMG_Load(filename);     //Load an SDL_Surface using the filename given.

    if(!temp) {
        #if defined(DEBUGGING) || defined(VERBOSE)
        debug_print("image_loadSDL_Surface: IMG_Load error: %s\n", IMG_GetError());
        #endif /*defined DEBUGGING || defined VERBOSE*/

        return temp;    //aka NULL
    }

    SDL_Surface *output;    //Declare an SDL_Surface (for output by function)

    SDL_SetColorKey(temp,
                    SDL_TRUE, 
                    SDL_MapRGB(temp->format, 0, 0, 0)   ); /* Make the background transparent */

    output = SDL_ConvertSurfaceFormat(temp,
                                      SDL_PIXELTYPE_UNKNOWN,
                                      0 ); /* Convert the image to the screen's native format */

    if(!output) {
        #if defined(DEBUGGING) || defined(VERBOSE)
        debug_print("image_loadSDL_Surface: local var 'output': %s\n"\
                    "            SDL error: %s\n", ERROR_NULL_SDL_SURFACE, SDL_GetError() );
        #endif /*defined DEBUGGING || defined VERBOSE*/

        return output;  //aka NULL
    }

    SDL_FreeSurface(temp);  //Free our temporary variable

    return output;
}



/**
 * @brief Determines the number of images currently loaded in the engine.
 * 
 * @return The length of ainur.(struct image **images).
 */
size_t image_numLoaded(void) {
    size_t i;
    for(i = 0; ainur.images[i]; i++);
    return i;
}



/**
 * @brief A wrapper function to sort the elements in the ainur.(struct image **images).
 */
void image_qsort(void) {
    if(!ainur.images) { return; }
    qsort( ainur.images, image_numLoaded(), sizeof(struct image *), image_compare );
    return;
}
