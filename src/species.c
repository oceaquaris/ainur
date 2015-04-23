/*
 * species.c
 *
 *  Created on: Mar 23, 2015
 *      Author: rs14
 */


#include <stdlib.h>

#include "species.h"
#include "mem.h"


struct species *species_create(char *tag,
                               char *name,
                               unsigned int strength,
                               unsigned int intelligence,
                               unsigned int dexterity,
                               unsigned int speed,
                               unsigned int mass,
                               unsigned int height)
{
    struct species* new_species;
    new_species = ((struct species *)malloc(sizeof(struct species)));

    /* Populate the struct with information. */
    new_species->tag            = tag;
    new_species->name           = name;
    new_species->strength       = strength;
    new_species->intelligence   = intelligence;
    new_species->dexterity      = dexterity;
    new_species->mass           = mass;
    new_species->height         = height;

    return new_species;
}


void species_remove(struct species *target)
{
    //free pointers in the struct
    mem_free(2, target->tag, target->name);

    //free the struct itself
    free(target);

    return;
}
