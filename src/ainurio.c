/*
 * input.c
 *
 *  Created on: Apr 17, 2015
 *      Author: oceaquaris
 */

#include "ainurio.h"

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>

#include "debug.h"


/**
 * @brief Receives input from keystrokes through SDL and executes appropriate tasks.
 *
 * @return 0: program success
 */
int ainurio_SDLreceive()
{
    SDL_Event event;

    while(SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_QUIT:
                exit(0);
                break;

            case SDL_KEYDOWN:
                switch(event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        exit(0);
                        break;
                    default:
                        break;
                }
                break;

            default:
                break;

        }
    }

    return 0;
}


/**
 * @brief Concatenates a list (of length argc) of strings together and returns
 *        the string as a heap variable.
 *        @note Naming: lISThEAPstrINGCONcatENATE
 *
 * @param argc
 *        Number of strings to concatenate (follow in list)
 * @param ...
 *        Variable amount of arguments
 *
 * @return A string with all concatenated strings.
 *
 * @note Returned string needs to be free()ed; it is a heap variable.
 * @note Does NOT free() strings fed into function as arguments.
 */
char *ainurio_lhstrcat(unsigned int argc, ...)
{
    if(!argc) { //if argc == 0
        #ifdef DEBUGGING
        if(debugging) {
            debug_fprintf("ainurio_strlcat() => formal param 'argc': Value less than 1\n");
        }
        #endif /* DEBUGGING */
        #ifdef VERBOSE
        if(verbose) {
            debug_printf("ainurio_strlcat() => formal param 'argc': Value less than 1\n");
        }
        #endif /*VERBOSE*/

        return NULL;
    }

    va_list args;
    va_start(args, argc);

    int i, collective_length = 0;
    char *temp[argc]; //will store pointers to all the arguments

    //copies pointers to the arguments into an array.
    for(i = 0; i < argc; i++) {
        temp[i] = va_arg(args, char *);
    }
    va_end(args);

    //add up the lengths of all the strings
    for(i = 0; i < argc; i++) {
        collective_length += strlen(temp[i]);
    }

    char *output;
    //size the string to length of all strings, plus null terminator.
    output = (char *)malloc( (collective_length * sizeof(char)) + sizeof(char) );
    if(!output) {
        #ifdef DEBUGGING
        if(debugging) {
            debug_fprintf("ainurio_strlcat() => heap char* \'output\': %s\n", ERROR_MALLOC);
        }
        #endif /*DEBUGGING*/
        #ifdef VERBOSE
        if(verbose) {
            debug_printf("ainurio_strlcat() => heap char* \'output\': %s\n", ERROR_MALLOC);
        }
        #endif /*VERBOSE*/

        return NULL;
    }

    memcpy(output, temp[0], strlen(temp[0])+1 ); //copy first string into heap variable
    //copy remaining strings into heap variable
    for(i = 1; i < argc; i++) {
        strcat(output, temp[i]);
    }//output is still null terminated...


    return output;
}


/**
 * @brief Reallocates storage space for a destination string and concatenates a
 *        source string onto the destination string.
 *        @note Naming: sINGULARhEAPstrINGCONcatENATE
 *
 * @param destination
 *        Destination string to concatenate source onto.
 * @param source
 *        Source string to concatenate onto destination.
 *
 * @return A concatenated string.
 *
 * @note Returned string needs to be free()ed; it is a heap variable.
 * @note Does not free either the source nor the destination
 */
char *ainurio_shstrcat(char *destination, const char *source)
{
    #if defined DEBUGGING || defined VERBOSE
    if(!destination) {
        #ifdef DEBUGGING
        if(debugging) {
            debug_fprintf("ainurio_shstrcat() => formal param \'destination\': %s\n", ERROR_NULL_POINTER);
            debug_fprintf("                      Continuing on using NULL pointer in realloc()...");
        }
        #endif /*DEBUGGING*/
        #ifdef VERBOSE
        if(verbose) {
            debug_printf("ainurio_shstrcat() => formal param \'destination\': %s\n", ERROR_NULL_POINTER);
            debug_printf("                      Continuing on using NULL pointer in realloc()...");
        }
        #endif /*VERBOSE*/
    }
    if(!source) {
        #ifdef DEBUGGING
        if(debugging) {
            debug_fprintf("ainurio_shstrcat() => formal param \'source\': %s\n", ERROR_NULL_POINTER);
            debug_fprintf("                      Continuing on with realloc()...");
        }
        #endif /*DEBUGGING*/
        #ifdef VERBOSE
        if(verbose) {
            debug_printf("ainurio_shstrcat() => formal param \'source\': %s\n", ERROR_NULL_POINTER);
            debug_printf("                      Continuing on with realloc()...");
        }
        #endif /*VERBOSE*/
    }
    #endif /*defined DEBUGGING || defined VERBOSE*/
    if(!destination && !source) {
        #ifdef DEBUGGING
        if(debugging) {
            debug_fprintf("ainurio_shstrcat() => formal params \'destination\' && \'source\': %ss\n", ERROR_NULL_POINTER);
        }
        #endif /*DEBUGGING*/
        #ifdef VERBOSE
        if(verbose) {
            debug_printf("ainurio_shstrcat() => formal params \'destination\' && \'source\': %ss\n", ERROR_NULL_POINTER);
        }
        #endif /*VERBOSE*/

        return NULL;
    }

    //determine the lengths of the strings.
    unsigned int dest = strlen(destination),
                 src  = strlen(source);

    //reallocate memory
    destination = (char *)realloc(destination, dest + src + sizeof(char));
    if(!destination) { //check to see if allocation succeeded
        #ifdef DEBUGGING
        if(debugging) {
            debug_fprintf("ainurio_shstrcat() => heap var \'destination\': %s\n", ERROR_REALLOC);
        }
        #endif /*DEBUGGING*/
        #ifdef VERBOSE
        if(verbose) {
            debug_printf("ainurio_shstrcat() => heap var \'destination\': %s\n", ERROR_REALLOC);
        }
        #endif /*VERBOSE*/

        return NULL;
    }

    strcat(destination, source); //concatenate the two strings

    return destination; //give us back the destination string!
}
