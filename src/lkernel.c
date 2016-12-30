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
 * @return LUA_SUCCESS: successful execution
 *         exit()s on program failure, for safety reasons
 */
int lkernel_initLua(void)
{
    ainur.lkernel = luaL_newstate();
    if( !(ainur.lkernel) ) {
        #if defined(DEBUGGING) || defined(VERBOSE)
        debug_print("lkernel_init() => ainur.lkernel unable to initialize\n");
        #endif /*defined DEBUGGING || defined VERBOSE*/

        //TODO: error message print on terminal (non-debug)
        
        exit(EXIT_FAILURE); //exit program; execute close protocols, etc.
    }
    luaL_openlibs(ainur.lkernel);   //open lua libraries

    return LUA_SUCCESS; //successful
}

/**
 * @brief Closes the 'lkernel' Lua states
 */
void lkernel_close(void)
{
    lua_close(ainur.lkernel);
    return;
}

/**
 * @brief Gives the current version of Lua.
 *
 * @return The version of Lua we are currently using.
 * @note Returned char* needs to be free()ed.
 */
char *lkernel_version(void)
{
    char *temp = LUA_VERSION;   //Lua version
    char *version;              //heap variable, needs to be freed after use.
    unsigned int size = ( sizeof(char) * (strlen(temp)+1) );

    //allocate memory; check if allocation succeeded
    if( !(version = (char *)malloc( size )) ) {
        #if defined(DEBUGGING) || defined(VERBOSE)
        debug_print("lkernel_version() => heap char* 'version': %s\n", ERROR_MALLOC);
        #endif /*defined DEBUGGING || defined VERBOSE*/
        
        return version; //aka NULL
    }

    //copy temp string into heap string; return heap string
    return memcpy(version, temp, size);
}

/**
 * @brief Gives the current release of Lua.
 *
 * @return The release of Lua we are currently using.
 * @note Returned char* needs to be free()ed.
 */
char *lkernel_release(void)
{
    char *temp = LUA_RELEASE;   //Lua version (defined in lua.h)
    char *release;              //heap variable, needs to be freed after use.
    unsigned int size = ( sizeof(char) * (strlen(temp)+1) );

    //allocate memory; check if allocation succeeded
    if( !(release = (char *)malloc( size )) ) {
        #if defined(DEBUGGING) || defined(VERBOSE)
        debug_print("lkernel_release() => heap char* 'release': %s\n", ERROR_MALLOC);
        #endif /*defined DEBUGGING || defined VERBOSE*/
        
        return release; //aka NULL
    }

    //copy string into allocated heap space; return the string
    return memcpy(release, temp, size);
}

/**
 * @brief Gives Lua copyright info.
 *
 * @return A copyright statement for the Lua we are currently using.
 * @note Returned char* needs to be free()ed.
 */
char *lkernel_copyright(void)
{
    char *temp = LUA_COPYRIGHT;     //as defined in lua.h
    char *copyright;                //to become a heap variable (free() after use)
    unsigned int size = ( sizeof(char) * (strlen(temp)+1) );

    //allocate memory; check if allocation succeeded
    if( !(copyright = (char *)malloc( size )) ) {
        #if defined(DEBUGGING) || defined(VERBOSE)
        debug_print("lkernel_copyright() => heap char* 'copyright': %s\n", ERROR_MALLOC);
        #endif /*defined DEBUGGING || defined VERBOSE*/

        return copyright;   //aka NULL
    }

    //copy string into allocated heap space; return the string
    return memcpy(copyright, temp, size);
}

/**
 * @brief Gives the authors of Lua.
 *
 * @return The authors who wrote the Lua script language we are currently using.
 * @note Returned char* needs to be free()ed.
 */
char *lkernel_authors(void)
{
    char *temp = LUA_AUTHORS;   //found in lua.h
    char *authors;              //to become a heap variable
    unsigned int size = ( sizeof(char) * (strlen(temp)+1) );

    //allocate memory; check if allocation succeeded
    if( !(authors = (char *)malloc( size )) ) {
        #if defined(DEBUGGING) || defined(VERBOSE)
        debug_print("lkernel_author() => heap char* 'authors': %s\n", ERROR_MALLOC);
        #endif /*defined DEBUGGING || defined VERBOSE*/

        return authors; //aka NULL
    }

    //copy string into allocated heap space; return the string
    return memcpy(authors, temp, size);
}
