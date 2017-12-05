//
//  Filename: manage_table.h
//  Made by: Quentin Brunet
//  Date: 12/11/2017
//  Description: Header of manage_table.c
//

#ifndef MANAGE_TABLE
#define MANAGE_TABLE

#include "config.h"

void createTable(const Database database, const Table table);
void dropTable(const Database database, const Table table);
int hasTable(const Database database, const Table table);
void createTableStructure(const Database database, const Table table);
int hasColumn(const Database database, const Table table, const Column column);
StringArray getColumnNames(const Database database, const Table table);
void removeDataTilde(const Database database, const Table table);

#endif
