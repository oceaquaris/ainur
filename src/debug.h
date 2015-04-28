/**
 * @file debug.h
 *
 *  Created on: Mar 12, 2015
 *      Author: oceaquaris
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#include <stdio.h>
#include <time.h>

/* Master switch to include debug protocols. */
#define DEBUGGING

/* Switch to cut down on error messages. Can be deactivated to silence many debug_printf messages*/
#define VERBOSE

/* Template for debug code
#ifdef DEBUGGING
if(debugging) {
	debug_fprintf("");
}
#endif /*DEBUGGING
#ifdef VERBOSE
if(verbose) {
	debug_printf("");
}
#endif /*VERBOSE
 */

/**
 * @brief External variables associated with the log-book
 * @note Affected by DEBUGGING preprocessor option
 */
#ifdef DEBUGGING
extern int debugging;
extern const char *logbook_name;
extern FILE *debug_logbook;
extern time_t start_time;       //for printing time stamps
#endif /*DEBUGGING*/
#ifdef VERBOSE
extern int verbose;
#endif /*VERBOSE*/

/**
 * Function declarations.
 */
int debug_printf(const char *format, ...);      //analogous to printf

#ifdef DEBUGGING
int debug_fprintf(const char *format, ...);     //analogous to fprintf
int debug_fputc(int character);                 //analogous to fputc
#endif /*DEBUGGING*/

/**
 * @brief Error messages.
 */
extern const char *ERROR_MALLOC;
extern const char *ERROR_CALLOC;
extern const char *ERROR_REALLOC;
extern const char *ERROR_FREE;
extern const char *ERROR_NO_FILE;
extern const char *ERROR_NULL_FILE;
extern const char *ERROR_NULL_POINTER;
extern const char *ERROR_NULL_STRING;
extern const char *ERROR_NULL_SDL_SURFACE;

#endif /* DEBUG_H_ */
