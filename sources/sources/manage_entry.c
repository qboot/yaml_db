//
//  Filename: manage_database.c
//  Made by: Léo Legron
//  Date: 10/11/2017
//  Description: Parsing function
//

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../headers/manage_database.h"
#include "../headers/manage_table.h"
#include "../headers/manage_row.h"
#include "../headers/manage_array.h"
#include "../headers/manage_entry.h"
#include "../headers/manage_parsing.h"
#include "../headers/read_file.h"
#include "../headers/read_table.h"

static void showErrorMessage(void);
static void showUserHelp(void);
static void explodeCondition(StringArray *conditions, char *string, char *position, char *delimiter);

//
// Global parsing of user input
//
void parseEntry(Manager manager, char *currentDatabase, char *entry)
{
    if (strcmp(entry, "\n") == 0 || strcmp(entry, ";\n") == 0) {
        return;
    }
    
    if (entry[strlen(entry)-2] != ';') {
        showErrorMessage();
        return;
    }
    
    StringArray *entryArray = createStringArray();
    char entryPart[STRING_SIZE] = "";
    int inParenthesis = 0;
    
    for (int i = 0; i < strlen(entry); ++i) {
        if (i == 0) {
            char charToString[] = {entry[0], '\0'};
            strcat(entryPart, charToString);
            continue;
        }
        
        if (
            (entry[i] == '(' && entry[i-1] != '\\' && !inParenthesis) ||
            (entry[i] == ')' && entry[i-1] != '\\' && inParenthesis) ||
            (entry[i] == ' ' && !inParenthesis) ||
            (entry[i] == ';' && !inParenthesis)
            ) {
            
            if (entry[i] == '(' && entry[i-1] != '\\' && !inParenthesis) {
                inParenthesis = 1;
            }
            
            if (entry[i] == ')' && entry[i-1] != '\\' && inParenthesis) {
                inParenthesis = 0;
            }
            
            if (strcmp(entryPart, "") != 0) {
                appendToStringArray(entryArray, entryPart);
                strcpy(entryPart, "\0");
            }
            
            continue;
        }
        
        char charToString[] = {entry[i], '\0'};
        strcat(entryPart, charToString);
    }
    
    if (strcmp(entryArray->data[0], "HELP") == 0) {
        showUserHelp();
        return;
    }
    
    if (entryArray->size < 2) {
        showErrorMessage();
        return;
    }
    
    int nbColumns = 0;
    
    if (strcmp(entryArray->data[0], "SELECT") == 0) {
        
        if (strcmp(currentDatabase, "") == 0) {
            printf("Oops! No database selected.\n");
            freeStringArray(entryArray);
            return;
        }
        
        Table currentTable = findAllRecords(currentDatabase, entryArray->data[3]);

        // SELECT WHERE
        
        if (entryArray->size > 4) {

            if (strcmp(entryArray->data[6], "=") == 0)
            {
                int columnPlace = searchColumnPlace(&currentTable, entryArray->data[5]);
                currentTable = searchSpecificData(&currentTable, columnPlace, entryArray->data[7]);
            }
            else if (strcmp(entryArray->data[6], "LIKE") == 0)
            {
                int columnPlace = searchColumnPlace(&currentTable, entryArray->data[5]);
                currentTable = searchData(&currentTable, columnPlace, entryArray->data[7]);
            }
            
        }
        
        // SELECT specific column
        if (strcmp(entryArray->data[1], "*") != 0)
        {
            searchSpecificColumn(&currentTable, entryArray->data[1]);
        }

        printAllResult(&currentTable);
        
        
    }
    
    // INSERT INTO
    
    else if (strcmp(entryArray->data[0], "INSERT") == 0 && strcmp(entryArray->data[1], "INTO") == 0) {
        
        if (strcmp(currentDatabase, "") == 0) {
            printf("Oops! No database selected.\n");
            freeStringArray(entryArray);
            return;
        }
        
        if (entryArray->size < 6 && entryArray->size != 4) {
            showErrorMessage();
            freeStringArray(entryArray);
            return;
        }

        // check if explicit column names have been passed
        Row newRows[] = {(entryArray->size == 4) ? getRow(entryArray->data[3]) : getRow(entryArray->data[5])};
        Column *columns = (entryArray->size == 4) ? NULL : getColumns(entryArray->data[3], &nbColumns, 1);
        Table table = {entryArray->data[2], nbColumns, 1, columns, newRows};
        Database database = {currentDatabase};
        
        addRows(database, table);
        
        // free memory
        for (int i = 0; i < nbColumns; ++i) {
            free(columns[i].name);
            free(columns[i].type);
        }
        free(columns);
        
        for (int i = 0; i < newRows[0].nbCells; ++i) {
            free(newRows[0].cells[i].data);
        }
        free(newRows[0].cells);
    }
    
    // DELETE FROM
    
    else if (strcmp(entryArray->data[0], "DELETE") == 0 && strcmp(entryArray->data[1], "FROM") == 0) {

        if (strcmp(currentDatabase, "") == 0) {
            printf("Oops! No database selected.\n");
            freeStringArray(entryArray);
            return;
        }
        
        if (entryArray->size < 5 && entryArray->size != 3) {
            showErrorMessage();
            freeStringArray(entryArray);
            return;
        }
        
        // if entryArray.size == 3, there is no WHERE clause, delete all rows
        // freeStringArray
        // return
        
        StringArray *conditionsArray = createStringArray();

        for (int i = 4; i < entryArray->size; ++i) {

            char string[STRING_SIZE] = "";
            strcat(string, entryArray->data[i]);

            if (strlen(string) < 3) {
                appendToStringArray(conditionsArray, string);
                continue;
            }

            char *isDifferent = strstr(string, "!=");
            if (isDifferent != NULL) {
                explodeCondition(conditionsArray, string, isDifferent, "!=");
                continue;
            }

            char *isLessThanOrEqual = strstr(string, "<=");
            if (isLessThanOrEqual != NULL) {
                explodeCondition(conditionsArray, string, isLessThanOrEqual, "<=");
                continue;
            }

            char *isGreaterThanOrEqual = strstr(string, ">=");
            if (isGreaterThanOrEqual != NULL) {
                explodeCondition(conditionsArray, string, isGreaterThanOrEqual, ">=");
                continue;
            }

            char *isLessThan = strstr(string, "<");
            if (isLessThan != NULL) {
                explodeCondition(conditionsArray, string, isLessThan, "<");
                continue;
            }

            char *isGreaterThan = strstr(string, ">");
            if (isGreaterThan != NULL) {
                explodeCondition(conditionsArray, string, isGreaterThan, ">");
                continue;
            }

            char *isEqual = strstr(string, "=");
            if (isEqual != NULL) {
                explodeCondition(conditionsArray, string, isEqual, "=");
                continue;
            }

            appendToStringArray(conditionsArray, string);
        }

        // WHERE clause is not resolvable
        if (conditionsArray->size % 4 - 3 != 0) {
            showErrorMessage();
            freeStringArray(entryArray);
            freeStringArray(conditionsArray);
            return;
        }
        
        // hydrate conditions
        int nbConditions = conditionsArray->size / 4 + 1;
        Condition conditions[nbConditions];
        
        for (int i = 0, j = 0; i < conditionsArray->size;) {
            
            conditions[j].column = conditionsArray->data[i];
            conditions[j].type = conditionsArray->data[i+1];
            conditions[j].value = conditionsArray->data[i+2];
            
            if (i == 0) {
                conditions[j].logicalOperator = "NULL";
            } else {
                conditions[j].logicalOperator = conditionsArray->data[i-1];
            }
            
            i += 4;
            ++j;
        }
        
        // DELETE FROM test WHERE id=23 AND age<=19 OR name=quentin;
        Table table = {entryArray->data[2], 0, 0, NULL, NULL};
        Database database = {currentDatabase};
        
        deleteRows(database, table, nbConditions, conditions);

        // free
        freeStringArray(conditionsArray);
    }
    
    // UPDATE
    
    else if (strcmp(entryArray->data[0], "UPDATE") == 0) {
        // replaceQuotes(entryArray.data, entryArray.size);
        printf("launch the UPDATE function\n");
        printf("Table name : %s\n", entryArray->data[1]); // parsedEntry[1] is the table
        printf("Column name : %s\n", entryArray->data[3]); // parsedEntry[3] is the column where we change the value
        printf("New value : %s\n", entryArray->data[5]); // parsedEntry[5] is the new value
        printf("Column : %s\n", entryArray->data[7]); // parsedEntry[7] is the column
        printf("Operator : %s\n", entryArray->data[8]); // parsedEntry[8] is the operator
        printf("Value : %s\n", entryArray->data[9]); // parsedEntry[9] is the value
        
    }
    
    else if (strcmp(entryArray->data[0], "CREATE") == 0) {
        
        // CREATE DATABASE
        
        if (strcmp(entryArray->data[1], "DATABASE") == 0) {
            
            if (entryArray->size < 3) {
                showErrorMessage();
                freeStringArray(entryArray);
                return;
            }
            
            Database newDatabase = {entryArray->data[2]};
            
            if (createDatabase(manager, newDatabase)) {
                // if success, change the current database to the new one
                strcpy(currentDatabase, newDatabase.name);
                printf("Current database is now: `%s`.\n", currentDatabase);
            }
        }
        
        // CREATE TABLE
        // ( Primary key, not null, auto increment not supported yet ) WIP
        
        else if (strcmp(entryArray->data[1], "TABLE") == 0) {
            
            if (entryArray->size < 4) {
                showErrorMessage();
                freeStringArray(entryArray);
                return;
            }
            
            if (strcmp(currentDatabase, "") == 0) {
                printf("Oops! No database selected.\n");
                freeStringArray(entryArray);
                return;
            }
            
            Column *newColumns = getColumns(entryArray->data[3], &nbColumns, 0);
            Table newTable = {entryArray->data[2], nbColumns, 0, newColumns};
            
            Database database = {currentDatabase};
            
            createTable(database, newTable);
            
            for (int i = 0; i < nbColumns; ++i) {
                free(newColumns[i].name);
                free(newColumns[i].type);
            }
            free(newColumns);
        }
    }
    
    else if(strcmp(entryArray->data[0], "DROP") == 0) {
        
        if (entryArray->size < 3) {
            showErrorMessage();
            freeStringArray(entryArray);
            return;
        }
        
        if (strcmp(entryArray->data[1], "DATABASE") == 0) {
            Database oldDatabase = {entryArray->data[2]};
            
            if (dropDatabase(manager, oldDatabase)) {
                // if success && currentDatabase is dropped database
                // change the current database to null
                if (strcmp(currentDatabase, oldDatabase.name) == 0) {
                    strcpy(currentDatabase, "");
                }
            }
        }
        else if (strcmp(entryArray->data[1], "TABLE") == 0) {
            
            if (strcmp(currentDatabase, "") == 0) {
                printf("Oops! No database selected.\n");
                freeStringArray(entryArray);
                return;
            }
            
            Table oldTable = {entryArray->data[2]};
            Database database = {currentDatabase};
            
            dropTable(database, oldTable);
        }
        
    }
    
    else if(strcmp(entryArray->data[0], "SHOW") == 0){
        
        if (strcmp(entryArray->data[1], "DATABASES") == 0) {
            showDatabases();
        }
        else if (strcmp(entryArray->data[1], "TABLES") == 0) {
            printf("showTables();\n");
        }
        
    }
    
    else if (strcmp(entryArray->data[0], "USE") == 0) {
        
        Database database = {entryArray->data[1]};
        
        if (!isDatabase(database)) {
            printf("Oops! Database `%s` doesn't exist.\n", database.name);
            freeStringArray(entryArray);
            return;
        }
        
        strcpy(currentDatabase, database.name);
        printf("Current database is now: `%s`.\n", currentDatabase);
    }
    
    else {
        showErrorMessage();
    }
    
    freeStringArray(entryArray);
}

