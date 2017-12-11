//
//  special_parsing.c
//  yaml_db
//
//  Created by Léo LEGRON on 03/12/2017.
//  Copyright © 2017 Groupe 16. All rights reserved.
//

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../headers/manage_array.h"
#include "../headers/manage_parsing.h"

//
// Parse a string into columns from user input
// If `onlyName` is true, this function will only fill each column name
//
Column* getColumns(char *string, int *nbColumns, int onlyName)
{
    StringArray *columnsArray = splitWithDelimiter(string, ',');
    Column *columns = malloc(sizeof(Column)*columnsArray->size);
    *nbColumns = columnsArray->size;
    
    for (int i = 0; i < *nbColumns; ++i) {
        columns[i].name = malloc(sizeof(char)*STRING_SIZE);
        columns[i].type = malloc(sizeof(char)*STRING_SIZE);
    }
    
    for (int i = 0; i < columnsArray->size; ++i) {
        StringArray *columnPart = splitWithDelimiter(columnsArray->data[i], ' ');
        
        strcpy(columns[i].name, columnPart->data[0]);
        
        if (!onlyName) {
            strcpy(columns[i].type, columnPart->data[1]);
        }
        
        freeStringArray(columnPart);
    }
    
    freeStringArray(columnsArray);
    
    return columns;
}

//
// Parse a string into a row from user input
//
Row getRow(char *string)
{
    StringArray *cellsArray = splitWithDelimiter(string, ',');
    Cell *cells = malloc(sizeof(int)*cellsArray->size);
    for (int i = 0; i < cellsArray->size; ++i) {
        cells[i].data = malloc(sizeof(char)*STRING_SIZE);
    }
    
    for (int i = 0; i < cellsArray->size; ++i) {
        strcpy(cells[i].data, secureInput(cellsArray->data[i]));
    }
    
    Row row = {cellsArray->size, cells};
    
    freeStringArray(cellsArray);
    
    return row;
}

//
// Split a string with a given delimiter which can be escaped with `\`
// Example : String "foo, bar \" baz, qux" => Array "foo" "bar \" baz" and "qux"
//
StringArray* splitWithDelimiter(char *string, char delimiter)
{
    StringArray *stringArray = createStringArray();
    char value[STRING_SIZE] = "";
    
    for (int i = 0; i < strlen(string); ++i) {
        if (i == 0) {
            char charToString[] = {string[0], '\0'};
            strcat(value, charToString);
            
            continue;
        }
        
        if (
            (string[i] == delimiter && string[i-1] != '\\') ||
            (i == strlen(string)-1 && string[i] != delimiter)
            ) {
            
            if (string[i] != delimiter) {
                char charToString[] = {string[i], '\0'};
                strcat(value, charToString);
            }
            
            trimLeadingSpaces(value);
            trimTrailingSpaces(value);
            
            if (strcmp(value, "") != 0) {
                appendToStringArray(stringArray, value);
                strcpy(value, "\0");
            }
            
            continue;
        }
        
        char charToString[] = {string[i], '\0'};
        strcat(value, charToString);
    }
    
    return stringArray;
}

//
// Prevent user input to break yaml file with quote in string or \n, \r special chars
//
char* secureInput(char *string)
{
    char *newString = malloc(sizeof(char)*STRING_SIZE);
    char quoteDelimiter = '\0';
    int pos = 0;
    
    for (int i = 0; i < strlen(string); ++i) {
        if (
            (i == 0 || (i == strlen(string)-1 && quoteDelimiter != '\0')) &&
            (string[i] == '"' || string[i] == '\'')
        ) {
            quoteDelimiter = string[i];
            continue;
        }
        
        if (string[i] == '\n' || string[i] == '\r') {
            strcat(newString, " ");
            ++pos;
            continue;
        }
        
        if (string[i] == '"' && string[i-1] != '\\') {
            strcat(newString, "\"");
            pos += 2;
            continue;
        }
        
        char charToString[] = {string[i], '\0'};
        strcat(newString, charToString);
        ++pos;
    }
    
    return newString;
}
