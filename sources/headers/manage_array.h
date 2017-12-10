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
int isInArray(char **array, int arraySize, char *value);
StringArray manageStringArray(StringArray array);
StringArray appendValueToStringArray(StringArray, char *value);
IntArray manageIntArray(IntArray array);
IntArray appendValueToIntArray(IntArray array, int value);

#endif
