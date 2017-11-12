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

static void generateDefaultTableContent(const char *table);

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
    
    char *tablePath = createFileInDir(name, database);
    
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
    
    generateDefaultTableContent(tablePath);
    free(tablePath);
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

//
// Write default struct and data keys in table file
//
static void generateDefaultTableContent(const char *table)
{
    FILE *file = fopen(table, "a");
    
    fputs(TAB "struct: ~\n", file);
    fputs(TAB "data: ~\n", file);
    fclose(file);
}

//
// Create a new column
//
void createColumn(const char *database, const char *table, const Column col)
{
    char *tablePath = createFileInDirPath(table, database);
    // has column ?
    // find good pos in file
    
    // name and type are required
    
    if (col.name != NULL) {
        printf("%s\n", col.name);
    }
    
    free(tablePath);
}

//
// Check if table has a column named `name`
//
int hasColumn(const char *table, const char *name)
{
    // will not work
    return hasProperty(table, name);
}


// addLine in manage_file file, pos, content

// TODO

// update createTable() function with columns argument
// createColumn(Column col)
// dropTable()
// hasColumn()
// dropColumn()
// check type Validity
// id type should be create and table file creation + it's a reserved name keyword
