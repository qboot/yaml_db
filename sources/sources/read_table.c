//
//  read_table.c
//  yaml_db
//
//  Created by pierre piron on 25/11/2017.
//  Copyright © 2017 Groupe 16. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "../headers/read_table.h"
#include "../headers/config.h"
#include "../headers/manage_file.h"
#include "../headers/manage_database.h"

/**
 // Read the data for a given database name 'db_name'
 // and a 'name'
 **/
void findAllRecords(char* db_name, char* table_name)
{
    char *table_file = createFilePath(table_name);
    
    if(!filesFound(db_name, table_name, table_file)) {
        return;
    }
    
    Table currentTable;
    
    FILE* f = fopen(table_file, "r");
    
    int columnNameSize = 0;
    char **columnName = readColumnName(f, &columnNameSize);
    
    currentTable.columns = malloc(sizeof(Column) * columnNameSize);
    currentTable.nbColumns = columnNameSize;
    
    for (int i = 0; i < columnNameSize; i++) {
        currentTable.columns[i].name = columnName[i];
    }
    
    int dataSize = 0;
    char **data = readData(f, &dataSize);


    parseData(data, dataSize, &currentTable);
    
    printResult(columnName, &columnNameSize, &currentTable);
    // func free table to clen memory
    
    
    return;
}

/**
 // For a given data, insert into a Table 'currentTable'
 // value in Row and Cell
 **/
void parseData(char **data, int numberOfData, Table *currentTable)
{
    // allocate space for rows.
    currentTable->rows = malloc(numberOfData);
    currentTable->nbRows = numberOfData;
    
    for (int i = 0; i < numberOfData; i++) {
        
        char *currentRow = malloc(sizeof(char) * STRING_SIZE);
        strcpy(currentRow, data[i] + 3);

        char *token;
        token = strtok(currentRow, ",");
        
        currentTable->rows[i].cells = malloc(sizeof(Cell) * currentTable->nbRows);
        
        for (int j = 0; j < currentTable->nbColumns && token != NULL; j++) {
            
            currentTable->rows[i].cells[j].data = malloc(sizeof(char) * STRING_SIZE);

                strcpy(currentTable->rows[i].cells[j].data, token);
                token = strtok(NULL, ",");
                currentTable->rows[i].nbCells = j + 1;
            
                //printf("nbCells : %d, current cell : %s\n", currentTable->rows[i].nbCells, currentTable->rows[i].cells[j].data);
        }
    }
    return;
}

/**
 // Read the data for a given database name 'db_name'
 // and a given 'tbale_name',
 // and print result where is a given 'record_value'
 **/
void findSpecificRecords(char* db_name, char* table_name, char* column_name, char* record_value)
{
    char *table_file = createFilePath(table_name);
    
    if (!filesFound(db_name, table_name, table_file)) {
        return;
    }
    
    FILE* f = fopen(table_file, "r");
    
    char temp[STRING_SIZE];
    int columnNameSize = 0;

    char **columnName = readColumnName(f, &columnNameSize);
    
    while ((fgets(temp, STRING_SIZE, f) != NULL) && (strstr(temp, "-") != NULL)) {
        if ((strstr(temp, "        ")) != NULL && (strstr(temp, "            ")) == NULL && (strstr(temp, record_value)) != NULL) {
            char temp2[STRING_SIZE];
            strcpy(temp2, temp + 11);
            char *token;
            token = strtok(temp2, ", ");
            while (token != NULL) {
                printf("%s |", token);
                token = strtok(NULL, ",");
            }
        }
    }
    
    for (int i = 0; i < columnNameSize; i++) {
        free(columnName[i]);
    }
    free(columnName);
    
}



/**
 // Read the data in a given FILE* 'f'
 // and return an array of string with the column name
 **/
char** readColumnName(FILE* f, int *columnNameSize)
{
    
    char **columnName = malloc(10 * sizeof(char *));

    char temp[STRING_SIZE];
    
    int i = 0;
    while ((fgets(temp, STRING_SIZE, f) != NULL) && (strstr(temp, "data") == NULL)) {
        if ((strstr(temp, "        ")) != NULL && (strstr(temp, "            ")) == NULL) {
            temp[strlen(temp)-2] = '\0';
            columnName[i] = malloc(STRING_SIZE * sizeof(char));
            strcpy(columnName[i], temp + 8);
            i++;
            *columnNameSize += 1;
        }
    }
    
    return columnName;
}

/**
 // Read the data in a given FILE* 'f'
 // and return an array of string with the records
 **/
char** readData(FILE* f, int *dataSize)
{
    // update
    char **data = malloc(50 * sizeof(char *));
    
    char temp[STRING_SIZE];
    
    int i = 0;
    while ((fgets(temp, STRING_SIZE, f) != NULL) && (strstr(temp, "-") != NULL)) {
        if ((strstr(temp, "        ")) != NULL && (strstr(temp, "            ")) == NULL) {
            temp[strlen(temp)-2] = '\0';
            data[i] = malloc(STRING_SIZE * sizeof(char));
            strcpy(data[i], temp + 8);
            i++;
            *dataSize += 1;
        }
    }
    
    return data;
}

/**
 // Print the result for a given 'column_name and
 // for a given 'table'
 **/
void printResult(char **column_name, int *columnNameSize, Table *currentTable)
{
    for (int i = 0; i < *columnNameSize; i++) {
        printf("%s | ", column_name[i]);
    }
    printf("\n");
    
    for (int i = 0; i < currentTable->nbRows; i++) {
        for (int j = 0; j < currentTable->rows[i].nbCells; j++) {
            printf("%s | ", currentTable->rows[i].cells[j].data);
        }
        printf("\n");
    }
}


/**
 // Search in the currentTable the place of the column searched
 // and return the place or -1 if not found
 **/
int searchColumnPlace(Table *currentTable, char *columnName)
{
    for (int i = 0; i < currentTable->nbColumns; i++) {
        if (strcmp(currentTable->columns[i].name, columnName) == 0) {
            return i;
        }
    }
    return -1;
}



/**
 //     NON TESTER
 //
 // Search in the currentTable if a the data searched is present at the specific
 // columnPlace
 // return the row number if found, -1 if not
 **/
int searchSpecificData(Table *currentTable, int columnPlace, char *dataSearched)
{
    for (int i = 0; i < currentTable->nbRows; i++) {
        if (strstr(currentTable->rows[i].cells[columnPlace].data, dataSearched) != NULL) {
            return i;
        }
    }
    return -1;
}



/**
// Check if database file exists
// if table is present in database
// and if table file exists
// Return 1 if all exists, else 0
 **/
int filesFound(char *db_name, char *table_name, char *table_file)
{
    if (!isFile(db_name)) {
        printf("the database doesn't exist\n");
        return 0;
    }
    
    if (!hasProperty(db_name, table_name)) {
        printf("the table doesn't exist in the database file\n");
        return 0;
    }
    

    if (!isFile(table_file)) {
        printf("the table file doesn't exist\n");
        return 0;
    }
    
    return 1;
}
