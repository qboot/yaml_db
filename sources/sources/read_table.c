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
    
    if (!filesFound(db_name, table_name, table_file)) {
        return;
    }
    
    FILE* f = fopen(table_file, "r");
    

    int columnNameSize = 0;
    char **columnName = readColumnName(f, &columnNameSize);

    int dataSize = 0;
    char **data = readData(f, &dataSize);
    printResult(columnName, &columnNameSize, data, &dataSize);

    
    for (int i = 0; i < columnNameSize; i++) {
        free(columnName[i]);
    }
    free(columnName);
    
    for (int i = 0; i < dataSize; i++) {
        free(data[i]);
    }
    free(data);
    
    fclose(f);
    return;
}


/**
 // Read the data for a given database name 'db_name'
 // and a given 'tbale_name',
 // and print result where is a given 'record_value'
 **/
void findSpecificRecords(char* db_name, char* table_name, char* record_value)
{
    char *table_file = createFilePath(table_name);
    
    if (!filesFound(db_name, table_name, table_file)) {
        return;
    }
    
    FILE* f = fopen(table_file, "r");
    
    char temp[STRING_SIZE];
    int columnNameSize = 0;

    char **columnName = readColumnName(f, &columnNameSize);
    char **data[10][STRING_SIZE] = {"test 1", "test2", "test3", "test4", "test5", "test6", "test7", "test8", "test9", "test10"};
    //printResult(columnName, &columnNameSize, data);
    
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
            temp[strlen(temp)-1] = '\0';
            temp[strlen(temp)-1] = '\0';
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
    
    char **data = malloc(50 * sizeof(char *));
    
    char temp[STRING_SIZE];
    
    int i = 0;
    while ((fgets(temp, STRING_SIZE, f) != NULL) && (strstr(temp, "-") != NULL)) {
        if ((strstr(temp, "        ")) != NULL && (strstr(temp, "            ")) == NULL) {
            temp[strlen(temp)-1] = '\0';
            temp[strlen(temp)-1] = '\0';
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
 // for a given 'data'
 **/
void printResult(char **column_name, int *columnNameSize, char **data, int *dataSize)
{
    
    for (int i = 0; i < *columnNameSize; i++) {
        printf("%s | ", column_name[i]);
    }
    printf("\n");
    
    for (int i = 0; i < *dataSize; i++) {
        char temp[STRING_SIZE];
        strcpy(temp, data[i] + 3);
        char *token;
        token = strtok(temp, ",");
        while (token != NULL) {
            printf("%s | ", token);
            token = strtok(NULL, ",");
        }
        printf("\n");
    }

    return;
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
