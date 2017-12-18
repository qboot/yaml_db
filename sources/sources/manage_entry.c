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
static StringArray* explodeEntry(char *entry);
static void explodeCondition(StringArray *conditions, char *string, char *position, char *delimiter);
static StringArray* explodeConditions(StringArray *array, int beginning);
static Condition* createConditionsArray(int nbConditions);
static void freeConditionsArray(Condition *conditions, int nbConditions);

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
    
    StringArray *entryArray = explodeEntry(entry);
    
    if (strcmp(entryArray->data[0], "HELP") == 0) {
        showUserHelp();
        freeStringArray(entryArray);
        return;
    }
    
    if (entryArray->size < 2) {
        showErrorMessage();
        freeStringArray(entryArray);
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
        
        if (entryArray->size < 5) {
            showErrorMessage();
            freeStringArray(entryArray);
            return;
        }
        
        StringArray *conditionsArray = explodeConditions(entryArray, 4);

        // WHERE clause is not resolvable
        if (conditionsArray->size % 4 - 3 != 0) {
            showErrorMessage();
            freeStringArray(entryArray);
            freeStringArray(conditionsArray);
            return;
        }
        
        // hydrate conditions
        int nbConditions = conditionsArray->size / 4 + 1;
        Condition *conditions = createConditionsArray(nbConditions);
        
        for (int i = 0, j = 0; i < conditionsArray->size;) {
            strcpy(conditions[j].column, conditionsArray->data[i]);
            strcpy(conditions[j].type, conditionsArray->data[i+1]);
            strcpy(conditions[j].value, conditionsArray->data[i+2]);
            
            if (i == 0) {
                strcpy(conditions[j].logicalOperator, "NULL");
            } else {
                strcpy(conditions[j].logicalOperator, conditionsArray->data[i-1]);
            }
            
            i += 4;
            ++j;
        }
        
        Table table = {entryArray->data[2], 0, 0, NULL, NULL};
        Database database = {currentDatabase};
        
        deleteRows(database, table, nbConditions, conditions);

        freeStringArray(conditionsArray);
        freeConditionsArray(conditions, nbConditions);
    }
    
    // UPDATE
    
    else if (strcmp(entryArray->data[0], "UPDATE") == 0) {
        
        if (strcmp(currentDatabase, "") == 0) {
            printf("Oops! No database selected.\n");
            freeStringArray(entryArray);
            return;
        }
        
        if (entryArray->size < 4) {
            showErrorMessage();
            freeStringArray(entryArray);
            return;
        }
        
        if (strcmp(entryArray->data[2], "SET") != 0) {
            showErrorMessage();
            freeStringArray(entryArray);
            return;
        }
        
        StringArray *newValuesArray = createStringArray();
        StringArray *conditionsArray = createStringArray();
        int wherePosition = 0;
        
        for (int i = 3; i < entryArray->size; ++i) {
            if (strcmp(entryArray->data[i], "WHERE") == 0) {
                wherePosition = i;
                break;
            }
            trimComma(entryArray->data[i]);
            appendToStringArray(newValuesArray, entryArray->data[i]);
        }
        
        if (wherePosition != 0) {
            for (int i = wherePosition + 1; i < entryArray->size; ++i) {
                appendToStringArray(conditionsArray, entryArray->data[i]);
            }
        }
        
        StringArray *newValues = explodeConditions(newValuesArray, 0);
        StringArray *conditionsExploded = explodeConditions(conditionsArray, 0);
        freeStringArray(newValuesArray);
        freeStringArray(conditionsArray);
        
        
        // WHERE clause is not resolvable
        if (conditionsExploded->size % 4 - 3 != 0) {
            showErrorMessage();
            freeStringArray(entryArray);
            freeStringArray(conditionsExploded);
            freeStringArray(newValues);
            return;
        }
        
        // SET clause is not resolvable
        if (newValues->size % 3 != 0) {
            showErrorMessage();
            freeStringArray(entryArray);
            freeStringArray(conditionsExploded);
            freeStringArray(newValues);
            return;
        }
        
        // hydrate conditions
        int nbConditions = conditionsExploded->size / 4 + 1;
        Condition *conditions = createConditionsArray(nbConditions);
        
        for (int i = 0, j = 0; i < conditionsExploded->size;) {
            strcpy(conditions[j].column, conditionsExploded->data[i]);
            strcpy(conditions[j].type, conditionsExploded->data[i+1]);
            strcpy(conditions[j].value, conditionsExploded->data[i+2]);
            
            if (i == 0) {
                strcpy(conditions[j].logicalOperator, "NULL");
            } else {
                strcpy(conditions[j].logicalOperator, conditionsExploded->data[i-1]);
            }
            
            i += 4;
            ++j;
        }
        
        freeStringArray(conditionsExploded);
        
        StringArray *values = createStringArray();
        Column columns[newValues->size/3];
        
        for (int i = 0, j = 0; i < newValues->size;) {
            columns[j].name = malloc(sizeof(char) * STRING_SIZE);
            strcpy(columns[j].name, newValues->data[i]);
            appendToStringArray(values, newValues->data[i+2]);
            ++j;
            i += 3;
        }
        
        freeStringArray(newValues);
        
        Table table = {entryArray->data[1], newValues->size/3, 0, columns, NULL};
        Database database = {currentDatabase};
        
        updateRows(database, table, values, nbConditions, conditions);
        
        freeConditionsArray(conditions, nbConditions);
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
            if (strcmp(currentDatabase, "") == 0) {
                printf("Oops! No database selected.\n");
                freeStringArray(entryArray);
                return;
            }
            
            Database database = {currentDatabase};
            showTables(database);
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

//
// Explode `entry` string to a StringArray
// Return a StringArray pointer
//
StringArray* explodeEntry(char *entry)
{
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
    
    return entryArray;
}

//
// Explode a `string` depending on a delimiter and its position
// Add the 3 parts (column, delimiter, value) in `conditions` StringArray
//
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
    printf("\nYou should type your query after : \"yaml_db >\".\n");
    printf("All SQL keywords must be written in capital letters.\n");
    printf("Queries must end with \";\".\n");
    printf("\n");
    printf("SHOW DATABASES;\n");
    printf("SHOW TABLES;\n");
    printf("USE database;\n");
    printf("CREATE DATABASE database\n");
    printf("DROP DATABASE database;\n");
    printf("CREATE TABLE table (col1 type, col2 type, ...);\n");
    printf("DROP TABLE table;\n");
    printf("INSERT INTO table (col1, col2) VALUES (val1, val2);\n");
    printf("INSERT INTO table (val1, val2);\n");
    printf("UPDATE table SET col1 = val1, col2 = val2 WHERE col1 = val1 OR col2 = val2;\n");
    printf("DELETE FROM table WHERE col1 = val1 AND col2 != val2 OR col3 = val3;\n");
    printf("EXIT; QUIT;\n");
    return;
}
    
