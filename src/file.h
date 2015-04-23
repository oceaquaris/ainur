/*
 * @file file.h
 *
 *  Created on: Mar 13, 2015
 *      Author: rs14
 *
 * @brief Header file for file.c
 */

#ifndef FILE_H_
#define FILE_H_

#include <stdio.h>

/**
 * Function declarations
 */
int file_exists(const char *filename);

char *file_mkpath(const char *directory, const char *filename);

FILE *file_open(const char *filename, const char *mode);

int file_load_from_directory(const char *directory, const char *filename);

#endif /* FILE_H_ */
