//
//  Filename: manage_database.c
//  Made by: Léo Legron
//  Date: 10/11/2017
//  Description: Parsing function
//

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../headers/manage_entry.h"
#include "../headers/special_parsing.h"
#include "../headers/conf.h"


// Global parsing

void parseEntry(char* entry){
    
    unsigned long entryLength = strlen(entry);
    
    if ((strcmp(entry, "\n") == 0) ||(strcmp(entry, ";\n") == 0)) {
        return;
    }
    else if (entry[entryLength-2] != ';'){
        printf("Please enter a valid SQL command\n");
        printf("Check this website for more information : http://sql.sh/\n");
        return;
    }
    
    // Get the number of words from entry to optimize parsedEntry array
    
    int numberOfWords = 0;
    int inBrackets = 0;
    
    for (int i = 0; i <= entryLength; i++){
        
        const char value = entry[i];
        const char value2 = entry[i+1];
        
        if((value == ' ' && inBrackets == 0)){
            numberOfWords++;
        }
        else if((value == '(' && inBrackets == 0) || (value == 34 && inBrackets == 0) || (value == 39 && inBrackets == 0)){
            inBrackets = 1;
        }
        else if((value == ')' && value2 == ';' && inBrackets == 1) || (value2 == ';' && inBrackets == 0)){
            inBrackets = 0;
            numberOfWords++;
            break;
        }
        else if((value == 34 && inBrackets == 1) || (value == 39 && inBrackets == 1)){
            inBrackets = 0;
        }
        
    }
    
    if(numberOfWords == 0){
        printf("Please enter a valid SQL command\n");
        printf("Check this website for more information : http://sql.sh/\n");
        return;
    }
    
    // Create a array with words from entry
    
    char *parsedEntry[numberOfWords];
    
    for(int i=0; i < numberOfWords; i++){
        parsedEntry[i] = malloc(sizeof(char*)*100);
    }
    
    // Array initialization
    
    for (int i = 0; i < numberOfWords; i++){
        strcpy(parsedEntry[i], "");
    }
    
    // Array association
    
    char character[2];
    int inQuote = 0;

    for (int i = 0, j = 0; i < entryLength; i++){
        
        const char value = entry[i];
        const char value2 = entry[i+1];
        
        character[0] = entry[i];
        character[1] = '\0';
        
        if((value == ' ' && inBrackets == 0 && inQuote == 0)){
            j++;
        }
        else if((value == '(' && inBrackets == 0 && inQuote == 0)){
            strcat(parsedEntry[j], character);
            inBrackets = 1;
        }
        else if((value == 34 && inBrackets == 0 && inQuote == 0) || (value == 39 && inBrackets == 0 && inQuote == 0)){
            strcat(parsedEntry[j], character);
            inQuote = 1;
        }
        else if((value == 34 && inBrackets == 0 && inQuote == 1) || (value == 39 && inBrackets == 0 && inQuote == 1)){
            strcat(parsedEntry[j], character);
            inQuote = 0;
        }
        else if((value == ')' && value2 == ';' && inBrackets == 1)){
            strcat(parsedEntry[j], character);
            strcat(parsedEntry[j], ";");
            break;
        }
        else if(value == ';' && inBrackets == 0){
            break;
        }
        else{
            strcat(parsedEntry[j], character);
        }
    }
    
    const char *SELECT = "SELECT";
    const char *INSERT = "INSERT";
    const char *DELETE = "DELETE";
    const char *UPDATE = "UPDATE";
    const char *CREATE = "CREATE";
    const char *DATABASE = "DATABASE";
    const char *TABLE = "TABLE";
    const char *DATABASES = "DATABASES";
    const char *TABLES = "TABLES";
    const char *DROP = "DROP";
    const char *SHOW = "SHOW";
    const char *USE = "USE";
    
    int numberOfValue = 0;
    
    if (strcmp(parsedEntry[0], SELECT) == 0) {
        
        // INNER JOIN SELECT
        
        if (numberOfWords > 4) {
            printf("launch the SELECT function with an INNER JOIN\n");
            printf("select : %s\n", parsedEntry[1]); // parsedEntry[1] is what to select
            printf("from table : %s\n", parsedEntry[3]); // parsedEntry[3] is the first table
            printf("join with : %s\n", parsedEntry[6]); // parsedEntry[6] is the second table
            printf("on : %s ", parsedEntry[8]); // parsedEntry[8] is the column from first table
            printf("equals %s\n", parsedEntry[10]); // parsedEntry[10] is the column from second table
        }
        
         // BASIC SELECT
        
        else{
            printf("launch the SELECT function\n");
            printf("select : %s\n", parsedEntry[1]); // parsedEntry[1] is what to select
            printf("from table : %s\n", parsedEntry[3]); // parsedEntry[3] is the table
        }
        
        
    }
    
    // INSERT INTO
    
    else if (strcmp(parsedEntry[0], INSERT) == 0) {
        
        char *valuesToParse = parsedEntry[4];
        char **values = getValuesOfInsert(valuesToParse, &numberOfValue);
        
        printf("launch the INSERT INTO function\n");
        printf("name of the table : ");
        printf("%s\n", parsedEntry[2]); // parsedEntry[2] is the table
        for (int i = 0; i < numberOfValue; i++) {
            printf("values %d :", i+1); // values is the array of values
            printf(" %s\n", values[i]); // numberOfValue is the number of values in the array
        }
    }
    
    // DELETE FROM
    
    else if (strcmp(parsedEntry[0], DELETE) == 0) {
        printf("launch the DELETE FROM function\n");
        printf("name of the table : %s\n", parsedEntry[2]); // parsedEntry[2] is the table
        printf("Column : %s\n", parsedEntry[4]); // parsedEntry[4] is the column
        printf("Operator : %s\n", parsedEntry[5]); // parsedEntry[5] is the operator
        printf("Value : %s\n", parsedEntry[6]); // parsedEntry[6] is the value to check
    }
    
    // UPDATE
    
    else if (strcmp(parsedEntry[0], UPDATE) == 0) {
        replaceQuotes(numberOfWords, parsedEntry);
        printf("launch the UPDATE function\n");
        printf("Table name : %s\n", parsedEntry[1]); // parsedEntry[1] is the table
        printf("Column name : %s\n", parsedEntry[3]); // parsedEntry[3] is the column where we change the value
        printf("New value : %s\n", parsedEntry[5]); // parsedEntry[5] is the new value
        printf("Column : %s\n", parsedEntry[7]); // parsedEntry[7] is the column
        printf("Operator : %s\n", parsedEntry[8]); // parsedEntry[8] is the operator
        printf("Value : %s\n", parsedEntry[9]); // parsedEntry[9] is the value
        
    }
    
    else if(strcmp(parsedEntry[0], CREATE) == 0){
        
        // CREATE DATABASE
        
        if(strcmp(parsedEntry[1], DATABASE) == 0){
            printf("createDatabase(%s);\n", parsedEntry[2]); // parsedEntry[2] is the name of the database
        }
        
        // CREATE TABLE ( Primary key, not null, auto increment not supported yet ) WIP
        
        else if(strcmp(parsedEntry[1], TABLE) == 0){
            
            char *valuesToParse = parsedEntry[3];
            Column *columnOfTableCreation = getValuesOfTableCreation(valuesToParse, &numberOfValue);
        
            printf("launch the CREATE TABLE function\n");
            printf("name of the table : ");
            printf("%s\n", parsedEntry[2]); // parsedEntry[2] is the name of the table
            printf("values :\n");
            for (int i = 0; i < numberOfValue; i++){
                printf("name : %s\n", columnOfTableCreation[i].name); // columnOfTableCreation is an array of Column structure
                printf("type : %s\n", columnOfTableCreation[i].type); // numberOfValue is the number of Column structure
                printf("length : %d\n", columnOfTableCreation[i].length); // For now only 3 fields are fill (name, type and length)
            }
            
            free(columnOfTableCreation);
        }
    }
    
    else if(strcmp(parsedEntry[0], DROP) == 0){
        
        if (strcmp(parsedEntry[1], DATABASE) == 0) {
            printf("dropDatabase(%s);\n", parsedEntry[2]); // parsedEntry[2] is the name of the database
        }
        else if (strcmp(parsedEntry[1], TABLE) == 0) {
            printf("dropTable(%s);\n", parsedEntry[2]); // parsedEntry[2] is the name of the database
        }
        
    }
    
    else if(strcmp(parsedEntry[0], SHOW) == 0){
        
        if (strcmp(parsedEntry[1], DATABASES) == 0) {
            printf("showDatabases();\n");
        }
        else if (strcmp(parsedEntry[1], TABLES) == 0) {
            printf("showTables();\n");
        }
        
    }
    
    else if(strcmp(parsedEntry[0], USE) == 0){
        printf("use(%s);\n", parsedEntry[1]);
    }
    
    else{
        printf("Please enter a valid SQL command\n");
        printf("Check this website for more information : http://sql.sh/\n");
    }
    
    free(*parsedEntry);
}
