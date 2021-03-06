/*
 * image.c
 *
 *     Created on: 21 April 2015
 *         Author: oceaquaris
 *  Last Modified: 26 May 2017
 *
 * Field Overview:
 *  static:
 *      image_compare_bsearch
 *      image_compare_qsort
 *  extern:
 *      image_bsearch
 *      image_close
 *      image_dump
 *      image_dumpAll
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
 * @brief Wrapper function that performs a comparison on a key and the tag
 *        of a struct image *.
 *
 * @param pkey
 *        A pointer to a (char *) to be compared (the key).
 * @param pelem
 *        A pointer to a (struct image *) to be compared (the element).
 *
 * @return < 0: pkey has a lower 'tag' string value than pelem.
 *         = 0: pkey and pelem have the same 'tag' label.
 *         > 0: pkey has a higher 'tag' string value than pelem.
 */
static int image_compare_bsearch(const void *pkey, const void *pelem) {
    return strcmp( ((char *)pkey), ((struct image **)pelem)[0]->tag );
}



/**
 * @brief Wrapper function that performs a comparison on the 'tag' strings
 *        in two struct image *'s.
 *
 * @param p1
 *        A pointer to a (char *) to be compared (the key).
 * @param p2
 *        A pointer to a (struct image *) to be compared (the element).
 *
 * @return < 0: p1 has a lower 'tag' string value than p2.
 *         = 0: p1 and p2 have the same 'tag' label.
 *         > 0: p1 has a higher 'tag' string value than p2.
 */
static int image_compare_qsort(const void *p1, const void *p2) {
    return strcmp( (*((struct image **)p1))->tag, (*((struct image **)p2))->tag );
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
                                     image_compare_bsearch );
}



/**
 * @brief Wrapper function to perform cleanup protocols for image
 *        functionalities.
 * @note Function calls in this wrapper function are order dependent.
 */
void image_close(void) {
    image_freeAll();    //function calls are order dependent
    IMG_Quit();
    return;
}



/**
 * @brief Dump information about an image to a file.
 *
 * @param stream
 *        The FILE to dump the text to.
 * @param image
 *        The struct image* to examine.
 */
int image_dump(FILE *stream, struct image *image) {
    return fprintf(stream,
                   "struct image *: %p\n"\
                   "    tag = %s\n"\
                   "    surface = %p\n"\
                   "    filename = %s\n",
                   image,
                   image->tag,
                   image->surface,
                   image->filename);
}



/**
 * @brief Dump information about all loaded images to a file.
 *
 * @param stream
 *        The FILE to dump the text to.
 *
 * @return The total number of characters written (similar to fprintf).
 */
int image_dumpAll(FILE *stream) {
    register int sum = 0, i;
    size_t len = image_numLoaded();

    sum += fprintf(stream, "struct image **: %p\n"\
                           "  length = %lu\n"\
                           "  contents = {\n", ainur.images, len);
    for(i = 0; i < len; i++) {
        sum += fprintf(stream,
                       "    struct image *: %p\n"\
                       "        tag = \"%s\"\n"\
                       "        surface = %p\n"\
                       "        filename = \"%s\"\n",
                       ainur.images[i],
                       ainur.images[i]->tag,
                       ainur.images[i]->surface,
                       ainur.images[i]->filename);
    }
    sum += fprintf(stream, "    %p\n}\n", NULL);

    return sum;
}



/**
 * @brief Free an image struct.
 *
 * @param image
 *        Pointer to image struct to free.
 */
