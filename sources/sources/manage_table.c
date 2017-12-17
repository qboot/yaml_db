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
int createTable(const Database database, const Table table)
{
    if (isDatabase(database) == 0) {
        printf("Oops! Database `%s` doesn't exist.\n", database.name);
        return 0;
    }
    
    if (hasTable(database, table) != 0) {
        printf("Oops! Table `%s` already exists.\n", table.name);
        return 0;
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
    
    if (createTableStructure(database, table)) {
        printf("New table `%s` created!\n", table.name);
        return 1;
    }
    
    return 0;
}

//
// Drop an existing table
//
int dropTable(const Database database, const Table table)
{
    if (isDatabase(database) == 0) {
        printf("Oops! Database `%s` doesn't exist.\n", database.name);
        return 0;
    }
    
    int lineNumber = hasTable(database, table);
    
    // if table doesn't exist, stop here
    if (lineNumber == 0) {
        printf("Oops! Table `%s` doesn't exist.\n", table.name);
        return 0;
    }
    
    removeFileInDir(table.name, database.name);
    
    char *databasePath = createFilePath(database.name);
    removeLine(databasePath, lineNumber);
    free(databasePath);
    printf("Table `%s` dropped!\n", table.name);
    return 1;
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
int createTableStructure(const Database database, const Table table)
{
    // table name is not valid, stop here
    if (isValidName(table.name) == 0) {
        printf("Table name should only contain 0-9 a-z A-Z and _ characters.\n");
        return 0;
    }
    
    char *tablePath = createFileInDir(table.name, database.name);
    
    FILE *file = fopen(tablePath, "a");

    if (file == NULL) {
        free(tablePath);
        printf("fopen() failed in file %s at line #%d\n", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }

    fputs(TAB "struct:\n", file);
    
    int i = 0;
    int error = 0;
    char *allowedTypes[] = {"int", "float", "string", "char"};
    int allowedTypesSize = 4;
    
    for (i = 0; i < table.nbColumns; ++i) {
        
        if (
            !table.columns[i].name ||
            strcmp("", table.columns[i].name) == 0 ||
            !isValidName(table.columns[i].name)
        ) {
            printf("Please check that all your columns have a valid name.\n");
            error = 1;
            break;
        }
        
        if (
            !table.columns[i].type ||
            strcmp("", table.columns[i].type) == 0 ||
            !isInArray(allowedTypes, allowedTypesSize, table.columns[i].type)
        ) {
            printf("Please check that all your columns have a valid type.\n");
            printf("Allowed types are : `int`, `float`, `string` and `char`.\n");
            error = 1;
            break;
        }
        
        fprintf(file, TAB TAB "%s:\n", table.columns[i].name);
        fprintf(file, TAB TAB TAB "type: %s\n", table.columns[i].type);
    }

    fputs(TAB "data: ~\n", file);
    fclose(file);
    
    free(tablePath);
    
    // error thrown, drop the table
    if (error) {
        dropTable(database, table);
        return 0;
    }
    
    return 1;
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
Column* getAllColumns(const Database database, const Table table, int *nbColumns)
{
    char *tablePath = createFileInDirPath(table.name, database.name);
    
    StringArray *columnNames = createStringArray();
    StringArray *columnTypes = createStringArray();
    
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
            
            // get column types
            char *result = strstr(line, "type: ");
            if (result != NULL) {
                int beginning = (int) (result - line + 6);
                int length = (int) strlen(line) - beginning;
                char type[STRING_SIZE] = "";
                strncpy(type, line+beginning, length);
                trimSpaces(type);
                appendToStringArray(columnTypes, type);
            }
            
            continue;
        }
        
        // trim spaces + remove final ':' char
        line[strlen(line)-2] = '\0';
        trimSpaces(line);
        appendToStringArray(columnNames, line);
    }
    
    fclose(file);
    free(tablePath);
    
    Column *columns = malloc(sizeof(Column) * columnNames->size);
    *nbColumns = columnNames->size;
    
    for (int i = 0; i < *nbColumns; ++i) {
        columns[i].name = malloc(sizeof(char) * STRING_SIZE);
        columns[i].type = malloc(sizeof(char) * STRING_SIZE);
        columns[i].name = columnNames->data[i];
        columns[i].type = columnTypes->data[i];
    }
    
    for (int i = 0; i < *nbColumns; ++i) {
        printf("%s\n", columns[i].name);
    }
    
    freeStringArray(columnTypes);
    freeStringArray(columnNames);
    
    return columns;
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