//
// Perform a second explode on an `entryArray` for all stuff WHERE-related
// Let user choose if he wants type id=1 or id = 1
//
StringArray* explodeConditions(StringArray *array, int beginning)
{
    StringArray *conditionsArray = createStringArray();
    
    for (int i = beginning; i < array->size; ++i) {
        
        char string[STRING_SIZE] = "";
        strcat(string, array->data[i]);
        
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
    
    return conditionsArray;
}

Condition* createConditionsArray(int nbConditions)
{
    Condition *conditions = malloc(sizeof(Condition) * nbConditions);
    
    for (int i = 0; i < nbConditions; ++i) {
        conditions[i].column = malloc(sizeof(char) * STRING_SIZE);
        conditions[i].type = malloc(sizeof(char) * STRING_SIZE);
        conditions[i].value = malloc(sizeof(char) * STRING_SIZE);
        conditions[i].logicalOperator = malloc(sizeof(char) * STRING_SIZE);
    }
        return conditions;
}

void freeConditionsArray(Condition *conditions, int nbConditions)
{
    for (int i = 0; i < nbConditions; ++i) {
        free(conditions[i].column);
        free(conditions[i].type);
        free(conditions[i].value);
        free(conditions[i].logicalOperator);
    }
    
    free(conditions);
}
