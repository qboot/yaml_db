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
#include "../headers/manage_array.h"

#include <ctype.h>

int main(int argc, const char *argv[])
{
    char *manager = DB_FILENAME;
    char *managerPath = createFile(manager);
    
    char *base1 = "base1";
    char *base2 = "base2";
    
    char *table1 = "table1";
    Column columns1[] = {
        {
            "id",
            "int"
        },
        {
            "pseudo",
            "char"
        }
    };
    int nbColumns1 = 2;
    
    char *table2 = "table2";
    Column columns2[] = {
        {
            "id",
            "int"
        },
        {
            "project",
            "char"
        },
        {
            "event",
            "char"
        },
        {
            "people",
            "char"
        }
    };
    int nbColumns2 = 6;
    
    // create and drop databases + tables
    createDatabase(managerPath, base1);
    createDatabase(managerPath, base2);
    createTable(base2, table1, nbColumns1, columns1);
    createTable(base2, table2, nbColumns2, columns2);

    dropTable(base2, table1);
    dropDatabase(managerPath, base1);
    
    // check if column exists
    char columnName[] = "project";
    printf("%d\n", hasColumn("databases/base2/table2.yml", columnName));
    
    // try to add two rows
    const char **columns = malloc(sizeof(int)*4*sizeof(char)*STRING_SIZE);
    columns[0] = "id";
    columns[1] = "project";
    columns[2] = "event";
    columns[3] = "people";
    
    const char ***rows = malloc(sizeof(int)*2);
    rows[0] = malloc(sizeof(int)*4*sizeof(char)*STRING_SIZE);
    rows[1] = malloc(sizeof(int)*4*sizeof(char)*STRING_SIZE);
    rows[0][0] = "1";
    rows[0][1] = "2";
    rows[0][2] = "3";
    rows[0][3] = "4";
    rows[1][0] = "a";
    rows[1][1] = "b";
    rows[1][2] = "c";
    rows[1][3] = "d";
    
    addRows("databases/base2.yml", "databases/base2/table2.yml", 4, columns, 2, rows);
    
    free(managerPath);
    return 0;
}
