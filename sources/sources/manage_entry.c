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

void parseEntry(char* entry){
    
    printf("entry :\n");
    printf("%s", entry);
    
    // Get the number of words from entry to optimize parsedEntry array
    
    int numberOfWords = 0;
    unsigned long entryLength = strlen(entry);
    
    for (int i = 0; i < entryLength; i++){
        
        const char value = entry[i];
        
        if(value == ' ' || value == '\n'){
            numberOfWords++;
        }
        else{
             continue;
        }
    }
    
    printf("numberOfWords :\n");
    printf("%d\n", numberOfWords);
    
    // Create a array with words from entry
    
    char parsedEntry[numberOfWords][30];
    
    // Array initialization
    
    for (int i = 0; i < numberOfWords; i++){
        strcpy(parsedEntry[i], "");
    }
    
    // Array association

    for (int i = 0, j = 0; i < entryLength; i++) {
    
        const char value = entry[i];
        
        if(value == ' '){
            j++;
        }
        else if(value == '\n'){
            break;
        }
        else{
            strcat(parsedEntry[j], &value);
        }
    };
    
    printf("%s\n", parsedEntry[1]);

}


