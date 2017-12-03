//
//  Filename: manage_table.h
//  Made by: Quentin Brunet
//  Date: 12/11/2017
//  Description: Header of manage_table.c
//

#ifndef MANAGE_TABLE
#define MANAGE_TABLE

#include "config.h"

void createTable(const char *database, const char *name, int nbColumns, const Column *columns);
void dropTable(const char *database, const char *name);
int hasTable(const char *database, const char *name);
void createTableStructure(const char *table, int nbColumns, const Column *columns);
int hasColumn(const char *table, const char *name);
char** getColumns(const char *table, int *nbColumns);
void addRows(const char *database, const char *table, const int nbColumns, const char **columns, const int nbRows, const Row *rows);
void removeDataTilde(const char *table);
char **parseRow(char *row, int *size);
void updateRows(const char *database, const char *table, const int nbColumns, const char **columns, const int nbValues, const char **values, const int nbConditions, const Condition *conditions);

#endif