void image_free(struct image *image) {
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

    //if tag is null or tag image is not found
    if(!tag || !(image = image_bsearch(tag)) ) {
        return;//do nothing
    }

    size_t length = image_numLoaded();     //find the number of elements in 'images'

    image_free(image[0]);   //free the image that we found.

    image[0] = ainur.images[length - 1];  //copy a reference to the last image in 'images' where our freed image was.
    ainur.images[length - 1] = NULL;      //make the last reference in 'images' NULL

    //attempt to resize the 'images' array
    if( !( ainur.images = realloc(ainur.images, length * (sizeof(struct image *)) ) ) ) {
        dbgprint("image_freeTag:\n"\
                 "    Unable to reallocate enough memory to resize static struct images **images.\n");

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
        dbgprint("image_init: IMG_Init error: Failed to initialize required png support.\n"\
                 "            IMG_Init error: %s\n", IMG_GetError());

        exit(EXIT_FAILURE); //close program and free everything.
    }

    //attempt to allocate memory for the 'images' array
    if( !(ainur.images = malloc(sizeof(struct image *))) ) {
        dbgprint("image_init: IMG_Init error: Unable to allocate memory for struct tile **tiles.\n");

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
    //'filename' and 'tag' cannot be NULL!!!
    if(!filename || !tag) {
        dbgprint("image_load: Unable to load image: %s.\n"\
                 "            formal params 'filename' and 'tag' must be non-NULL\n"\
                 "            filename = \"%s\"\n"\
                 "            tag = \"%s\"\n", filename, filename, tag);

        return NULL;
    }

    //check to see if the file exists
    if(!file_exists(filename)) {
        dbgprint("image_load: Unable to load image: %s.\n"\
                 "            %s\n", filename, ERROR_NO_FILE);

        return NULL;
    }

    //'tag' must be unique!
    if( image_bsearch(tag) ) {
        dbgprint("image_load: Unable to load image: %s.\n"\
                 "            Associated tag, \"%s\", is not unique.\n",
                 filename, tag);

        return NULL;
    }

    //first, attempt to load an SDL_Surface.
    SDL_Surface *surface = image_loadSDL_Surface(filename);
    if(!surface) {
        dbgprint("image_load: Unable to load image: %s.\n", filename);

        return NULL;
    }

    //second, attempt to create a new struct image.
    struct image *load = NULL;

    //attempt to allocate memory for the struct image
    if( !(load = malloc( sizeof(struct image) ))  ) {
        dbgprint("image_load: Unable to allocate enough memory for new struct image: %s.\n", tag);

        return load; //aka NULL
    }

    //set the surface inside the image struct
    load->surface = surface;

    //'length' will store the lengths of 'tag' and 'filename'
    size_t length = strlen(tag);

    //attempt to allocate memory for the (char *) field 'tag' in the struct image
    if ( !(load->tag = malloc( sizeof(char) * (length + 1) )) ) {
        dbgprint("image_load: Unable to allocate enough memory for (%s)->tag.\n", tag);

        image_free(load);   //free up memory
        return NULL;
    }

    //copy 'tag' to load->tag
    memcpy( load->tag, tag, sizeof(char) * (length + 1) );

    //reassign the length to that of 'filename'
    length = strlen(filename);

    //attempt to allocate memory for the (char *) field 'filename' in the struct image
    if( !(load->filename = malloc( sizeof(char) * (length + 1) )) ) {
        dbgprint("image_load: Unable to allocate enough memory for (%s)->filename.\n", tag);

        image_free(load);   //free up memory
        return NULL;
    }

    //copy 'filename' to load->filename
    memcpy( load->filename, filename, sizeof(char) * (length + 1) );

    //third, attempt to extend the length of the statically allocated struct image **images
    int numloads = image_numLoaded();
    if ( !( ainur.images = realloc(ainur.images, (numloads + 2) * (sizeof(struct image *) ) ) ) ) {
        dbgprint("image_load: Unable to reallocate enough memory to resize ainur.(struct images **images).\n");

        free(load); //free up memory
        return NULL;
    }

    ainur.images[numloads] = load;        //insert new image into array
    ainur.images[numloads + 1] = NULL;    //attach new NULL terminator

    image_qsort();  //sort the array for future use.

    return load;    //return a pointer to the newly created and archived struct image.
}



/**
 * @brief Loads multiple images with their tags.
 *
 * @param argc
 *        Argument count, must be divisible by 2 or function will terminate.
 * @param ...
 *        Variable args list. Input pair format is filename, tag, ...
 */
int image_loadMultiple(unsigned int argc, ...) {
    if(!(argc % 2)) { //if argc is not divisible by zero
        dbgprint("image_loadMultiple: formal param 'argc' is not divisible by 2.\n"\
                 "    'argc' must be divisible by 2.\n"\
                 "    argc = %u\n"
                 "    Returning 0.\n", argc);

        return 0;
    }

    va_list args;
    va_start(args, argc);

    int i, pairs = argc / 2;
    char *filename, *tag;
    for(i = 0; i < pairs; i++) {
        filename = va_arg(args, char *);
        tag = va_arg(args, char *);

        /*
        if(!filename || !tag) {
            dbgprint("image_loadMultiple: An image pair contains a NULL\n"\
                     "    filename = %s\n"\
                     "    tag = %s\n"\
                     "    Skipping pair.", filename, tag);

            continue;
        }

        if(!file_exists(filename)) {
            dbgprint("image_loadMultiple: %s: %s\n"\
                     "    Skipping pair: \"%s\", \"%s\"\n",
                     filename, ERROR_NO_FILE,
                     filename, tag);

            continue;
        }

        if(image_bsearch(tag)) {
            dbgprint("image_loadMultiple: 'tag' \"%s\" is not unique.\n"\
                     "    Skipping pair: \"%s\", \"%s\"\n",
                     tag, filename, tag);

            continue
        }
        */
        image_load(filename, tag);
    }
    va_end(args);

    return pairs;
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
        dbgprint("image_loadSDL_Surface: formal param 'filename'(%s): %s\n", filename, ERROR_NO_FILE);

        return NULL;
    }

    SDL_Surface *temp = IMG_Load(filename);     //Load an SDL_Surface using the filename given.

    if(!temp) {
        dbgprint("image_loadSDL_Surface: IMG_Load error: %s\n", IMG_GetError());

        return temp;    //aka NULL
    }

    SDL_Surface *output = NULL;    //Declare an SDL_Surface (for output by function)

    /*SDL_SetColorKey(temp,
                    SDL_TRUE,
                    SDL_MapRGB(temp->format, 0, 0, 0)   ); // Make the background transparent */

    /*output = SDL_ConvertSurfaceFormat(temp,
                                      SDL_PIXELTYPE_UNKNOWN,
                                      0 ); *//* Convert the image to the screen's native format */

    output = SDL_ConvertSurface(temp, SDL_GetWindowSurface(ainur.screen)->format, 0);

    if(!output) {
        dbgprint("image_loadSDL_Surface: local var 'output': %s\n"\
                 "            SDL error: %s\n", ERROR_NULL_SDL_SURFACE, SDL_GetError() );

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
    qsort( ainur.images, image_numLoaded(), sizeof(struct image *), image_compare_qsort );
    return;
}
