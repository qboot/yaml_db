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
char** manageStringArray(char **array, int *size, int *capacity)
{
    *capacity *= 3;
    
    char **newArray = malloc(*capacity * sizeof(int));
    
    for (int i = 0; i < *size; ++i) {
        newArray[i] = array[i];
    }
    
    for (int i = 0; i < *size; ++i) {
        free(array[i]);
    }
    
    free(array);
    return newArray;
}

//
// Append a value to a given string array
// Return an array of char
//
char** appendValueToStringArray(char **array, int *size, int *capacity, char *value)
{
    if (*size < *capacity) {
        array[*size] = malloc(STRING_SIZE * sizeof(char));
        strcpy(array[*size], value);
        ++(*size);
        return array;
    } else {
        char **newArray = manageStringArray(array, size, capacity);
        newArray[*size] = malloc(STRING_SIZE * sizeof(char));
        strcpy(newArray[*size], value);
        ++(*size);
        return newArray;
    }
}

//
// Allocate more space to a given int array, depending of size and capacity
// Return an array of int
//
int* manageIntArray(int *array, int *size, int *capacity)
{
    *capacity *= 3;
    
    int *newArray = malloc(*capacity * sizeof(int));
    
    for (int i = 0; i < *size; ++i) {
        newArray[i] = array[i];
    }
    
    free(array);
    return newArray;
}

//
// Append a value to a given int array
// Return an array of int
//
int* appendValueToIntArray(int *array, int *size, int *capacity, int value)
{
    if (*size < *capacity) {
        array[*size] = value;
        ++(*size);
        return array;
    } else {
        int *newArray = manageIntArray(array, size, capacity);
        newArray[*size] = value;
        ++(*size);
        return newArray;
    }
}