//
// Print default error message : invalid SQL command
//
void showErrorMessage()
{
    printf("Please enter a valid SQL command.\n");
    printf("Check this website for more informations: http://sql.sh/.\n");
}

void explodeCondition(StringArray *conditions, char *string, char *position, char *delimiter)
{
    int beginning = (int) (position - string + strlen(delimiter));
    int length = (int) strlen(string) - beginning;
    
    char column[STRING_SIZE] = "";
    strncpy(column, string, strlen(string) - length - strlen(delimiter));
    appendToStringArray(conditions, column);
    
    appendToStringArray(conditions, delimiter);
    
    char value[STRING_SIZE] = "";
    strncpy(value, string+beginning, length);
    appendToStringArray(conditions, value);
}

//
// Provide help to the users, with the query HELP;
//
void showUserHelp()
{
    printf("\nWelcome to the user manual!\n");
    printf("\n");
    printf("----------------------------\n");
    printf("   -                    -   \n");
    printf("  -     USER MANUAL      -  \n");
    printf("   -                    -   \n");
    printf("----------------------------\n");
    printf("\nWrite your query after : \"yaml_db >\"\n");
    printf("\nKeywords from SQL must be written in capital\n");
    printf("Query must end with \";\"\n");
    printf("\nUsers can write values with quotes, only if the values is a string\n");
    printf("Users cans use simple or double quote.\n");
    printf("\n\"INSERT INTO\" query works with explicit and implicit column name.\n");
    printf("With explicit name, users just have to specify some colmun name\n");
    printf("With implicit users must set value to every column\n");
    printf("\nTo read datas, users can select all columns, a specific column.\n");
    printf("Users can use where condition.\n");
    printf("Where coniditon works with \"LIKE\" or with \"=\".\n");
    printf("\nHere is some query exemples users can use :\n\n");
    printf("SHOW DATABASES;\n");
    printf("USE db_name;\n");
    printf("CREATE TABLE table_name (col1 type, col2 type, ...);\n");
    printf("INSERT INTO table_name (col1, col2) VALUES (val1, val2);\n");
    printf("INSERT INTO table_name (val1, val2);\n");
    printf("UPDATE table_name SET col1 = val1, col2 = val2 WHERE id = 1 OR name = quentin;\n");
    printf("DELETE FROM table_name WHERE id = 1 AND name != quentin OR age = 22;\n");
    printf("DROP TABLE table_name;\nDROP DATABASE db_name;\n");
    printf("EXIT; QUIT;\n");
    return;
}
