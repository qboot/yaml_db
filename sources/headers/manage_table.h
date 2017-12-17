//
//  Filename: manage_table.h
//  Made by: Quentin Brunet
//  Date: 12/11/2017
//  Description: Header of manage_table.c
//

#ifndef MANAGE_TABLE
#define MANAGE_TABLE

#include "config.h"

int createTable(const Database database, const Table table);
int dropTable(const Database database, const Table table);
int hasTable(const Database database, const Table table);
int createTableStructure(const Database database, const Table table);
int hasColumn(const Database database, const Table table, const Column column);
Column* getAllColumns(const Database database, const Table table, int *nbColumns);
void removeDataTilde(const Database database, const Table table);

#endif
