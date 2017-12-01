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
void trimString(char *string)
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
// Allocate more space to a given array, depending of size and capacity
// Return an array of char
//
char** manageArray(char **array, int *size, int *capacity)
{
    *capacity *= 3;
    
    char **newArray = malloc(*capacity * sizeof(int));
    
    for (int i = 0; i < *size; ++i) {
        newArray[i] = array[i];
    }
    
    free(array);
    return newArray;
}

//
// Append a value to a given array
// Return an array of char
//
char** appendValueToArray(char **array, int *size, int *capacity, char *value)
{
    if (*size < *capacity) {
        array[*size] = malloc(STRING_SIZE * sizeof(char));
        strcpy(array[*size], value);
        ++(*size);
        return array;
    } else {
        char **newArray = manageArray(array, size, capacity);
        newArray[*size] = malloc(STRING_SIZE * sizeof(char));
        strcpy(newArray[*size], value);
        ++(*size);
        return newArray;
    }
}
