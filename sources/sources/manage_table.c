//
//  Filename: manage_table.c
//  Made by: Quentin Brunet
//  Date: 12/11/2017
//  Description: Functions to handle tables
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/manage_database.h"
#include "../headers/manage_file.h"
#include "../headers/manage_table.h"
#include "../headers/manage_array.h"

//
// Create a new table
//
void createTable(const Database database, const Table table)
{
    if (isDatabase(database) == 0) {
        return;
    }
    
    if (hasTable(database, table) != 0) {
        return;
    }
    
    char *databasePath = createFilePath(database.name);
    
    FILE *file = fopen(databasePath, "a");
    
    if (file == NULL) {
        free(databasePath);
        printf("fopen() failed in file %s at line #%d\n", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
    
    fputs(TAB, file);
    fputs("- ", file);
    fputs(table.name, file);
    fputs("\n", file);
    fclose(file);
    free(databasePath);
    
    createTableStructure(database, table);
}

//
// Drop an existing table
//
void dropTable(const Database database, const Table table)
{
    int lineNumber = hasTable(database, table);
    
    // if table doesn't exist, stop here
    if (lineNumber == 0) {
        return;
    }
    
    removeFileInDir(table.name, database.name);
    
    char *databasePath = createFilePath(database.name);
    removeLine(databasePath, lineNumber);
    free(databasePath);
}

//
// Check if database has a table named `name`
// Return 1 if exists, else 0
//
int hasTable(const Database database, const Table table)
{
    char *databasePath = createFilePath(database.name);
    int hasTable = hasProperty(databasePath, table.name);
    
    free(databasePath);
    return hasTable;
}

//
// Create table structure with all columns
//
void createTableStructure(const Database database, const Table table)
{
    char *tablePath = createFileInDir(table.name, database.name);
    
    FILE *file = fopen(tablePath, "a");

    if (file == NULL) {
        free(tablePath);
        printf("fopen() failed in file %s at line #%d\n", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }

    fputs(TAB "struct:\n", file);
    
    int i = 0;
    
    for (i = 0; i < table.nbColumns; ++i) {
        
        if (!table.columns[i].name || strcmp("", table.columns[i].name) == 0) {
            // name can't be null or empty
            // delete table
            // return error
            exit(EXIT_FAILURE);
        }
        
        if (!table.columns[i].type || strcmp("", table.columns[i].type) == 0) {
            // type can't be null or empty
            // delete table
            // return error
            exit(EXIT_FAILURE);
        }
        
        // check type is one of four available type
        // make a function do simplify all this stuff
        fprintf(file, TAB TAB "%s:\n", table.columns[i].name);
        fprintf(file, TAB TAB TAB "type: %s\n", table.columns[i].type);
    }

    fputs(TAB "data: ~\n", file);
    fclose(file);
    
    free(tablePath);
    
    // then improve the proto with check if two column has the same name
    // validate that a col name is correct
    // has column ?
    // find good pos in file
    // name and type are required
}

//
// Check if table has a column named `name`
// Return 1 if exists, else 0
//
int hasColumn(const Database database, const Table table, const Column column)
{
    char *tablePath = createFileInDirPath(table.name, database.name);
    
    FILE *file = fopen(tablePath, "r");

    if (file == NULL) {
        free(tablePath);
        printf("fopen() failed in file %s at line #%d\n", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }

    char line[STRING_SIZE] = "";

    while (fgets(line, STRING_SIZE, file) != NULL && (strcmp(line, TAB "data:\n") != 0 || strcmp(line, TAB "data: ~\n") != 0)) {

        if (strstr(line, TAB TAB TAB) != NULL || strstr(line, TAB "struct:\n") != NULL) {
            continue;
        }

        // trim spaces + remove final ':' char
        line[strlen(line)-2] = '\0';
        trimSpaces(line);
        char *columnName = line;
        
        if (strcmp(columnName, column.name) == 0) {
            fclose(file);
            free(tablePath);
            return 1;
        }
    }
    
    fclose(file);
    free(tablePath);
    
    return 0;
}

//
// Get all column names of a given table
// Return them in an array of char
//
StringArray getColumnNames(const Database database, const Table table)
{
    char *tablePath = createFileInDirPath(table.name, database.name);
    
    StringArray columnNames = {
        malloc(ARRAY_CAPACITY * sizeof(int)),
        0,
        ARRAY_CAPACITY
    };
    
    FILE *file = fopen(tablePath, "r");
    
    if (file == NULL) {
        free(tablePath);
        printf("fopen() failed in file %s at line #%d\n", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
    
    char line[STRING_SIZE] = "";
    
    while (fgets(line, STRING_SIZE, file) != NULL) {
        
        if (strcmp(line, TAB "data:\n") == 0 || strcmp(line, TAB "data: ~\n") == 0) {
            break;
        }
        
        if (strstr(line, TAB TAB) == NULL || strstr(line, TAB TAB TAB) != NULL) {
            continue;
        }
        
        // trim spaces + remove final ':' char
        line[strlen(line)-2] = '\0';
        trimSpaces(line);
        
        columnNames = appendValueToStringArray(columnNames, line);
    }
    
    fclose(file);
    free(tablePath);
    
    return columnNames;
}

//
// Remove ~ in `data: ~` before inserting data
//
void removeDataTilde(const Database database, const Table table)
{
    char *tablePath = createFileInDirPath(table.name, database.name);
    
    FILE *file = fopen(tablePath, "r");
    
    if (file == NULL) {
        free(tablePath);
        printf("fopen() failed in file %s at line #%d\n", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
    
    char line[STRING_SIZE] = "";
    int lineNumber = 0;
    int needChange = 0;
    
    while (fgets(line, STRING_SIZE, file) != NULL && strcmp(line, TAB "data:\n") != 0) {
        if (strcmp(line, TAB "data: ~\n") == 0) {
            needChange = 1;
            break;
        }
        
        ++lineNumber;
    }
    
    fclose(file);
    
    if (!needChange) {
        free(tablePath);
        return;
    }
    
    replaceLine(tablePath, lineNumber, TAB "data:\n");
    
    free(tablePath);
}
