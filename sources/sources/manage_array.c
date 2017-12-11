//
//  Filename: manage_array.c
//  Made by: Quentin Brunet
//  Date: 03/12/2017
//  Description: Functions to handle arrays
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/manage_array.h"

static StringArray manageStringArray(StringArray array);
static IntArray manageIntArray(IntArray array);

//
// Trim a string : remove all spaces
//
void trimSpaces(char *string)
{
    char newString[STRING_SIZE] = "";
    
    for (int i = 0; i < strlen(string); ++i) {
        if (string[i] != ' ' && string[i] != '\n') {
            char charToString[] = {string[i], '\0'};
            strcat(newString, charToString);
        }
    }
    
    strcpy(string, newString);
}

//
// Trim a string : remove all leading spaces
//
void trimLeadingSpaces(char *string)
{
    char newString[STRING_SIZE] = "";
    int isBeginning = 1;
    
    for (int i = 0; i < strlen(string); ++i) {
        if (string[i] == ' ' && isBeginning) {
            continue;
        }
        
        isBeginning = 0;
        
        char charToString[] = {string[i], '\0'};
        strcat(newString, charToString);
    }
    
    strcpy(string, newString);
}

//
// Trim a string : remove all trailing spaces
//
void trimTrailingSpaces(char *string)
{
    char newString[STRING_SIZE] = "";
    int countSpaces = 0;
    
    for (int i = (int) strlen(string)-1; 0 <= i; --i) {
        if (string[i] != ' ') {
            break;
        }
        
        ++countSpaces;
    }
    
    for (int i = 0; i < strlen(string)-countSpaces; ++i) {
        char charToString[] = {string[i], '\0'};
        strcat(newString, charToString);
    }
    
    strcpy(string, newString);
}

//
// @deprecated
// Allocate more space to a given string array, depending of size and capacity
// Return an array of char
//
StringArray manageStringArray(StringArray array)
{
    array.capacity *= 3;
    
    StringArray newArray = {
        malloc(array.capacity * sizeof(int)),
        array.size,
        array.capacity
    };
    
    for (int i = 0; i < array.size; ++i) {
        newArray.data[i] = array.data[i];
    }
    
    for (int i = 0; i < array.size; ++i) {
        free(array.data[i]);
    }
    
    free(array.data);
    return newArray;
}

//
// @deprecated
// Append a value to a given string array
// Return an array of char
//
StringArray appendValueToStringArray(StringArray array, char *value)
{
    if (array.size < array.capacity) {
        array.data[array.size] = malloc(STRING_SIZE * sizeof(char));
        strcpy(array.data[array.size], value);
        ++array.size;
        return array;
    } else {
        StringArray newArray = manageStringArray(array);
        newArray.data[newArray.size] = malloc(STRING_SIZE * sizeof(char));
        strcpy(newArray.data[newArray.size], value);
        ++newArray.size;
        return newArray;
    }
}

//
// @deprecated
// Allocate more space to a given int array, depending of size and capacity
// Return an array of int
//
IntArray manageIntArray(IntArray array)
{
    array.capacity *= 3;
    
    IntArray newArray = {
        malloc(array.capacity * sizeof(int)),
        array.size,
        array.capacity
    };
    
    for (int i = 0; i < array.size; ++i) {
        newArray.data[i] = array.data[i];
    }
    
    free(array.data);
    return newArray;
}

//
// @deprecated
// Append a value to a given int array
// Return an array of int
//
IntArray appendValueToIntArray(IntArray array, int value)
{
    if (array.size < array.capacity) {
        array.data[array.size] = value;
        ++array.size;
        return array;
    } else {
        IntArray newArray = manageIntArray(array);
        newArray.data[newArray.size] = value;
        ++newArray.size;
        return newArray;
    }
}

//
// Check if value `value` is in array `array`
// Return 1 if true, else 0
//
int isInArray(char **array, int arraySize, char *value)
{
    for (int i = 0; i < arraySize; ++i) {
        if (strcmp(array[i], value) == 0) {
            return 1;
        }
    }
    
    return 0;
}

StringArray* createStringArray()
{
    StringArray *array = malloc(sizeof(StringArray));
    array->data = malloc(sizeof(char *) * ARRAY_CAPACITY);
    array->size = 0;
    array->capacity = ARRAY_CAPACITY;
    
    for (int i = 0; i < array->capacity; ++i) {
        array->data[i] = malloc(sizeof(char) * STRING_SIZE);
    }
    return array;
}

void appendToStringArray(StringArray *array, char *value)
{
    if (array->size == array->capacity) {
        array->capacity *= 3;
        array->data = realloc(array->data, sizeof(char *) * array->capacity);
        for (int i = array->size; i < array->capacity; ++i) {
            array->data[i] = malloc(sizeof(char) * STRING_SIZE);
        }
    }
    
    strcpy(array->data[array->size], value);
    ++array->size;
}

//
// Free memory for a given StringArray `array` pointer
//
void freeStringArray(StringArray *array)
{
    for (int i = 0; i < array->capacity; ++i) { // or size ????
        free(array->data[i]);
    }
    free(array);
}
