/*
 * file.c
 *
 *  Created on: Mar 13, 2015
 *      Author: oceaquaris
 *
 * @brief Contains functions for manipulating files.
 */


#include "file.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"


/**
 * @brief Determines if a filename is valid
 *
 * @param filename
 *        Name of a file to test
 *
 * @return File exists: 1; File does not exist: 0
 */
int file_exists(const char *filename)
{
    //attempt to open file with name 'filename'
    FILE *file = fopen(filename, "r");

    //if the pointer is valid, return 1, true
    if(file) {
        fclose(file);
        return 1;
    }

    //Protocols for debugging
    #ifdef DEBUGGING
    if (debugging) {
        //write a debug message
        debug_fprintf("file_exists() => File \"%s\": %s\n", filename, ERROR_NO_FILE);
    }
    #endif /*DEBUGGING*/
    #ifdef VERBOSE
    if(verbose) {
        //print an error message to the terminal
        debug_printf("file_exists() => File \"%s\": No such file or directory\n", filename);
    }
    #endif /*VERBOSE*/

    //return 0, false, if the pointer is NULL
    return 0;
}

/**
 * @brief Determines if a file in a directory is valid.
 *
 * @param directory
 *        The directory of the file.
 * @param filename
 *        The name of the file.
 *
 * @return 1: File exists; 0: File does not exist
 */
int file_exists_in_directory(const char *directory, const char *filename)
{
    char *path = file_mkpath(directory, filename);  //create path name (heap variable)

    if(path && file_exists(path)) { //test if path was created successfully
        free(path);
        return 1;
    }

    #ifdef DEBUGGING
    (!path) ? debug_fprintf("file_exists_in_directory() => local heap char* \'path\': %s\n", ERROR_NULL_STRING)
            : debug_fprintf("file_exists_in_directory() => File \'path\' %s : %s\n", path, ERROR_NO_FILE);
    #endif /*DEBUGGING*/
    #ifdef VERBOSE
    (!path) ? debug_printf("file_exists_in_directory() => local heap char* \'path\': %s\n", ERROR_NULL_STRING)
            : debug_printf("file_exists_in_directory() => File \'path\' %s : %s\n", path, ERROR_NO_FILE);
    #endif /*VERBOSE*/

    //free the path no matter what.
    free(path);
    return 0;       //return 0, false
}

/**
 * @brief Concatenates a directory name followed by a file name.
 *
 * @param directory
 *        Directory path of the file.
 * @param filename
 *        Name of the file.
 *
 * @return A string representing the path to the file.
 *
 * @note 'directory' needs a terminating '/'; the char* returned needs to be free()ed!
 */
char *file_mkpath(const char *directory, const char *filename)
{
    //test and determine if 'directory' or 'filename' is NULL
    if ( !directory || !filename ) {
        #ifdef DEBUGGING
        if( !directory && debugging) { //if 'directory' is NULL and we're debugging
            //add entry to log-file
            debug_fprintf("file_mkpath => formal parameter \'directory\': %s\n", ERROR_NULL_STRING);

            #ifdef VERBOSE
            //print an error message in the terminal
            debug_printf("file_mkpath => formal parameter \'directory\': %s\n", ERROR_NULL_STRING);
            #endif /*VERBOSE*/
        }
        //if 'filename' is NULL and we're debugging
        if( !filename && debugging) {
            //add entry to log-file
            debug_fprintf("file_mkpath => formal parameter \'filename\': %s\n", ERROR_NULL_STRING);

            #ifdef VERBOSE
            //print an error message in the terminal
            debug_printf("file_mkpath => formal parameter \'filename\': %s\n", ERROR_NULL_STRING);
            #endif /*VERBOSE*/
        }
        #endif /*DEBUGGING*/

        return NULL;  //return a null string; return false;
    }

    unsigned int dirlen = strlen(directory), filelen = strlen(filename);

    //test to see if the directory is '/' terminated
    if(directory[dirlen-1] != '/') {
        #ifdef DEBUGGING
        if(debugging) {
            //add entry to log-file
            debug_fprintf("file_mkpath() => parameter \'directory\': string not \'/\' terminated\n");

            #ifdef VERBOSE
            //print an error message in the terminal
            debug_printf("file_mkpath() => parameter \'directory\': string not \'/\' terminated\n");
            #endif /*VERBOSE*/
        }
        #endif /*DEBUGGING*/

        return NULL;  //return a null string;
    }


    char *output;
    output = (char *)malloc( sizeof(char) * (dirlen + filelen + sizeof(char)));

    if(!output) { //check to see if the heap allocated the data
        #ifdef DEBUGGING
        if(debugging) {
            debug_fprintf("file_mkpath() => heap char* \'output\': %s\n", ERROR_MALLOC);
        }
        #ifdef VERBOSE
        debug_printf("file_mkpath() => heap char* \'output\': %s\n", ERROR_MALLOC);
        #endif /*VERBOSE*/

        #endif /*DEBUGGING*/

        return NULL;
    }

    strcpy(output, directory);  //copy directory into output
    strcat(output, filename);   //tack on filename to output and return output
    return output;
}

/**
 * @brief Opens a file.
 *
 * @param filename
 *        A filename or path to a file.
 * @param mode
 *        The file access mode.
 *
 * @return A pointer to a FILE struct.
 */
FILE *file_open(const char *filename, const char *mode)
{
    if(!filename || !mode) {
        #ifdef DEBUGGING
        if(debugging) {
            if(!filename) {
                debug_fprintf("file_open => formal param \'filename\': %s\n", ERROR_NULL_STRING);
            }
            if(!mode) {
                debug_fprintf("file_open => formal param \'mode\': %s\n", ERROR_NULL_STRING);
            }
        }
        #endif /*DEBUGGING*/
        #ifdef VERBOSE
        if(verbose) {
            if(!filename) {
                debug_printf("file_open => formal param \'filename\': %s\n", ERROR_NULL_STRING);
            }
            if(!mode) {
                debug_printf("file_open => formal param \'mode\': %s\n", ERROR_NULL_STRING);
            }
        }
        #endif /*VERBOSE*/

        return NULL;
    }

    FILE *file = NULL;

    file = fopen(filename, mode);

    if(!file) {
        #ifdef DEBUGGING
        if(debugging) {
            debug_fprintf("file_open() => local FILE* \'file\': %s\n", ERROR_NULL_FILE);
        }
        #endif /*DEBUGGING*/
        #ifdef VERBOSE
        if(verbose) {
            debug_printf("file_open() => local FILE* \'file\': %s\n", ERROR_NULL_FILE);
        }
        #endif /*VERBOSE*/

        return NULL;
    }

    return file;
}

/**
 * @NOTE: Construction still in process!!!!!!!!
 *
 * @brief Loads a specified file into the Lua API.
 *
 * @param filename
 *        File to load.
 *
 * @return 1: successfully loaded; 0: unsuccessfully loaded
 */
int file_load(const char *filename)
{
    if(!filename) {
        #ifdef DEBUGGING
        if(debugging) {
            debug_fprintf("file_load() => formal param \'filename\': %s\n", ERROR_NULL_STRING);
        }
        #endif /*DEBUGGING*/
        #ifdef VERBOSE
        if(verbose) {
            debug_printf("file_load() => formal param \'filename\': %s\n", ERROR_NULL_STRING);
        }
        #endif /*VERBOSE*/

        return 0;
    }

    /*
     * Lua load functions go below.
     */

    return 1;
}

int file_load_from_directory(const char *directory, const char *filename)
{


    return 1;
}
