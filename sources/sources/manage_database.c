//
//  Filename: manage_database.c
//  Made by: Quentin Brunet
//  Date: 22/10/2017
//  Description: Functions to handle databases
//

#include <stdio.h>
#include <stdlib.h>
#include "../headers/manage_database.h"
#include "../headers/manage_file.h"

#define TAB "    "

//
// Create a new database
//
void createDatabase(const char *manager, const char *name)
{
    // if database already exists, stop here
    if (findMatchingLine(manager, name) != 0) {
        return;
    }
    
    createFile(name);
    
    FILE *file = fopen(manager, "a");
    
    if (file == NULL) {
        printf("fopen() failed in file %s at line #%d\n", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
    
    fputs(TAB, file);
    fputs("- ", file);
    fputs(name, file);
    fputs("\n", file);
    fclose(file);
}

//
// Drop an existing database
//
void dropDatabase(const char *manager, const char *name)
{
    int lineNumber = findMatchingLine(manager, name);
    
    // if database doesn't exist, stop here
    if (lineNumber == 0) {
        return;
    }
    
    removeFile(name);
    removeMatchingLine(manager, lineNumber);
}
