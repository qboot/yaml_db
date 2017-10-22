//
//  Filename: main.c
//  Made by: Quentin Brunet
//  Date: 22/10/2017
//  Description: Main File
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/manage_file.h"
#include "../headers/manage_database.h"

#define DB_FILENAME "databases"

int main(int argc, const char *argv[])
{
    char *manager = DB_FILENAME;
    char *managerPath = createPath(manager);
    createFile(manager);
    
    char *name1 = "base1";
    char *name2 = "base2";
    createDatabase(managerPath, name1);
    createDatabase(managerPath, name2);
    
    dropDatabase(managerPath, name1);
    
    free(managerPath);
    return 0;
}
