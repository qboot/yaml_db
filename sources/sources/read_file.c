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
 // read the name of all databases
**/
void showDatabases()
{
    char *databasePath = createFilePath(DB_FILENAME);
    FILE* file = fopen(databasePath, "r");
    if (file != NULL)
    {
        char databaseName[STRING_SIZE];
        while (fgets(databaseName, STRING_SIZE, file) != NULL) {
            if (strstr(databaseName, TAB) != NULL) {
                printf("%s", databaseName + 6);
            }
        }
    }
    free(databasePath);
    fclose(file);
}

/**
 // Read the data for a given database name 'db_name'
 // and a 'name'
 // Return a Table
 **/
Table findAllRecords(char* db_name, char* table_name)
{
    char *table_file = createFilePath(table_name);
    char *db_file = createFilePath(db_name);
    Table currentTable;
    
    if(!filesFound(db_file, table_name, table_file)) {
        free(table_file);
        free(db_file);
        return currentTable;
    }

    FILE* f = fopen(table_file, "r");
    
    StringArray columnsName = readColumnsName(f);
    
    currentTable.columns = malloc(sizeof(Column) * columnsName.size);
    currentTable.nbColumns = columnsName.size;
    
    for (int i = 0; i < currentTable.nbColumns; i++) {
        currentTable.columns[i].name = columnsName.data[i];
    }
    
    
    StringArray data = readData(f);
    parseData(data, &currentTable);

    fclose(f);
    free(table_file);
    free(db_file);
    
    return currentTable;
}


/**
 // Read the data in a given FILE* 'f'
 // and return an array of string with the column name
 **/
StringArray readColumnsName(FILE* f)
{
    
    StringArray columnsName = {
        malloc(ARRAY_CAPACITY * sizeof(int)),
        0,
        ARRAY_CAPACITY
    };
    
    char temp[STRING_SIZE];
    
    int i = 0;
    while ((fgets(temp, STRING_SIZE, f) != NULL) && (strstr(temp, "data") == NULL)) {
        if ((strstr(temp, TAB TAB)) != NULL && (strstr(temp, TAB TAB TAB)) == NULL) {
            temp[strlen(temp)-2] = '\0';
            columnsName = appendValueToStringArray(columnsName, temp + 8);
            i++;
        }
    }
    
    return columnsName;
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
        if ((strstr(temp, TAB TAB)) != NULL && (strstr(temp, TAB TAB TAB)) == NULL) {
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


