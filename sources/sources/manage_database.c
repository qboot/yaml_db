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

//
// Create a new database
//
void createDatabase(const Manager manager, const Database database)
{
    // if database already exists, stop here
    if (hasDatabase(manager, database) != 0) {
        return;
    }
    
    char *databasePath = createFile(database.name);
    createDir(database.name);
    
    FILE *file = fopen(manager.path, "a");
    
    if (file == NULL) {
        printf("fopen() failed in file %s at line #%d\n", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
    
    fputs(TAB, file);
    fputs("- ", file);
    fputs(database.name, file);
    fputs("\n", file);
    fclose(file);
    free(databasePath);
}

//
// Drop an existing database
//
void dropDatabase(const Manager manager, const Database database)
{
    int lineNumber = hasDatabase(manager, database);
    
    // if database doesn't exist, stop here
    if (lineNumber == 0) {
        return;
    }
    
    removeFile(database.name);
    removeDir(database.name);
    removeLine(manager.path, lineNumber);
}

//
// Check if manager has a database named `name`
//
int hasDatabase(const Manager manager, const Database database)
{
    return hasProperty(manager.path, database.name);
}

//
// Check if a database exists
// Return 1 if exists, else 0
//
int isDatabase(const Database database)
{
    char *databasePath = createFilePath(database.name);
    int isDatabase = isFile(databasePath);
    
    free(databasePath);
    return isDatabase;
}
