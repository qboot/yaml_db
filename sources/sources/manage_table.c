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
void createTable(const char *database, const char *name, int nbColumns, const Column *columns)
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
    
    createTableStructure(tablePath, nbColumns, columns);
}

//
// Drop an existing table
//
void dropTable(const char *database, const char *name)
{
    int lineNumber = hasTable(database, name);
    
    // if table doesn't exist, stop here
    if (lineNumber == 0) {
        return;
    }
    
    removeFileInDir(name, database);
    
    // TODO: improve me
    char *databasePath = createFilePath(database);
    
    removeLine(databasePath, lineNumber);
    
    free(databasePath);
}

//
// Check if database has a table named `name`
// Return 1 if exists, else 0
//
int hasTable(const char *database, const char *name)
{
    char *databasePath = createFilePath(database);
    int hasTable = hasProperty(databasePath, name);
    
    free(databasePath);
    return hasTable;
}

//
// Create table structure with all columns
//
void createTableStructure(const char *table, int nbColumns, const Column *columns)
{
    FILE *file = fopen(table, "a");

    if (file == NULL) {
        free((char *) table);
        printf("fopen() failed in file %s at line #%d\n", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }

    fputs(TAB "struct:\n", file);
    
    int i = 0;
    
    for (i = 0; i < nbColumns; ++i) {
        
        if (!columns[i].name || strcmp("", columns[i].name) == 0) {
            // name can't be null or empty
            // delete table
            // return error
            exit(EXIT_FAILURE);
        }
        
        if (!columns[i].type || strcmp("", columns[i].type) == 0) {
            // type can't be null or empty
            // delete table
            // return error
            exit(EXIT_FAILURE);
        }
        
        // check type is one of four available type
        // make a function do simplify all this stuff
        fprintf(file, TAB TAB "%s:\n", columns[i].name);
        fprintf(file, TAB TAB TAB "type: %s\n", columns[i].type);
    }

    fputs(TAB "data: ~\n", file);
    fclose(file);
    
    free((char *) table);
    
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
int hasColumn(const char *table, const char *name)
{
    FILE *file = fopen(table, "r");

    if (file == NULL) {
        free((char *) table);
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
        
        if (strcmp(columnName, name) == 0) {
            fclose(file);
            return 1;
        }
    }
    
    fclose(file);
    return 0;
}

//
// Add row(s) to a given table
//
void addRows(
     const char *database,
     const char *table,
     const int nbColumns,
     const char **columns,
     const int nbRows,
     const Row *rows
)
{
    // TODO: Case where no column is given
    
    char **columnNames = NULL;
    int *size = malloc(sizeof(int));
    columnNames = getColumns(table, size);
    
    char newRows[nbRows][STRING_SIZE];
    int newRowsSize = 0;
    
    // foreach row -> foreach column name -> foreach column name given
    // if column name given == column name, save row value
    
    for (int i = 0; i < nbRows; ++i) {
        char row[STRING_SIZE] = "";
        strcat(row, "- [");
        
        for (int j = 0; j < *size; ++j) {
            int found = 0;
            
            for (int k = 0; k < nbColumns;  ++k) {
                if (strcmp(columnNames[j], columns[k]) != 0) {
                    continue;
                }
                
                strcat(row, "\"");
                strcat(row, rows[i].cells[j].data);
                strcat(row, "\",");
                found = 1;
                break;
            }
            
            if (!found) {
                strcat(row, "\"\",");
            }
        }
        
        row[strlen(row)-1] = '\0';
        strcat(row, "]\n");
        strcpy(newRows[newRowsSize], row);
        ++newRowsSize;
    }
    
    removeDataTilde(table);
    
    // write in file
    FILE *file = fopen(table, "a");
    
    if (file == NULL) {
        free((char *) table);
        printf("fopen() failed in file %s at line #%d\n", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
    
    for (int i = 0; i < newRowsSize; ++i) {
        fprintf(file, TAB TAB "%s", newRows[i]);
    }
    
    fclose(file);
    
    // free memory
    for (int i = 0; i < *size; ++i) {
        free(columnNames[i]);
    }
    free(size);
    free(columnNames);
}

//
// Get all column names of a given table
// Return them in an array of char
//
char** getColumns(const char *table, int *nbColumns)
{
    char **columns = malloc(ARRAY_CAPACITY * sizeof(int));
    int *size = malloc(sizeof(int));
    int *capacity = malloc(sizeof(int));
    
    *size = 0;
    *capacity = ARRAY_CAPACITY;
    
    FILE *file = fopen(table, "r");
    
    if (file == NULL) {
        free((char *) table);
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
        
        columns = appendValueToArray(columns, size, capacity, line);
    }
    
    fclose(file);
    
    *nbColumns = *size;
    
    free(size);
    free(capacity);
    
    return columns;
}

//
// Remove ~ in `data: ~` before inserting data
//
void removeDataTilde(const char *table)
{
    FILE *file = fopen(table, "r");
    
    if (file == NULL) {
        free((char *) table);
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
        return;
    }
    
    replaceLine(table, lineNumber, TAB "data:\n");
}

// TODO
void updateRows(
    const char *database,
    const char *table,
    const int nbColumns,
    const char **columns,
    const int nbValues,
    const char **values,
    const int nbConditions,
    const Condition *conditions
)
{
    // get columnNames
    char **columnNames = NULL;
    int *size = malloc(sizeof(int));
    columnNames = getColumns(table, size);
    
    FILE *file = fopen(table, "r");
    
    if (file == NULL) {
        free((char *) table);
        printf("fopen() failed in file %s at line #%d\n", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
    
    char line[STRING_SIZE] = "";
    int pos = 0;
    int isData = 0;
    
    while (fgets(line, STRING_SIZE, file) != NULL) {
        if (strcmp(line, TAB "data:\n") == 0) {
            isData = 1;
            ++pos;
            continue;
        }
        
        if (!isData) {
            ++pos;
            continue;
        }
        
        if (strcmp(line, TAB "data: ~\n") == 0) {
            break;
        }
        
        // row is array of values for current line
        int *rowSize = malloc(sizeof(int));
        char **row = parseRow(line, rowSize);
        int needUpdate = 0;
        
        // check if row is concerned by `where` claused
        for (int i = 0; i < *size; ++i) {
            for (int j = 0; j < nbConditions; ++j) {
                if (strcmp(conditions[j].type, "=") == 0) {
                    if (strcmp(conditions[j].column, columnNames[i]) == 0) {
                        if (strcmp(conditions[j].value, row[i]) == 0) {
                            needUpdate = 1;
                        } else {
                            needUpdate = 0;
                        }
                    }
                } else if (strcmp(conditions[i].type, ">") == 0) {
                    // TODO
                } else if (strcmp(conditions[i].type, "<") == 0) {
                    // TODO
                }
            }
        }

        if (needUpdate) {
            char newLine[STRING_SIZE] = TAB TAB "- [";

            for (int i = 0; i < *size; ++i) {
                int find = 0;
                strcat(newLine, "\"");
                for (int j = 0; j < nbColumns; ++j) {
                    if (strcmp(columnNames[i], columns[j]) == 0) {
                        find = 1;
                        strcat(newLine, values[j]);
                    }
                }
                
                if (!find) {
                    strcat(newLine, row[i]);
                }
                
                strcat(newLine, "\",");
            }

            newLine[strlen(newLine)-1] = '\0';
            strcat(newLine, "]\n");

            // replace line in file
            replaceLine(table, pos, newLine);
        }
        
        free(row);
        free(rowSize);
        
        ++pos;
    }
    
    fclose(file);
    
    free(columnNames);
    free(size);
}

char **parseRow(char *row, int *size)
{
    // clean row before manipulate it
    trimLeadingSpaces(row);
    
    char **rowToArray = malloc(ARRAY_CAPACITY * sizeof(int));
    int *capacity = malloc(sizeof(int));
    
    *size = 0;
    *capacity = ARRAY_CAPACITY;
    
    int start = 0;
    int isReading = 0;
    char value[STRING_SIZE] = "";
    
    for (int i = 0; i < strlen(row); ++i) {
        if (row[i] != '[' && !start) {
            continue;
        }
        
        start = 1;
        
        if (row[i] == '"' && row[i-1] != '\\' && !isReading) {
            isReading = 1;
            continue;
        }
        
        if (row[i] == '"' && row[i-1] != '\\' && isReading) {
            isReading = 0;
            
            // insert in array
            rowToArray = appendValueToArray(rowToArray, size, capacity, value);
            strcpy(value, "\0");
            
            continue;
        }
        
        if (isReading) {
            char charToString[] = {row[i], '\0'};
            strcat(value, charToString);
        }
    }
    
    free(capacity);
    
    return rowToArray;
}

void deleteRows(
    const char *database,
    const char *table,
    const Condition *conditions
    )
{
    // TODO
}

// rethink how to save different types, maybe validate them too - low
// try to use defined structs
// split row related function in a new file /!\
