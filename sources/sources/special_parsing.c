//
//  special_parsing.c
//  yaml_db
//
//  Created by Léo LEGRON on 03/12/2017.
//  Copyright © 2017 Groupe 16. All rights reserved.
//

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../headers/special_parsing.h"
#include "../headers/conf.h"

// Parsing of the INSERT INTO values

char ** getValuesOfInsert(char *valueToParse, int *numberOfValue){
    
    unsigned long valueToParseLength = strlen(valueToParse);
    int inQuote = 0;
    
    for (int i = 1; i < valueToParseLength; i++) {
        
        if ((valueToParse[i] == 34 || valueToParse[i] == 39) && inQuote == 0) {
            inQuote = 1;
        }
        else if ((valueToParse[i] == 34 || valueToParse[i] == 39) && valueToParse[i+1] == ',' && inQuote == 1) {
            inQuote = 0;
        }
        else if(valueToParse[i] == ',' && inQuote == 0){
            *numberOfValue += 1;
        }
        else if(valueToParse[i] == ')' && valueToParse[i+1] == ';'){
            *numberOfValue += 1;
            break;
        }
        
    }

    // Create an array of the Column structure with numberOfValue

    char **valuesOfInsert = malloc(sizeof(double)*(*numberOfValue));

    for(int i=0; i < *numberOfValue; i++){
        valuesOfInsert[i] = malloc(sizeof(char*));
    }

    // Array initialization

    for (int i = 0; i < *numberOfValue; i++){
        strcpy(valuesOfInsert[i], "");
    }

    // Array association
    
    char value[2];

    for (int i = 1, j = 0; i < valueToParseLength; i++) {
        
        value[0] = valueToParse[i];
        value[1] = '\0';
        
        if ((valueToParse[i] == 34 || valueToParse[i] == 39) && inQuote == 0) {
            inQuote = 1;
        }
        else if ((valueToParse[i] == 34 || valueToParse[i] == 39) && valueToParse[i+1] == ',' && inQuote == 1) {
            inQuote = 0;
        }
        
        if(valueToParse[i] == ',' && inQuote == 0){
            j++;
        }
        else if(valueToParse[i] == ')' && valueToParse[i+1] == ';'){
            j++;
            break;
        }
        else if((valueToParse[i] != ' ' && inQuote == 0) || inQuote == 1){
            strcat(valuesOfInsert[j], value);
        }
        
    }
    
    replaceQuotes(*numberOfValue, valuesOfInsert);
    
    return valuesOfInsert;
}

// Replace string with quotes by string without quotes

void replaceQuotes(int numberOfValue, char** array){
    for (int i = 0; i < numberOfValue; i++) {
        unsigned long int arrayLength = strlen(array[i]);
        if((array[i][0] == 34 && array[i][arrayLength-1] == 34) || (array[i][0] == 39 && array[i][arrayLength-1] == 39)){
            for (int j = 0; j < arrayLength; j++) {
                array[i][j] = array[i][j+1];
            }
            array[i][arrayLength-2] = '\0';
        }
    }
}

// Parsing of the Columns in the CREATE TABLE command

Column * getValuesOfTableCreation(char *valueToParse, int *numberOfValue){
    
    unsigned long valueToParseLength = strlen(valueToParse);
    
    for (int i = 1; i < valueToParseLength; i++) {
        
        if(valueToParse[i] == ','){
            *numberOfValue += 1;
        }
        else if(valueToParse[i] == ')' && valueToParse[i+1] == ';'){
            *numberOfValue += 1;
            break;
        }
        
    }
    
    // Create an array of the Column structure with numberOfValue

    
    Column *columnOfTableCreation = (Column*)malloc(sizeof(Column)*(*numberOfValue));
    
    // Array initialization

    for (int i = 0; i < *numberOfValue; i++){
        columnOfTableCreation[i].name = malloc(sizeof(char*));
        columnOfTableCreation[i].type = malloc(sizeof(char*));
        columnOfTableCreation[i].defaultValue =  malloc(sizeof(char*));
        columnOfTableCreation[i].foreignKey =  malloc(sizeof(char*));
        columnOfTableCreation[i].autoIncrement = 0;
        strcpy(columnOfTableCreation[i].name, "");
        strcpy(columnOfTableCreation[i].type, "");
        strcpy(columnOfTableCreation[i].defaultValue, "");
        strcpy(columnOfTableCreation[i].foreignKey, "");
        columnOfTableCreation[i].notNull = 0;
        columnOfTableCreation[i].length = 0;
        columnOfTableCreation[i].primaryKey = 0;
    }
    
    // Structure Column association
    
    char character[2];
    int length = 0;
    
    for (int i = 1, currentColumnAttribute = 0, currentColumn = 0; i < valueToParseLength; i++) {
        
        character[0] = valueToParse[i];
        character[1] = '\0';
        
        if (valueToParse[i] != '(' && valueToParse[i] != ')' && valueToParse[i] != ' ' && valueToParse[i] != ',' && valueToParse[i] != ';' ) {
            switch (currentColumnAttribute) {
                case 0:
                    strcat(columnOfTableCreation[currentColumn].name, character);
                    break;
                case 1:
                    strcat(columnOfTableCreation[currentColumn].type, character);
                    break;
                case 2:
                    if (valueToParse[i+1] == ')') {
                        length = length*10+atoi(character);
                        columnOfTableCreation[currentColumn].length = length;
                        length = 0;
                    }
                    else{
                        length = length*10+atoi(character);
                    }
                    break;
                default:
                    break;
            }
        }
        else if((valueToParse[i] == ' ' && valueToParse[i-1] != ',') || valueToParse[i] == '('){
            currentColumnAttribute++;
        }
        else if (valueToParse[i] == ','){
            currentColumnAttribute = 0;
            currentColumn++;
        }
    }
    
    return columnOfTableCreation;
}
