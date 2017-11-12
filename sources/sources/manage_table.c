//
//  Filename: manage_table.c
//  Made by: Quentin Brunet
//  Date: 12/11/2017
//  Description: Functions to handle tables
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/config.h"
#include "../headers/manage_database.h"
#include "../headers/manage_file.h"
#include "../headers/manage_table.h"

//
// Create a new table
//
void createTable(const char *database, const char *name)
{
    if (isDatabase(database) == 0) {
        return;
    }
    
    if (hasTable(database, name) != 0) {
        return;
    }
    
    createFileInDir(name, database);
    
    char *databasePath = createFilePath(database);
    
    FILE *file = fopen(databasePath, "a");
    
    if (file == NULL) {
        free(databasePath);
        printf("fopen() failed in file %s at line #%d\n", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
    
    fputs(TAB, file);
    fputs("- ", file);
    fputs(name, file);
    fputs("\n", file);
    fclose(file);
    free(databasePath);
}

//
// Check if database has a table named `name`
//
int hasTable(const char *database, const char *name)
{
    char *databasePath = createFilePath(database);
    int hasTable = hasProperty(databasePath, name);
    
    free(databasePath);
    return hasTable;
}

// TODO

// update createTable() function with columns argument
// write struct: ~ and data: ~
// createColumn(Column col)
// dropTable()
// hasColumn()
// dropColumn()
