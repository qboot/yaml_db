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
#include "../headers/manage_array.h"
#include "../headers/manage_entry.h"
#include "../headers/manage_parsing.h"

//
// Global parsing of user input
//
void parseEntry(Manager manager, Database* currentDatabase, char* entry) {
    
    if (strcmp(entry, "\n") == 0 || strcmp(entry, ";\n") == 0) {
        return;
    }
    
    if (entry[strlen(entry)-2] != ';') {
        printf("Please enter a valid SQL command.\n");
        printf("Check this website for more information : http://sql.sh/\n");
        return;
    }
    
    StringArray entryArray = {malloc(ARRAY_CAPACITY * sizeof(int)), 0, ARRAY_CAPACITY};
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
                entryArray = appendValueToStringArray(entryArray, entryPart);
                strcpy(entryPart, "\0");
            }
            
            continue;
        }
        
        char charToString[] = {entry[i], '\0'};
        strcat(entryPart, charToString);
    }
    
    if (entryArray.size == 0) {
        printf("Please enter a valid SQL command.\n");
        printf("Check this website for more information : http://sql.sh/\n");
        return;
    }
    
    int nbColumns = 0;
    
    if (strcmp(entryArray.data[0], "SELECT") == 0) {
        
        // INNER JOIN SELECT
        
        if (entryArray.size > 4) {
            printf("launch the SELECT function with an INNER JOIN\n");
            printf("select : %s\n", entryArray.data[1]); // parsedEntry[1] is what to select
            printf("from table : %s\n", entryArray.data[3]); // parsedEntry[3] is the first table
            printf("join with : %s\n", entryArray.data[6]); // parsedEntry[6] is the second table
            printf("on : %s ", entryArray.data[8]); // parsedEntry[8] is the column from first table
            printf("equals %s\n", entryArray.data[10]); // parsedEntry[10] is the column from second table
        }
        
         // BASIC SELECT
        
        else {
            printf("launch the SELECT function\n");
            printf("select : %s\n", entryArray.data[1]); // parsedEntry[1] is what to select
            printf("from table : %s\n", entryArray.data[3]); // parsedEntry[3] is the table
        }
        
    }
    
    // INSERT INTO
    
    else if (strcmp(entryArray.data[0], "INSERT") == 0 && strcmp(entryArray.data[1], "INTO") == 0) {
        
        // TODO: case where no column provided
        // just do an extension of getcolumnsname function which check if number = number
        
        if (strcmp(currentDatabase->name, "") == 0) {
            // no database selected, stop here
            printf("Oops! No database selected.\n");
            return;
        }
        
        Row newRows[] = {getRow(entryArray.data[5])};
        Column *columns = getColumns(entryArray.data[3], &nbColumns, 1);
        Table table = {entryArray.data[2], nbColumns, 1, columns, newRows};
        Database database = {currentDatabase->name};
        
        printf("launch the INSERT INTO function\n");
    }
    
    // DELETE FROM
    
    else if (strcmp(entryArray.data[0], "DELETE") == 0) {
        printf("launch the DELETE FROM function\n");
        printf("name of the table : %s\n", entryArray.data[2]); // parsedEntry[2] is the table
        printf("Column : %s\n", entryArray.data[4]); // parsedEntry[4] is the column
        printf("Operator : %s\n", entryArray.data[5]); // parsedEntry[5] is the operator
        printf("Value : %s\n", entryArray.data[6]); // parsedEntry[6] is the value to check
    }
    
    // UPDATE
    
    else if (strcmp(entryArray.data[0], "UPDATE") == 0) {
        // replaceQuotes(entryArray.data, entryArray.size);
        printf("launch the UPDATE function\n");
        printf("Table name : %s\n", entryArray.data[1]); // parsedEntry[1] is the table
        printf("Column name : %s\n", entryArray.data[3]); // parsedEntry[3] is the column where we change the value
        printf("New value : %s\n", entryArray.data[5]); // parsedEntry[5] is the new value
        printf("Column : %s\n", entryArray.data[7]); // parsedEntry[7] is the column
        printf("Operator : %s\n", entryArray.data[8]); // parsedEntry[8] is the operator
        printf("Value : %s\n", entryArray.data[9]); // parsedEntry[9] is the value
        
    }
    
    else if(strcmp(entryArray.data[0], "CREATE") == 0){
        
        // CREATE DATABASE
        
        if (strcmp(entryArray.data[1], "DATABASE") == 0) {
            Database newDatabase = {entryArray.data[2]}; // parsedEntry[2] is the name of the database
            
            if (createDatabase(manager, newDatabase)) {
                // if success, change the current database to the new one
                currentDatabase->name = newDatabase.name;
                printf("Current database is now: `%s`.\n", currentDatabase->name);
            }
        }
        
        // CREATE TABLE
        // ( Primary key, not null, auto increment not supported yet ) WIP
        
        else if (strcmp(entryArray.data[1], "TABLE") == 0) {
            
            if (strcmp(currentDatabase->name, "") == 0) {
                // no database selected, stop here
                printf("Oops! No database selected.\n");
                return;
            }
            
            Column *newColumns = getColumns(entryArray.data[3], &nbColumns, 0);
            Table newTable = {entryArray.data[2], nbColumns, 0, newColumns};
            Database database = {currentDatabase->name};
            
            createTable(database, newTable);
            
            for (int i = 0; i < nbColumns; ++i) {
                free(newColumns[i].name);
                free(newColumns[i].type);
            }
            free(newColumns);
        }
    }
    
    else if(strcmp(entryArray.data[0], "DROP") == 0){
        
        if (strcmp(entryArray.data[1], "DATABASE") == 0) {
            Database oldDatabase = {entryArray.data[2]}; // parsedEntry[2] is the name of the database
            
            if (dropDatabase(manager, oldDatabase)) {
                // if success && currentDatabase is dropped database
                // change the current database to null
                if (strcmp(currentDatabase->name, oldDatabase.name) == 0) {
                    currentDatabase->name = "";
                }
            }
        }
        else if (strcmp(entryArray.data[1], "TABLE") == 0) {
            
            if (strcmp(currentDatabase->name, "") == 0) {
                // no database selected, stop here
                printf("Oops! No database selected.\n");
                return;
            }
            
            Table oldTable = {entryArray.data[2]};
            Database database = {currentDatabase->name};
            
            dropTable(database, oldTable);
        }
        
    }
    
    else if(strcmp(entryArray.data[0], "SHOW") == 0){
        
        if (strcmp(entryArray.data[1], "DATABASES") == 0) {
            printf("showDatabases();\n");
        }
        else if (strcmp(entryArray.data[1], "TABLES") == 0) {
            printf("showTables();\n");
        }
        
    }
    
    else if (strcmp(entryArray.data[0], "USE") == 0){
        
        Database database = {entryArray.data[1]};
        
        if (!isDatabase(database)) {
            printf("Oops! Database `%s` doesn't exist.\n", database.name);
            return;
        }
        
        currentDatabase->name = database.name;
        printf("Current database is now: `%s`.\n", currentDatabase->name);
    }
    
    else{
        printf("Please enter a valid SQL command.\n");
        printf("Check this website for more information : http://sql.sh/\n");
    }
    
    for (int i = 0; i < entryArray.size; ++i) {
        free(entryArray.data[i]);
    }
    free(entryArray.data);
}
