/*
 * mem.c
 *
 *  Created on: Mar 20, 2015
 *      Author: oceaquaris
 */


#include <stdlib.h>
#include <stdarg.h>

#include "debug.h"
#include "mem.h"

/**
 * @brief free()s a list of pointers
 *
 * @param argc
 *        Number of pointers following the 'argc' parameter
 * @param ...
 *        Variable argument list; arguments treated as type (void *)
 */
void mem_free(unsigned int argc, ...)
{
    if(!argc) { //if argc == 0
        dbgprintf("mem_free() => formal param \'argc\': Value less than 1\n");

        return;
    }

    va_list args;
    va_start(args, argc);
    int i;
    void *temp;
    for(i = 0; i < argc; i++) {
        temp = va_arg(args, void *);
        free(temp);
    }
    va_end(args);
    return;
}
