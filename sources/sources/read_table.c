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
#include "../headers/manage_array.h"


/**
 // For a given data, insert into a Table 'currentTable'
 // value in Row and Cell
 **/
void parseData(StringArray data, Table *currentTable)
{
    // allocate space for rows.
    currentTable->rows = malloc(data.size);
    currentTable->nbRows = data.size;
    
    for (int i = 0; i < data.size; i++) {
        
        char *currentRow = malloc(sizeof(char) * STRING_SIZE);
        strcpy(currentRow, data.data[i] + 3);

        char *token;
        token = strtok(currentRow, ",");
        
        currentTable->rows[i].cells = malloc(sizeof(Cell) * currentTable->nbColumns);
        
        for (int j = 0; j < currentTable->nbColumns && token != NULL; j++) {
            
            currentTable->rows[i].cells[j].data = malloc(sizeof(char) * STRING_SIZE);
                if (token[0] == ' ')
                {
                    token += 1;
                }
                strcpy(currentTable->rows[i].cells[j].data, token);
                token = strtok(NULL, ",");
                currentTable->rows[i].nbCells = j + 1;
            
        }
    }
    return;
}

/**
 // Print the result for a given 'column_name and
 // for a given 'table'
 **/
void printAllResult(Table *currentTable)
{
    if (currentTable->nbRows == 0)
    {
        printf("No result found");
        return;
    }
    printf("\n");
    
    for (int i = 0; i < currentTable->nbColumns; i++) {
        printf("%s | ", currentTable->columns[i].name);
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
 // SQL : Where Like %...%
 // Search in the currentTable if a the data searched is present at the specific
 // columnPlace
 // return the row number if found, -1 if not
 **/
Table searchData(Table *currentTable, int columnPlace, char *dataSearched)
{
    Table newTable;
    
    newTable.name = currentTable->name;
    newTable.columns = currentTable->columns;
    newTable.nbColumns = currentTable->nbColumns;
    newTable.nbRows = 0;
    newTable.rows = malloc(currentTable->nbRows);
    
    if (dataSearched[0] == '%' && dataSearched[strlen(dataSearched) - 1] == '%')
    {
        dataSearched += 1;
        dataSearched[strlen(dataSearched) - 1] = '\0';
        
        for (int i = 0; i < currentTable->nbRows; i++) {
            char* dataRead = currentTable->rows[i].cells[columnPlace].data;
            if (dataRead[0] == '"')
            {
                dataRead += 1;
                dataRead[strlen(dataRead) - 1] = '\0';
            }
            char *temp = strstr(dataRead, dataSearched);
            if (temp != NULL) {
                newTable.rows[newTable.nbRows] = currentTable->rows[i];
                newTable.nbRows += 1;
            }
        }
    }
    
    return newTable;
}

/**
 // SQL : where =
 // Search in the currentTable if a the data searched is present at the specific
 // columnPlace
 // return a IntArray which contain all the row number where the data was find
 **/
Table searchSpecificData(Table *currentTable, int columnPlace, char *dataSearched)
{
    Table newTable;
    
    newTable.name = currentTable->name;
    newTable.columns = currentTable->columns;
    newTable.nbColumns = currentTable->nbColumns;
    newTable.nbRows = 0;
    newTable.rows = malloc(currentTable->nbRows);
    
    for (int i = 0; i < currentTable->nbRows; i++) {
        char* dataRead = currentTable->rows[i].cells[columnPlace].data;
        if (dataRead[0] == '"')
        {
            dataRead += 1;
            dataRead[strlen(dataRead) - 1] = '\0';
        }
        if (strcmp(dataRead, dataSearched) == 0) {
            newTable.rows[newTable.nbRows] = currentTable->rows[i];
            newTable.nbRows += 1;
        }
    }
    
    return newTable;
}

/**
 // Search in a given table for a specific column
 // When found, delete unused colmun
 // and call the method clearUnusedColumn
 **/
void searchSpecifiColumn(Table *currentTable, char *columnName)
{
    Column *newColumns = malloc(sizeof(Column));
    int nbColumn = 0;
    
    int columnPlace = searchColumnPlace(currentTable, columnName);
    
    // if the column is not in the table
    if (columnPlace < 0)
    {
        currentTable->nbColumns = 0;
        currentTable->nbRows = 0;
        printf("Column not found");
        return;
    }
    for(int i = 0; i < currentTable->nbColumns; i++)
    {
        if (strcmp(currentTable->columns[i].name, columnName) == 0)
        {
            newColumns[nbColumn] = currentTable->columns[i];
            nbColumn++;
        }
    }
    currentTable->columns = newColumns;
    currentTable->nbColumns = nbColumn;
    
    clearUnusedColumn(currentTable, columnPlace);
    
    return;
}

/**
 // for each rows in the table
 // clear unused columns
 **/
void clearUnusedColumn(Table *currentTable, int columnPlace)
{
    Row *newRow = malloc(currentTable->nbRows);
    for (int i = 0; i < currentTable->nbRows; i++)
    {
        newRow->nbCells = 1;
        newRow->cells = malloc(sizeof(STRING_SIZE));
        newRow->cells[0] = currentTable->rows[i].cells[columnPlace];
        free(currentTable->rows[i].cells);
    }
    currentTable->rows = newRow;
    
    return;
}

/**
 // Return the number of rows in the table
 **/
int selectCount(Table *currentTable)
{
    return currentTable->nbRows;
}
