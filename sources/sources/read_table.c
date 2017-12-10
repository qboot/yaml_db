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

                strcpy(currentTable->rows[i].cells[j].data, token);
                token = strtok(NULL, ",");
                currentTable->rows[i].nbCells = j + 1;
            
                //printf("nbCells : %d, current cell : %s\n", currentTable->rows[i].nbCells, currentTable->rows[i].cells[j].data);
        }
    }
    return;
}

/**
 // Print the result for a given 'column_name and
 // for a given 'table'
 **/
void printResult(Table *currentTable)
{
    for (int i = 0; i < currentTable->nbColumns; i++) {
        printf("%s | ", currentTable->columns[i].name);
    }
    printf("\n");
    
    /*

    for (int i = 0; i < currentTable->nbColumns; i++) {
        printf("%s | ", currentTable->columns[i]);
    }
    
    */
    
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
 // SQL : Like %...%
 // Search in the currentTable if a the data searched is present at the specific
 // columnPlace
 // return the row number if found, -1 if not
 **/
IntArray searchData(Table *currentTable, int columnPlace, char *dataSearched)
{
    IntArray rowNumberList = {
        malloc(ARRAY_CAPACITY * sizeof(int)),
        0,
        ARRAY_CAPACITY
    };
    
    for (int i = 0; i < currentTable->nbRows; i++) {
        if (strstr(currentTable->rows[i].cells[columnPlace].data, dataSearched) != NULL) {
           rowNumberList = appendValueToIntArray(rowNumberList, i);
        }
    }
    return rowNumberList;
}

/**
 //     NON TESTER
 //
 // SQL : where
 // Search in the currentTable if a the data searched is present at the specific
 // columnPlace
 // return a IntArray which contain all the row number where the data was find
 **/
IntArray searchSpecificData(Table *currentTable, int columnPlace, char *dataSearched)
{
    IntArray rowNumberList = {
        malloc(ARRAY_CAPACITY * sizeof(int)),
        0,
        ARRAY_CAPACITY
    };
    
    for (int i = 0; i < currentTable->nbRows; i++) {
        if (strcmp(currentTable->rows[i].cells[columnPlace].data, dataSearched) == 0) {
            rowNumberList = appendValueToIntArray(rowNumberList, i);
        }
    }
    
    return rowNumberList;
}