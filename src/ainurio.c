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


//static functions
static void ainurio_reset_keylog();
//static variable needed for keylog
static int keys_pressed[AINURIO_MAX_SDLRECEIVE];


static void ainurio_reset_keylog()
{
	int i;
	for(i = 0; i < AINURIO_MAX_SDLRECEIVE; i++) {
		keys_pressed[i] = 0; //set everything to 0
	}
	return;
}

/**
 * @brief Receives input from keystrokes through SDL and executes appropriate tasks.
 *
 * @return 0: program success
 */
int ainurio_SDLreceive()
{
    SDL_Event event;
    int index = 0;

    ainurio_reset_keylog();

    while(SDL_PollEvent(&event) && index < AINURIO_MAX_SDLRECEIVE) {
        switch(event.type) {
            case SDL_QUIT:
                exit(0);
                break;

            case SDL_KEYDOWN:
				keys_pressed[index] = event.key.keysym.sym; //stash keystrokes in array
                /*switch(event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        exit(0);
                        break;
                    default:
                        break;
                }*/
                break;

            default:
                break;

        }
    }

    return 0;
}

/**
 * @brief Runs through keys_pressed array and executes code based on which keys
 *        were pressed.
 */
void ainurio_interpretKey()
{
	int i;
	//make sure loop doesn't exceed array's length && the element isn't '0'
	for(i = 0; i < AINURIO_MAX_SDLRECEIVE && keys_pressed[i]; i++) {
		//TODO: convert from else if to non-testing array based solution
		if(keys_pressed[i] == SDLK_ESCAPE) {
			exit(0); //exit the program
		}
	}
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


/**
 * @brief Receives input from the keyboard (stdin as a FILE) and outputs it as a string.
 *
 * @param inputstream
 *        FILE to be read (stdin in most cases)
 * @param size
 *        Initial size of the output string to be allocated (for internal use).
 *
 * @return A null terminated string from input (input is terminated by carriage return).
 * @note Return string needs to be free()ed; it is a heap variable.
 */
char *ainurio_rawInput(FILE *input, unsigned int size)
{
	//check to see if 'input' is valid
	if(!input) {
		#ifdef DEBUGGING
        if(debugging) {
            debug_fprintf("ainurio_rawInput() => formal param \'input\': %s\n", ERROR_NULL_POINTER);
        }
        #endif /*DEBUGGING*/
        #ifdef VERBOSE
        if(verbose) {
            debug_printf("ainurio_rawInput() => formal param \'input\': %s\n", ERROR_NULL_POINTER);
        }
        #endif /*VERBOSE*/

        return NULL;
	}

	char *string;		//string to be dynamically sized.
	int character;
	unsigned int length = 0;

	//allocate data for 'string' with 'size' bytes of data
	string = (char *)realloc(NULL, sizeof(char) * size);

	if(!string) { //if string is still NULL
		#ifdef DEBUGGING
        if(debugging) {
            debug_fprintf("ainurio_rawInput() => local char* \'string\': %s\n", ERROR_REALLOC);
        }
        #endif /*DEBUGGING*/
        #ifdef VERBOSE
        if(verbose) {
            debug_printf("ainurio_rawInput() => local char* \'string\': %s\n", ERROR_REALLOC);
        }
        #endif /*VERBOSE*/

        return string; //aka NULL
	}

	/* Loop through the input FILE until it encounters end of file or newline character. */
	while(EOF != (character = fgetc(input)) && character != '\n') {
		string[length++] = character;	//copy first character; increment index 'length'
		if(length == size) {
			//allocate more space in 16 byte chunks
			string = (char *)realloc( string, sizeof(char)*(size += 16) );
			if(!string) {
				#ifdef DEBUGGING
				if(debugging) {
					debug_fprintf("ainurio_rawInput() => heap char* \'string\': %s\n", ERROR_REALLOC);
					debug_fprintf("                      Error expanding \'string\'\n");
				}
				#endif /*DEBUGGING*/
				#ifdef VERBOSE
				if(verbose) {
					debug_printf("ainurio_rawInput() => heap char* \'string\': %s\n", ERROR_REALLOC);
					debug_printf("                      Error expanding \'string\'\n");
				}
				#endif /*VERBOSE*/

				return string; //aka NULL
			}
		}
	}
	string[length++] = '\0';	//end string with null terminator

	return (char *)realloc( string, sizeof(char) * length );	//return a string of precise length
}
