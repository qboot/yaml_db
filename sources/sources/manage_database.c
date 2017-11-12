//
//  Filename: manage_database.c
//  Made by: Quentin Brunet
//  Date: 22/10/2017
//  Description: Functions to handle databases
//

#include <stdio.h>
#include <stdlib.h>
#include "../headers/config.h"
#include "../headers/manage_database.h"
#include "../headers/manage_file.h"

//
// Create a new database
//
void createDatabase(const char *manager, const char *name)
{
    // if database already exists, stop here
    if (hasDatabase(manager, name) != 0) {
        return;
    }
    
    createFile(name);
    createDir(name);
    
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
    int lineNumber = hasDatabase(manager, name);
    
    // if database doesn't exist, stop here
    if (lineNumber == 0) {
        return;
    }
    
    removeFile(name);
    removeDir(name);
    removeLine(manager, lineNumber);
}

//
// Check if manager has a database named `name`
//
int hasDatabase(const char *manager, const char *name)
{
    return hasProperty(manager, name);
}

//
// Check if a database exists
// Return 1 if exists, else 0
//
int isDatabase(const char *name)
{
    char *path = createFilePath(name);
    int isDatabase = isFile(path);
    
    free(path);
    return isDatabase;
}
