/*
 * lkernel_lua.c
 *
 *  Created on: Mar 20, 2015
 *      Author: oceaquaris
 */



#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <string.h>
#include <stdlib.h>

#include "debug.h"
#include "lkernel.h"
#include "ainur.h"


/**
 * External variables.
 */
struct engine ainur;

/**
 * @brief Creates and initializes the 'lkernel' Lua state.
 *
 * @return 0: successful execution
 *         exit()s on program failure, for safety reasons
 */
int lkernel_init()
{
    ainur.lkernel = luaL_newstate();
    if(!(ainur.lkernel)) {
        #ifdef DEBUGGING
        if(debugging) {
            debug_fprintf("lkernel_init() => ainur.lkernel unable to initialize\n");
        }
        #endif /*DEBUGGING*/
        #ifdef VERBOSE
        if(verbose) {
            debug_printf("lkernel_init() => ainur.lkernel unable to initialize\n");
        }
        #endif /*VERBOSE*/

        exit(EXIT_FAILURE);
    }
    luaL_openlibs(ainur.lkernel);
    //atexit(lkernel_close);

    return 0;
}

/**
 * @brief Closes the 'lkernel' Lua states
 *
 * @return 0: on successful execution
 */
int lkernel_close()
{
    lua_close(ainur.lkernel);
    return 0;
}

/**
 * @brief Gives the current version of Lua.
 *
 * @return The version of Lua we are currently using.
 * @note Returned char* needs to be free()ed.
 */
char *lkernel_version()
{
    char *temp = LUA_VERSION;   //Lua version
    char *version;              //heap variable, needs to be freed after use.
    unsigned int size = ( sizeof(char) * strlen(temp) ) + sizeof(char);

    version = (char *)malloc( size );   //allocate memory space

    if(!version) { //if memory allocation failed
        #ifdef DEBUGGING
        if(debugging) {
            debug_fprintf("lkernel_version() => heap char* 'version': %s\n", ERROR_MALLOC);
        }
        #endif /*DEBUGGING*/
        #ifdef VERBOSE
        if(verbose) {
            //print error to screen
            debug_fprintf("lkernel_version() => heap char* 'version': %s\n", ERROR_MALLOC);
        }
        #endif /*VERBOSE*/

        return NULL;    //return 0
    }

    //copy temp string into returned heap string.
    memcpy(version, temp, size);
    return version;     //return heap string
}

/**
 * @brief Gives the current release of Lua.
 *
 * @return The release of Lua we are currently using.
 * @note Returned char* needs to be free()ed.
 */
char *lkernel_release()
{
    char *temp = LUA_RELEASE;   //Lua version (defined in lua.h)
    char *release;              //heap variable, needs to be freed after use.
    unsigned int size = ( sizeof(char) * strlen(temp) ) + sizeof(char);

    release = (char *)malloc( size );   //allocate memory space

    if(!release) { //if the allocation failed
        #ifdef DEBUGGING //only compile this section if we want to generate log files
        if(debugging) { //write errors
            debug_fprintf("lkernel_release() => heap char* 'release': %s\n", ERROR_MALLOC);
        }
        #endif /*DEBUGGING*/
        #ifdef VERBOSE
        if(verbose) {
            //print the error to screen
            debug_printf("lkernel_release() => heap char* 'release': %s\n", ERROR_MALLOC);
        }
        #endif /*VERBOSE*/
        return NULL;    //return NULL (0)
    }

    //copy string into allocated heap space
    memcpy(release, temp, size);
    return release; //return the string
}

/**
 * @brief Gives Lua copyright info.
 *
 * @return A copyright statement for the Lua we are currently using.
 * @note Returned char* needs to be free()ed.
 */
char *lkernel_copyright()
{
    char *temp = LUA_COPYRIGHT;     //as defined in lua.h
    char *copyright;                //to become a heap variable (free() after use)
    unsigned int size = (sizeof(char) * strlen(temp) + sizeof(char));

    copyright = (char *)malloc( size );

    if(!copyright) {
        #ifdef DEBUGGING //only compile this section if we're generating log files
        if(debugging) {
            debug_fprintf("lkernel_copyright() => heap char* 'copyright': %s\n", ERROR_MALLOC);
        }
        #endif /*DEBUGGING*/
        #ifdef VERBOSE
        if(verbose) {
            //print error to output screen
            debug_printf("lkernel_copyright() => heap char* 'copyright': %s\n", ERROR_MALLOC);
        }
        #endif /*VERBOSE*/

        return NULL;
    }

    //copy data into heap variable
    memcpy(copyright, temp, size);
    return copyright;   //return our string
}

/**
 * @brief Gives the authors of Lua.
 *
 * @return The authors who wrote the Lua script language we are currently using.
 * @note Returned char* needs to be free()ed.
 */
char *lkernel_authors()
{
    char *temp = LUA_AUTHORS;   //found in lua.h
    char *authors;              //to become a heap variable
    unsigned int size = (sizeof(char) * strlen(temp) + sizeof(char)); //string length +1, for null terminator

    authors = (char *)malloc( size );

    if(!authors) {
        #ifdef DEBUGGING
        if(debugging) {
            debug_fprintf("lkernel_author() => heap char* 'authors': %s\n", ERROR_MALLOC);
        }
        #endif /* DEBUGGING */
        #ifdef VERBOSE
        if(verbose) {
            debug_printf("lkernel_author() => heap char* 'authors': %s\n", ERROR_MALLOC);
        }
        #endif /*VERBOSE*/

        return NULL;
    }

    //copy memory
    memcpy(authors, temp, size);
    return authors;     //return the string
}
