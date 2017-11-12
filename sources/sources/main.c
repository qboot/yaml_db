//
//  Filename: main.c
//  Made by: Quentin Brunet
//  Date: 22/10/2017
//  Description: Main File
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/config.h"
#include "../headers/manage_file.h"
#include "../headers/manage_database.h"
#include "../headers/manage_table.h"

int main(int argc, const char *argv[])
{
    char *manager = DB_FILENAME;
    char *managerPath = createFilePath(manager);
    createFile(manager);
    
    char *base1 = "base1";
    char *base2 = "base2";
    char *table1 = "table1";
    
    createDatabase(managerPath, base1);
    createDatabase(managerPath, base2);
    createTable(base2, table1);
    
    dropDatabase(managerPath, base1);
    
    free(managerPath);
    return 0;
}
