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

static void freeColumns(Column *columns, int *nbColumns);
static int checkWhenConditionSucceed(Condition condition, int *shouldChange);
static int checkWhenConditionFailed(Condition condition, int *shouldChange);

//
// Parse a row string `- ["1","2","3","4"]` to an array of values
// Return an array of string
//
StringArray* parseRow(char *row)
{
    // clean row before manipulate it
    trimLeadingSpaces(row);
    
    StringArray *rowToArray = createStringArray();
    
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
            appendToStringArray(rowToArray, value);
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
    
    int *nbColumns = malloc(sizeof(int));
    Column *columns = getAllColumns(database, table, nbColumns);
    
    if (*nbColumns != table.rows[0].nbCells && table.nbColumns == 0) {
        
        freeColumns(columns, nbColumns);
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
        
        for (int j = 0; j < *nbColumns; ++j) {
            
            if (isImplicit) {
                // we should simply fill all values of the row
                strcat(row, "\"");
                strcat(row, table.rows[i].cells[j].data);
                strcat(row, "\",");
                continue;
            }
            
            int found = 0;
            
            for (int k = 0; k < table.nbColumns;  ++k) {
                if (strcmp(columns[j].name, table.columns[k].name) != 0) {
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
        freeColumns(columns, nbColumns);
        free(tablePath);
        printf("fopen() failed in file %s at line #%d\n", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
    
    for (int i = 0; i < newRowsSize; ++i) {
        fprintf(file, TAB TAB "%s", newRows[i]);
    }
    
    fclose(file);
    
    freeColumns(columns, nbColumns);
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

    int *nbColumns = malloc(sizeof(int));
    Column *columns = getAllColumns(database, table, nbColumns);
    
    char *tablePath = createFileInDirPath(table.name, database.name);
    FILE *file = fopen(tablePath, "r");
    
    if (file == NULL) {
        freeColumns(columns, nbColumns);
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
        StringArray *row = parseRow(line);
        int needUpdate = 0;
        
        // check if row is concerned by `where` claused
        // foreach column in table -> foreach condition
        // if condition concerned column x, if row value matches for column x, row needs update
        for (int i = 0; i < *nbColumns; ++i) {
            for (int j = 0; j < nbConditions; ++j) {
                if (strcmp(conditions[j].type, "=") == 0) {
                    if (strcmp(conditions[j].column, columns[i].name) == 0) {
                        if (strcmp(conditions[j].value, row->data[i]) == 0) {
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
            // create the new line
            char newLine[STRING_SIZE] = TAB TAB "- [";
            
            for (int i = 0; i < *nbColumns; ++i) {
                int find = 0;
                strcat(newLine, "\"");
                for (int j = 0; j < table.nbColumns; ++j) {
                    if (strcmp(columns[i].name, table.columns[j].name) == 0) {
                        find = 1;
                        strcat(newLine, values[j]);
                    }
                }
                
                if (!find) {
                    strcat(newLine, row->data[i]);
                }
                
                strcat(newLine, "\",");
            }
            
            newLine[strlen(newLine)-1] = '\0';
            strcat(newLine, "]\n");
            
            // replace line in file
            replaceLine(tablePath, pos, newLine);
        }
        
        freeStringArray(row);
        
        ++pos;
    }
    
    fclose(file);
    
    freeColumns(columns, nbColumns);
    free(tablePath);
}

//
// Delete row(s) of a given table depending of some conditions (`where` SQL clause)
//
int deleteRows(const Database database, const Table table, const int nbConditions, const Condition *conditions)
{
    if (!hasTable(database, table)) {
        printf("Oops! Table `%s` doesn't exist.\n", table.name);
        return 0;
    }
    
    int *nbColumns = malloc(sizeof(int));
    Column *columns = getAllColumns(database, table, nbColumns);
    
    // prepare an array `rowsToDelete` to save all line positions of rows that should be deleted
    IntArray *rowsToDelete = createIntArray();
    
    char *tablePath = createFileInDirPath(table.name, database.name);
    FILE *file = fopen(tablePath, "r");
    
    if (file == NULL) {
        freeColumns(columns, nbColumns);
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
        StringArray *row = parseRow(line);
        int shouldBeDeleted = 0;
        int shouldStop = 0;
        
        for (int i = 0; i < nbConditions; ++i) {
            if (shouldStop) {
                break;
            }
            
            for (int j = 0; j < *nbColumns; ++j) {
                if (strcmp(conditions[i].column, columns[j].name) == 0) {
                    // Condition `=`
                    if (strcmp(conditions[i].type, "=") == 0) {
                        if (strcmp(conditions[i].value, row->data[j]) == 0) {
                            shouldStop = checkWhenConditionSucceed(conditions[i], &shouldBeDeleted);
                        } else {
                            shouldStop = checkWhenConditionFailed(conditions[i], &shouldBeDeleted);
                        }
                    } else if (strcmp(conditions[i].type, "!=") == 0) {
                        if (strcmp(conditions[i].value, row->data[j]) != 0) {
                            shouldStop = checkWhenConditionSucceed(conditions[i], &shouldBeDeleted);
                        } else {
                            shouldStop = checkWhenConditionFailed(conditions[i], &shouldBeDeleted);
                        }
                    }
                }
            }
        }
        
        if (shouldBeDeleted) {
            // save pos of line that should be deleted to `rowToDelete` array
            appendToIntArray(rowsToDelete, pos);
        }
        
        freeStringArray(row);
        
        ++pos;
    }
    
    fclose(file);
    
    // remove all matching rows
    for (int i = 0; i < rowsToDelete->size; ++i) {
        removeLine(tablePath, rowsToDelete->data[i]-i);
    }
    
    if (rowsToDelete->size == 0) {
        // free memory
        freeColumns(columns, nbColumns);
        freeIntArray(rowsToDelete);
        free(tablePath);
        
        printf("Oops. No matching row found in `%s`.\n", table.name);
        return 0;
    }
    
    // free memory
    freeColumns(columns, nbColumns);
    freeIntArray(rowsToDelete);
    free(tablePath);
    
    printf("Row(s) deleted from `%s`.\n", table.name);
    return 1;
}

//
// If condition succeed:
// Check if condition matches depending on previous one and logical operator
//
int checkWhenConditionSucceed(Condition condition, int *shouldChange)
{
    if (strcmp(condition.logicalOperator, "OR") == 0 && *shouldChange) {
        return 1;
    } else if (strcmp(condition.logicalOperator, "OR") == 0 && !*shouldChange) {
        *shouldChange = 1;
    } else if (strcmp(condition.logicalOperator, "AND") == 0 && *shouldChange) {
        *shouldChange = 1;
    } else if (strcmp(condition.logicalOperator, "AND") == 0 && !*shouldChange) {
        *shouldChange = 0;
    } else {
        *shouldChange = 1;
    }

    return 0;
}

//
// If condition failed:
// Check if condition matches depending on previous one and logical operator
//
int checkWhenConditionFailed(Condition condition, int *shouldChange)
{
    if (strcmp(condition.logicalOperator, "OR") == 0 && shouldChange) {
        return 1;
    } else if (strcmp(condition.logicalOperator, "OR") == 0 && !*shouldChange) {
        *shouldChange = 0;
    } else if (strcmp(condition.logicalOperator, "AND") == 0 && *shouldChange) {
        *shouldChange = 0;
    } else if (strcmp(condition.logicalOperator, "AND") == 0 && !*shouldChange) {
        *shouldChange = 0;
    } else {
        *shouldChange = 0;
    }
    
    return 0;
}

//
// Free an existing `columns` pointer with its related `nbColumns` pointer
//
void freeColumns(Column *columns, int *nbColumns)
{
    for (int i = 0; i < *nbColumns; ++i) {
        free(columns[i].name);
        free(columns[i].type);
    }
    free(columns);
    free(nbColumns);
}
