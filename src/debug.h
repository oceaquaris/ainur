/**
 * @file debug.h
 *
 *  Created on: Mar 12, 2015
 *      Author: oceaquaris
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#include <stdarg.h>
#include <stdio.h>
#include <time.h>

/* Master switch to include debug protocols. */
#define DEBUGGING

/* Switch to cut down on error messages. Can be deactivated to silence many debug_printf messages*/
#define VERBOSE

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


/**
 * Function declarations.
 */
int debug_printf(const char *format, ...);      //analogous to printf
int debug_vprintf(const char *format, va_list args);

#if defined(DEBUGGING) || defined(VERBOSE)
int debug_print(const char *format, ...);
#endif

#ifdef DEBUGGING
void debug_debugOn(void);
void debug_debugOff(void);
int debug_getDebugStatus();
int debug_fprintf(const char *format, ...);     //analogous to fprintf
int debug_vfprintf(const char *format, va_list args);
//int debug_fputc(int character);                 //analogous to fputc
#endif /*DEBUGGING*/

#ifdef VERBOSE
void debug_verboseOn(void);
void debug_verboseOff(void);
int debug_getVerboseStatus(void);
#endif


#endif /* DEBUG_H_ */
