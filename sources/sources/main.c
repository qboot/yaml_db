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
#include "../headers/manage_entry.h"

int main(int argc, const char * argv[]) {
    
    printf("Welcome to yaml_db project !\n");
    printf("----------------------------\n");
    printf("   -                    -   \n");
    printf("  -                      -  \n");
    printf("  -       v.1.0.0        -  \n");
    printf("  -                      -  \n");
    printf("   -                    -   \n");
    printf("----------------------------\n");
    
    entryLoop();
    
    printf("\nClosing yaml_db project... Bye !\n");
    
    return 0;
}

void entryLoop() {
    
    Manager manager = {DB_FILENAME, createFile(DB_FILENAME)};
    Database* currentDatabase = malloc(sizeof(Database));
    currentDatabase->name = "";
    
    char entry[200] = "";
    char *exit = "exit\n";
    
    do {
        printf("yaml_db > ");
        fgets(entry, sizeof(entry), stdin);
        parseEntry(manager, currentDatabase, entry);
    } while (strcmp(entry, exit));
    
    free(currentDatabase);
    free(manager.path);
}
