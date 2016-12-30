/**
 * @file palette.c
 * 
 * Created: 2015-12-31
 * Author: oceaquaris
 * 
 * @brief
 */

#include <stdlib.h>

#include "palette.h"

struct palette *palette_create(void){
    struct palette *palette;
    if( !(palette = (struct palette *)malloc(sizeof(palette))) ) {
        printf("Unable to allocate mem.\n");
    }
    return palette;
}
