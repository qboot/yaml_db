//
//  Filename: manage_array.h
//  Made by: Quentin Brunet
//  Date: 03/12/2017
//  Description: Header of manage_array.c
//

#ifndef MANAGE_ARRAY
#define MANAGE_ARRAY

#include "config.h"

void trimSpaces(char *string);
void trimLeadingSpaces(char *string);
char** manageArray(char **array, int *size, int *capacity);
char** appendValueToArray(char **array, int *size, int *capacity, char *value);

#endif

