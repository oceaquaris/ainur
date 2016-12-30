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
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_keycode.h>

#include "debug.h"


//static functions


void ainurio_interpretInput(void) {
    SDL_Event event;
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    
    //MUST USE SCANCODEs for this section!!!
    if(state[SDL_SCANCODE_RETURN]) {
        printf("<RETURN> is pressed.\n");
    }
    if(state[SDL_SCANCODE_RIGHT] && state[SDL_SCANCODE_UP]) {
        printf("Right and Up Keys Pressed.\n");
    }
    if(state[SDL_SCANCODE_LCTRL] || state[SDL_SCANCODE_RCTRL]) {
        printf("Ctrl is pressed.\n");
    }
    

    while( SDL_PollEvent(&event) ) {
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

            //case SDL_MOUSEMOTION:
            default:
                break;

        }
    }
    return;
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
char *ainurio_lhstrcat(unsigned int argc, ...) {
    if(!argc) { //if argc == 0
        #if defined(DEBUGGING) || defined(VERBOSE)
        debug_printf("ainurio_strlcat() => formal param 'argc': Value less than 1\n");
        #endif /*defined(DEBUGGING) || defined(VERBOSE)*/
        
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
        #if defined(DEBUGGING) || defined(VERBOSE)
        debug_printf("ainurio_strlcat() => heap char* \'output\': %s\n", ERROR_MALLOC);
        #endif /*defined(DEBUGGING) || defined(VERBOSE)*/
        
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
 * @param dest_hstr
 *        Destination heap string to concatenate source onto.
 * @param src
 *        Source string to concatenate onto destination.
 *
 * @return A concatenated string.
 *
 * @note Returned string needs to be free()ed; it is a heap variable.
 * @note Does not free either the source nor the destination
 */
char *ainurio_shstrcat(char *dest_hstr, const char *src) {
    #if defined(DEBUGGING) || defined(VERBOSE)
    if(!dest_hstr) {
        debug_print("ainurio_shstrcat() => formal param 'dest_hstr': %s\n"\
                    "                      Continuing on using NULL pointer in realloc()...",
                    ERROR_NULL_POINTER);
    }
    #endif /*defined DEBUGGING || defined VERBOSE*/
    
    if(!src) {
        #if defined(DEBUGGING) || defined(VERBOSE)
        debug_print("ainurio_shstrcat() => formal param 'src': %s\n", ERROR_NULL_POINTER);
        #endif /*defined DEBUGGING || defined VERBOSE*/

        return dest_hstr;
    }

    //determine the lengths of the strings.
    unsigned int ldest = strlen(dest_hstr),
                 lsrc  = strlen(src);

    //reallocate memory and check to see if allocation succeeded
    if( !(dest_hstr = (char *)realloc(dest_hstr, sizeof(char)*(ldest+lsrc+1))) ) {
        #if defined(DEBUGGING) || defined(VERBOSE)
        debug_print("ainurio_shstrcat() => heap var 'dest_hstr': %s\n", ERROR_REALLOC);
        #endif /*defined DEBUGGING || defined VERBOSE*/

        return dest_hstr; //aka NULL
    }

    strcat(dest_hstr, src); //concatenate the two strings

    return dest_hstr;
}


/**
 * @brief Receives input from the keyboard (stdin as a FILE) and outputs it as a string.
 *
 * @param input
 *        FILE to be read (stdin in most cases)
 * 
 * @return A null terminated string from input (input is terminated by carriage return).
 * @note Return string needs to be free()ed; it is a heap variable.
 */
#define RAWINPUT_SIZE   16 //cannot equal zero!!!
char *ainurio_rawInput(FILE *input) {
    if(!input) { //check to see if 'input' is valid
        #if defined(DEBUGGING) || defined(VERBOSE)
        debug_print("ainurio_rawInput() => formal param 'input': %s\n", ERROR_NULL_POINTER);
        #endif /*defined DEBUGGING || defined VERBOSE*/

        return NULL;
    }

    char *str;  //string to be dynamically sized.
    int c,      //character
        size = RAWINPUT_SIZE;   //initial array size length
    unsigned int len = 0;

    //allocate data for 'str' with 'size' bytes of data; and if 'str' is NULL
    if( !(str = (char *)malloc(sizeof(char) * size)) ) {
        #if defined(DEBUGGING) || defined(VERBOSE)
        debug_print("ainurio_rawInput() => local char* 'str': %s\n", ERROR_REALLOC);
        #endif /*defined DEBUGGING || defined VERBOSE*/

        return str; //aka NULL
    }

    /* Loop through the input FILE until it encounters end of file or newline character. */
    while(EOF != (c = fgetc(input)) && c != '\n') {
        str[len++] = c;	//copy first character; increment index 'len'
        if(len == size) {
            //allocate more space in 16 byte chunks; check if allocation succeeded
            if( !(str = (char *)realloc( str, sizeof(char)*(size += 16) )) ) {
                #if defined(DEBUGGING) || defined(VERBOSE)
                debug_print("ainurio_rawInput() => heap char* 'str': %s\n"\
                            "                      Error expanding 'str'\n", ERROR_REALLOC);
                #endif /*defined DEBUGGING || defined VERBOSE*/

                return str; //aka NULL
            }
        }
    }
    str[len++] = '\0';   //end str with null terminator

    return (char *)realloc( str, sizeof(char) * len );    //return a string of precise length
}
