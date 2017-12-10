//
//  read_file.c
//  yaml_db
//
//  Created by pierre piron on 10/12/2017.
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
#include "../headers/manage_array.h"
#include "../headers/read_file.h"

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
    
    
    StringArray data = readData(f);
    parseData(data, &currentTable);
    
    printResult(columnName, &columnNameSize, &currentTable);
    // func free table to clen memory
    
    
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
StringArray readData(FILE* f)
{
    
    StringArray data = {
        malloc(ARRAY_CAPACITY * sizeof(int)),
        0,
        ARRAY_CAPACITY
    };
    
    
    char temp[STRING_SIZE];
    
    int i = 0;
    while ((fgets(temp, STRING_SIZE, f) != NULL) && (strstr(temp, "-") != NULL)) {
        if ((strstr(temp, "        ")) != NULL && (strstr(temp, "            ")) == NULL) {
            temp[strlen(temp)-2] = '\0';
            data = appendValueToStringArray(data, temp + 8);
            i++;
        }
    }
    
    return data;
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


