/*
 * lkernel_image.c
 *
 *     Created on: 9 July 2017
 *         Author: oceaquaris
 *  Last Modified:
 *
 * Field Overview:
 *  Static:
 *      lkernel_image_load
 *  Extern:
 *      lkernel_image_init
 */

#include <lauxlib.h>
#include <lua.h>

#include "image.h"
#include "lkernel_image.h"

static int lkernel_image_load(lua_State *L);
static const luaL_Reg lkernel_image_functions[] = {
    {"load", lkernel_image_load},
    {NULL, NULL}
};

static int lkernel_image_load(lua_State *L) {
    //pointers to the image filename and the tag associated with the image
    const char *filename = NULL,
               *tag = NULL;

    //retrieve the first two arguments
    filename = luaL_checkstring(L, 1);
    tag = luaL_checkstring(L, 2);

    lua_pushlightuserdata(L, image_load(filename, tag));
    return 1;
}



int lkernel_image_init(lua_State *L) {
    luaL_openlib(L, "image", lkernel_image_functions, 0);
    return 1;
}
