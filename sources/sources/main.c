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
#include "../headers/manage_row.h"

#include <ctype.h>

int main(int argc, const char *argv[])
{    
    Manager manager = {DB_FILENAME, createFile(DB_FILENAME)};
    
    Database base1 = {"base1"};
    Database base2 = {"base2"};
    
    Column columns1[] = {{"id", "int"}, {"pseudo", "char"}};
    Table table1 = {"table1", 2, 0, columns1};
    
    Column columns2[] = {{"id", "int"}, {"project", "char"}, {"event", "char"}, {"people", "char"}};
    Table table2 = {"table2", 4, 0, columns2};
    
    // create and drop databases + tables
    createDatabase(manager, base1);
    createDatabase(manager, base2);
    createTable(base2, table1);
    createTable(base2, table2);

    dropTable(base2, table1);
    dropDatabase(manager, base1);
    
    // check if column exists
    Column randomColumn = {"project"};
    printf("%d // column exists!\n", hasColumn(base2, table2, randomColumn));

    // add three rows
    Cell cells1[] = {"1", "2", "3", "4"};
    Cell cells2[] = {"a", "b", "c", "d"};
    Cell cells3[] = {"z", "y", "x", "w"};
    Row newRows[] = {{4, cells1}, {4, cells2}, {4, cells3}};
    Column newColumns[] = {{"id"}, {"project"}, {"event"}, {"people"}};
    Table newTable = {table2.name, 4, 3, newColumns, newRows};

    addRows(base2, newTable);

    // update a row with `where` clause
    const char *updateValues[] = {"newId", "newProject"};
    Column updateColumns[] = {{"id"}, {"project"}};
    Condition updateConditions[] = {{"project", "b", "="}};
    Table updateTable = {table2.name, 2, 0, updateColumns};

    updateRows(base2, updateTable, 2, updateValues, 1, updateConditions);

    // delete a row with `where` clause
    Condition deleteConditions[] = {{"project", "y", "="}};
    deleteRows(base2, table2, 1, deleteConditions);
    
    free(manager.path);
    return 0;
}
