/*
 * species.h
 *
 *  Created on: Mar 23, 2015
 *      Author: rs14
 */

#ifndef SPECIES_H_
#define SPECIES_H_

/**
 * Species struct definitions.
 */
struct species {
    char *tag;
    char *name;

    /* Attributes of the species
     * NOTE: these are base values */
    unsigned int strength;
    unsigned int intelligence;
    unsigned int dexterity;
    unsigned int speed;

    unsigned int mass;      //in kilograms
    unsigned int height;    //in meters


};

/**
 * Function declarations.
 */

struct species *species_create(char *tag,
                               char *name,
                               unsigned int strength,
                               unsigned int intelligence,
                               unsigned int dexterity,
                               unsigned int speed,
                               unsigned int mass,
                               unsigned int height);
void species_remove(struct species *target);

#endif /* SPECIES_H_ */
