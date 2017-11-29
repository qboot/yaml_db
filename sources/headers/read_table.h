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

void findAllRecords(char *db_name, char *table_name);
void findSpecificRecords(char *db_name, char *table_name, char *record_value);
void printResult(char **column_name, int *columnNameSize, char **result, int *dataSize);
char** readColumnName(FILE* f, int *columnNameSize);
int filesFound(char *db_name, char *table_name, char *table_file);
char** readData(FILE* f, int *dataSize);
#endif

