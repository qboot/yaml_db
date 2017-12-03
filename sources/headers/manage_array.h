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
char** manageStringArray(char **array, int *size, int *capacity);
char** appendValueToStringArray(char **array, int *size, int *capacity, char *value);
int* manageIntArray(int *array, int *size, int *capacity);
int* appendValueToIntArray(int *array, int *size, int *capacity, int value);

#endif

