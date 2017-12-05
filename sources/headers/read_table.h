//
//  read_table.h
//  yaml_db
//
//  Created by pierre piron on 25/11/2017.
//  Copyright © 2017 Groupe 16. All rights reserved.
//

#ifndef read_table_h
#define read_table_h

#include <stdio.h>
#include "config.h"

void findAllRecords(char* db_name, char* table_name);
void findSpecificRecords(char *db_name, char *table_name, char* column_name, char *record_value);
char** readColumnName(FILE* f, int *columnNameSize);
int filesFound(char *db_name, char *table_name, char *table_file);
char** readData(FILE* f, int *dataSize);
void parseData(char **data, int numberOfData, Table *currentTable);
void printResult(char **column_name, int *columnNameSize, Table *currentTable);
int searchColumnPlace(Table *currentTable, char *columnName);
int searchSpecificData(Table *currentTable, int columnPlace, char *dataSearched);
#endif

