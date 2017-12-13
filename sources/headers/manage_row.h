//
//  Filename: manage_row.h
//  Made by: Quentin Brunet
//  Date: 04/12/2017
//  Description: Header of manage_row.c
//

#ifndef MANAGE_ROW
#define MANAGE_ROW

#include "config.h"

StringArray parseRow(char *row);
void addRows(const Database database, const Table table);
void updateRows(const Database database, const Table table, const int nbValues, const char **values, const int nbConditions, const Condition *conditions);
void deleteRows(const Database database, const Table table, const int nbConditions, const Condition *conditions);

#endif
