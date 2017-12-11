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
void trimTrailingSpaces(char *string);
int isInArray(char **array, int arraySize, char *value);

// @deprecated
StringArray appendValueToStringArray(StringArray, char *value);
IntArray appendValueToIntArray(IntArray array, int value);

// @new
StringArray* createStringArray(void);
IntArray* createIntArray(void);
void appendToStringArray(StringArray *array, char *value);
void appendToIntArray(IntArray *array, int value);
void freeStringArray(StringArray *array);
void freeIntArray(IntArray *array);

#endif
