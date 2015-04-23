/*
 * lkernel_lua.h
 *
 *  Created on: Mar 20, 2015
 *      Author: rs14
 */

#ifndef LKERNEL_H_
#define LKERNEL_H_

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

/**
 * Function declarations.
 */
char *lkernel_version();
char *lkernel_release();
char *lkernel_copyright();
char *lkernel_authors();
int lkernel_init();
int lkernel_close();


#endif /* LKERNEL_H_ */
