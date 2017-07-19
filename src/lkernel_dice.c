/*
 * lkernel_image.c
 *
 *     Created on: 9 July 2017
 *         Author: oceaquaris
 *  Last Modified:
 *
 * Field Overview:
 *  Static:
 *      lkernel_dice_average
 *      lkernel_dice_roll
 *      lkernel_dice_roll_numeric
 *      lkernel_dice_valid
 *  Extern:
 *      lkernel_image_init
 */

#include <lauxlib.h>
#include <lua.h>

#include "dice.h"
#include "lkernel.h"
#include "lkernel_dice.h"



static int lkernel_dice_average(lua_State *L);
static int lkernel_dice_roll(lua_State *L);
//probably will remove lkernel_dice_roll_numeric and use lkernel_dice_roll instead
static int lkernel_dice_roll_numeric(lua_State *L);
static int lkernel_dice_valid(lua_State *L);
static const luaL_Reg lkernel_dice_functions[] = {
    {"average", lkernel_dice_average},
    {"roll", lkernel_dice_roll},
    {"rollNumeric", lkernel_dice_roll_numeric},
    {"valid", lkernel_dice_valid},
    {NULL, NULL}
};



static int lkernel_dice_average(lua_State *L) {
    //pointer to the dice format string
    const char *fmt = luaL_checkstring(L, 1);

    //push the result
    lua_pushnumber(L, dice_average(fmt));

    return 1;
}



/**
 * 
 * dice.roll()
 * dice.roll(fmt)
 * dice.roll(num, faces, bias)
 */
static int lkernel_dice_roll(lua_State *L) {
    //check the number of arguments
    int argc = lua_gettop(L);

    if (argc == 0) {
        lua_pushnumber(L, dice_roll_numeric(1, 6, 0)); //roll a 6-sided dice
        return 1;
    }
    else if (argc == 1) {
        //pointer to the dice format string
        const char *fmt = luaL_checkstring(L, 1);

        //push the result
        lua_pushnumber(L, dice_roll(fmt));

        return 1;
    }
    else if (argc == 3) {
        int num = luaL_checkinteger(L, 1),
            faces = luaL_checkinteger(L, 2),
            bias = luaL_checkinteger(L, 3);

        lua_pushnumber(L, dice_roll_numeric(num, faces, bias));
        return 1;
    }

    LKERNEL_INVALID_PARAMETER(L);
    return 0;
}



static int lkernel_dice_roll_numeric(lua_State *L) {
    int num = luaL_checkinteger(L, 1),
        faces = luaL_checkinteger(L, 2),
        bias = luaL_checkinteger(L, 3);

    lua_pushnumber(L, dice_roll_numeric(num, faces, bias));

    return 1;
}



static int lkernel_dice_valid(lua_State *L) {
    const char *fmt = luaL_checkstring(L, 1);

    lua_pushnumber(L, dice_valid(fmt));
    return 1;
}



int lkernel_dice_init(lua_State *L) {
    luaL_openlib(L, "dice", lkernel_dice_functions, 0);
    return 1;
}
