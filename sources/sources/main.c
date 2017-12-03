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
    Column columns1[] = {{"id", "int"}, {"pseudo", "char"}};
    int nbColumns1 = 2;
    
    char *table2 = "table2";
    Column columns2[] = {{"id", "int"}, {"project", "char"}, {"event", "char"}, {"people", "char"}};
    int nbColumns2 = 4;
    
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
    
    // add three rows
    const char *columns[] = {"id", "project", "event", "people"};
    Cell cells1[] = {"1", "2", "3", "4"};
    Cell cells2[] = {"a", "b", "c", "d"};
    Cell cells3[] = {"z", "y", "x", "w"};
    const Row rows[] = {{4, cells1}, {4, cells2}, {4, cells3}};
    int rowsSize = 3;
    
    addRows("databases/base2.yml", "databases/base2/table2.yml", 4, columns, rowsSize, rows);
    
    // update a row
    const char *uColumns[] = {"id", "project"};
    const char *uValues[] = {"newId", "newProject"};
    Condition uConditions[] = {{"project", "b", "="}};
    
    updateRows("databases/base2.yml", "databases/base2/table2.yml", 2, uColumns, 2, uValues, 1, uConditions);
    
    // delete a row
    Condition rConditions[] = {{"project", "y", "="}};
    deleteRows("databases/base2.yml", "databases/base2/table2.yml", 1, rConditions);
    
    free(managerPath);
    return 0;
}
