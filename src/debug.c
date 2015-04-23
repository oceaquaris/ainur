/**
 * @file debug.c
 *
 * @brief Contains functions for debugging
 *  Created on: Mar 12, 2015
 *      Author: rs14
 */

#include <stdio.h>
#include <stdarg.h>
#include <time.h>

#include "debug.h"

/**
 * @brief Define various error warning constants.
 * @note  Not affected by DEBUGGING preprocessor options
 */
const char *ERROR_MALLOC            = "Memory allocation failure";
const char *ERROR_CALLOC            = "Memory allocation failure";
const char *ERROR_REALLOC           = "Memory reallocation failure";
const char *ERROR_FREE              = "Heap space freeing failure";
const char *ERROR_NO_FILE           = "No such file or directory";
const char *ERROR_NULL_FILE         = "Null file pointer";
const char *ERROR_NULL_POINTER      = "Null pointer";
const char *ERROR_NULL_STRING       = "Null string";
const char *ERROR_NULL_SDL_SURFACE  = "Null SDL_Surface";


/**
 * @brief External variables associated with the log-book
 * @note  Affected by #ifdef DEBUGGING
 */
#ifdef DEBUGGING //if we want to debug, store space for these variables

int debugging = 0;              //by default, not debugging
FILE *debug_logbook = NULL;     //by default, debug log-book is null
const char *logbook_name = "debug.log";
//time_t start_time = time(NULL);            //

#endif /*DEBUGGING*/

#ifdef VERBOSE
int verbose = 0;     //by default, not verbose
#endif /*VERBOSE*/

/**
 * @brief Writes a message to an output screen (terminal at the moment).
 * @note  Analogous to printf()
 *
 * @param format
 *        A string that contains text to be printed on screen.
 *
 * @return The number of characters written.
 *
 * @note SHOULD NEVER BE PREPROCESSORED OUT (ie: #ifdef ... #endif)!!!
 */
int debug_printf(const char *format, ...)
{
    int output;
    va_list args;
    va_start(args, format);

    output = vprintf(format, args);  //meat and potatoes of the function.

    va_end(args);
    return output;
}

/**
 * @brief Writes a message to debug_logbook (see debug.h)
 *
 * @param format
 *        A string that contains the text to print the debugging log-book.
 *
 * @return The number of characters printed.
 * @note Affected by debugging preprocessor
 */
#ifdef DEBUGGING //if we want to debug compile
int debug_fprintf(const char *format, ...)
{
    int output; //stores the value returned from vfprintf

    //start the va_list argument list using
    va_list args;
    va_start(args, format); //list starts at const char *format

    //open shared file with name 'logbook_name': append settings
    debug_logbook = fopen(logbook_name, "a+");
    //transfer parameters to vfprintf from debug_fprintf parameters
    output = vfprintf(debug_logbook, format, args);

    //end argument list
    va_end(args);

    //close file for safety.
    fclose(debug_logbook);

    //return output from the vfprintf function
    return output;
}
#endif /*DEBUGGING*/

/**
 * @brief Writes a single character to debug_logbook (see debug.h)
 *
 * @param character
 *        The int promotions of the character to be written. The value is converted
 *        to an unsigned char in the internal working of fputc()
 * @note Affected by preprocessor (DEBUGGING option)
 */
#ifdef DEBUGGING //found in debug.h
int debug_fputc(int character)
{
    int output;

    //open shared file with name 'logbook_name': append settings
    debug_logbook = fopen(logbook_name, "a+");
    //print the char with fputc
    output = fputc(character, debug_logbook);
    //close file for safety
    fclose(debug_logbook);

    return output;
}
#endif /*DEBUGGING*/
