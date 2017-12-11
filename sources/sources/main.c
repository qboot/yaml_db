//
//  Filename: main.c
//  Made by: Quentin Brunet
//  Date: 22/10/2017
//  Description: Main File
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../headers/main.h"
#include "../headers/manage_file.h"
#include "../headers/manage_array.h"
#include "../headers/manage_entry.h"

static int isExitCommand(char *command);

int main(int argc, const char *argv[])
{
    
//    StringArray *entryArray = createStringArray();
//
//    char string[] = "super string";
//    appendToStringArray(entryArray, string);
//
//    char string2[] = "waouh 2";
//    appendToStringArray(entryArray, string2);
//
//    char string3[] = "fail me 2";
//    appendToStringArray(entryArray, string3);
//
//    for (int i = 0; i < entryArray->size; ++i) {
//        printf("%s\n", entryArray->data[i]);
//        printf("%d\n", entryArray->size);
//        printf("%d\n", entryArray->capacity);
//    }
//
//    freeStringArray(entryArray);
//
//    exit(EXIT_FAILURE);
    
    
    printf("Welcome to yaml_db project! \n");
    printf("\n");
    printf("----------------------------\n");
    printf("   -                    -   \n");
    printf("  -                      -  \n");
    printf("  -       v.1.0.0        -  \n");
    printf("  -                      -  \n");
    printf("   -                    -   \n");
    printf("----------------------------\n");
    
    entryLoop();
    
    printf("\nClosing yaml_db project... Bye!\n");
    
    return 0;
}

//
// Entry point : will loop on user input and provide entry to parser
//
void entryLoop()
{
    Manager manager = {DB_FILENAME, createFile(DB_FILENAME)};
    char *currentDatabase = malloc(sizeof(char)*STRING_SIZE);
    strcpy(currentDatabase, "");
    
    char entry[STRING_SIZE] = "";
    
    do {
        printf("\nyaml_db > ");
        fgets(entry, sizeof(entry), stdin);
        
        if (isExitCommand(entry)) {
            break;
        }
        
        parseEntry(manager, currentDatabase, entry);
    } while (1);
    
    free(currentDatabase);
    free(manager.path);
}

//
// Check if user has typed `exit` command
//
int isExitCommand(char *command)
{
    char cleanCommand[STRING_SIZE] = "";
    
    for (int i = 0; i < strlen(command)-1; ++i) {
        if (command[i] == ';') {
            continue;
        }
        
        char charToString[] = {tolower(command[i]), '\0'};
        strcat(cleanCommand, charToString);
    }
    
    trimTrailingSpaces(cleanCommand);
    
    if (strcmp(cleanCommand, "exit") == 0 || strcmp(cleanCommand, "quit") == 0) {
        return 1;
    }
    
    return 0;
}
