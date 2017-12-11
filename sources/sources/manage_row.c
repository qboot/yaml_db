//
//  Filename: manage_row.c
//  Made by: Quentin Brunet
//  Date: 04/12/2017
//  Description: Functions to handle rows
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/manage_database.h"
#include "../headers/manage_file.h"
#include "../headers/manage_table.h"
#include "../headers/manage_array.h"

//
// Parse a row string `- ["1","2","3","4"]` to an array of values
// Return an array of string
//
StringArray parseRow(char *row)
{
    // clean row before manipulate it
    trimLeadingSpaces(row);
    
    StringArray rowToArray = {malloc(ARRAY_CAPACITY * sizeof(int)), 0, ARRAY_CAPACITY};
    
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
            rowToArray = appendValueToStringArray(rowToArray, value);
            strcpy(value, "\0");
            
            continue;
        }
        
        if (isReading) {
            char charToString[] = {row[i], '\0'};
            strcat(value, charToString);
        }
    }
    
    return rowToArray;
}

//
// Add row(s) to a given table
//
int addRows(const Database database, const Table table)
{
    if (!hasTable(database, table)) {
        printf("Oops! Table `%s` doesn't exist.\n", table.name);
        return 0;
    }
    
    StringArray columnNames = getColumnNames(database, table);
    
    if (columnNames.size != table.rows[0].nbCells && table.nbColumns == 0) {
        printf("Please provide a value for each column or use explicit mode.\n");
        return 0;
    }
    
    int isImplicit = (table.nbColumns == 0) ? 1 : 0;
    
    char newRows[table.nbRows][STRING_SIZE];
    int newRowsSize = 0;
    
    // foreach row -> foreach column name -> foreach column name given
    // if column name given == column name, save row value
    
    for (int i = 0; i < table.nbRows; ++i) {
        char row[STRING_SIZE] = "";
        strcat(row, "- [");
        
        for (int j = 0; j < columnNames.size; ++j) {
            
            if (isImplicit) {
                // we should simply fill all values of the row
                strcat(row, "\"");
                strcat(row, table.rows[i].cells[j].data);
                strcat(row, "\",");
                continue;
            }
            
            int found = 0;
            
            for (int k = 0; k < table.nbColumns;  ++k) {
                if (strcmp(columnNames.data[j], table.columns[k].name) != 0) {
                    continue;
                }
                
                strcat(row, "\"");
                strcat(row, table.rows[i].cells[k].data);
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
    
    removeDataTilde(database, table);
    
    // write in file
    char *tablePath = createFileInDirPath(table.name, database.name);
    
    FILE *file = fopen(tablePath, "a");
    
    if (file == NULL) {
        free(tablePath);
        printf("fopen() failed in file %s at line #%d\n", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
    
    for (int i = 0; i < newRowsSize; ++i) {
        fprintf(file, TAB TAB "%s", newRows[i]);
    }
    
    fclose(file);
    
    // free memory
    for (int i = 0; i < columnNames.size; ++i) {
        free(columnNames.data[i]);
    }
    free(columnNames.data);
    free(tablePath);
    
    printf("New row added to `%s`.\n", table.name);
    return 1;
}

//
// Update row(s) of a given table depending of some conditions (`where` SQL clause)
//
void updateRows(
                const Database database,
                const Table table,
                const int nbValues,
                const char **values,
                const int nbConditions,
                const Condition *conditions
                )
{
    // get all column names
    StringArray columnNames = getColumnNames(database, table);
    
    char *tablePath = createFileInDirPath(table.name, database.name);
    FILE *file = fopen(tablePath, "r");
    
    if (file == NULL) {
        free(tablePath);
        printf("fopen() failed in file %s at line #%d\n", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
    
    char line[STRING_SIZE] = "";
    int pos = 0;
    int isData = 0;
    
    while (fgets(line, STRING_SIZE, file) != NULL) {
        if (strcmp(line, TAB "data:\n") == 0) {
            // data reading begins
            isData = 1;
            ++pos;
            continue;
        }
        
        if (!isData) {
            // this is not a data line, continue
            ++pos;
            continue;
        }
        
        if (strcmp(line, TAB "data: ~\n") == 0) {
            // this table has no data, stop here
            break;
        }
        
        // create a `row` array which is an array of current line values
        StringArray row = parseRow(line);
        int needUpdate = 0;
        
        // check if row is concerned by `where` claused
        // foreach column in table -> foreach condition
        // if condition concerned column x, if row value matches for column x, row needs update
        for (int i = 0; i < columnNames.size; ++i) {
            for (int j = 0; j < nbConditions; ++j) {
                if (strcmp(conditions[j].type, "=") == 0) {
                    if (strcmp(conditions[j].column, columnNames.data[i]) == 0) {
                        if (strcmp(conditions[j].value, row.data[i]) == 0) {
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
        
        /****
        
        si c'est un OR et qu'il est valide, peu importe le résultat précédent, cela devient 1
        si c'est un OR et qu'il est faux, on garde le résultat précédent
        si c'est un AND et qu'il est valide, si le résultat précédent est bon, on reste à 1
        si c'est un AND et qu'il est valide, si le résultat précédent est faux, on reste à 0
        si c'est un AND et qu'il est faux, on passe à 0
         
        ****/
        
        if (needUpdate) {
            // create the new line
            char newLine[STRING_SIZE] = TAB TAB "- [";
            
            for (int i = 0; i < columnNames.size; ++i) {
                int find = 0;
                strcat(newLine, "\"");
                for (int j = 0; j < table.nbColumns; ++j) {
                    if (strcmp(columnNames.data[i], table.columns[j].name) == 0) {
                        find = 1;
                        strcat(newLine, values[j]);
                    }
                }
                
                if (!find) {
                    strcat(newLine, row.data[i]);
                }
                
                strcat(newLine, "\",");
            }
            
            newLine[strlen(newLine)-1] = '\0';
            strcat(newLine, "]\n");
            
            // replace line in file
            replaceLine(tablePath, pos, newLine);
        }
        
        for (int i = 0; i < row.size; ++i) {
            free(row.data[i]);
        }
        free(row.data);
        
        ++pos;
    }
    
    fclose(file);
    
    for (int i = 0; i < columnNames.size; ++i) {
        free(columnNames.data[i]);
    }
    free(columnNames.data);
    free(tablePath);
}

//
// Delete row(s) of a given table depending of some conditions (`where` SQL clause)
//
void deleteRows(const Database database, const Table table, const int nbConditions, const Condition *conditions)
{
    // get all column names
    StringArray columnNames = getColumnNames(database, table);
    
    // prepare an array `rowsToDelete` to save all line positions of rows that should be deleted
    IntArray rowsToDelete = {
        malloc(ARRAY_CAPACITY * sizeof(int)),
        0,
        ARRAY_CAPACITY
    };
    
    char *tablePath = createFileInDirPath(table.name, database.name);
    FILE *file = fopen(tablePath, "r");
    
    if (file == NULL) {
        free(tablePath);
        printf("fopen() failed in file %s at line #%d\n", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
    
    char line[STRING_SIZE] = "";
    int pos = 0;
    int isData = 0;
    
    while (fgets(line, STRING_SIZE, file) != NULL) {
        if (strcmp(line, TAB "data:\n") == 0) {
            // data reading begins
            isData = 1;
            ++pos;
            continue;
        }
        
        if (!isData) {
            // this is not a data line, continue
            ++pos;
            continue;
        }
        
        if (strcmp(line, TAB "data: ~\n") == 0) {
            // this table has no data, stop here
            break;
        }
        
        // create a `row` array which is an array of current line values
        StringArray row = parseRow(line);
        int shouldBeDeleted = 0;
        
        // check if row is concerned by `where` claused
        // foreach column in table -> foreach condition
        // if condition concerned column x, if row value matches for column x, row should be deleted
        for (int i = 0; i < columnNames.size; ++i) {
            for (int j = 0; j < nbConditions; ++j) {
                if (strcmp(conditions[j].type, "=") == 0) {
                    if (strcmp(conditions[j].column, columnNames.data[i]) == 0) {
                        if (strcmp(conditions[j].value, row.data[i]) == 0) {
                            shouldBeDeleted = 1;
                        } else {
                            shouldBeDeleted = 0;
                        }
                    }
                } else if (strcmp(conditions[i].type, ">") == 0) {
                    // TODO
                } else if (strcmp(conditions[i].type, "<") == 0) {
                    // TODO
                }
            }
        }
        
        if (shouldBeDeleted) {
            // save pos of line that should be deleted to `rowToDelete` array
            rowsToDelete = appendValueToIntArray(rowsToDelete, pos);
        }
        
        for (int i = 0; i < row.size; ++i) {
            free(row.data[i]);
        }
        free(row.data);
        
        ++pos;
    }
    
    fclose(file);
    
    // remove all matching rows
    for (int i = 0; i < rowsToDelete.size; ++i) {
        removeLine(tablePath, rowsToDelete.data[i]-i);
    }
    
    // free memory
    for (int i = 0; i < columnNames.size; ++i) {
        free(columnNames.data[i]);
    }
    free(columnNames.data);
    free(rowsToDelete.data);
    free(tablePath);
}
