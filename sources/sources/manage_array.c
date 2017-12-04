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
// Trim a string : remove all spaces
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
