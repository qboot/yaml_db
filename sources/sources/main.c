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

#include "../headers/config.h"
#include "../headers/main.h"
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
    
    printf("Byebye !\n");
    
    return 0;
}

void entryLoop(){
    
    char entry[100] = "";
    char *exit = "exit";
    
    do{
        printf("Myaml > ");
        fgets(entry, sizeof(entry), stdin);
        parseEntry(entry);
        
    }while(strncmp(entry, exit, 5));
    
}
