/*
 * lkernel_lua.h
 *
 *  Created on: Mar 20, 2015
 *      Author: oceaquaris
 */

#ifndef LKERNEL_H_
#define LKERNEL_H_

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#define LUA_SUCCESS 1
#define LUA_FAILURE 0

/**
 * Function declarations.
 */
char *lkernel_version(void);
char *lkernel_release(void);
char *lkernel_copyright(void);
char *lkernel_authors(void);
int lkernel_init(void);
void lkernel_close(void);


#endif /* LKERNEL_H_ */
