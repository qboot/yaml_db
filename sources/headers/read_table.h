//
//  read_table.h
//  yaml_db
//
//  Created by pierre piron on 25/11/2017.
//  Copyright © 2017 Groupe 16. All rights reserved.
//

#ifndef READ_TABLE
#define READ_TABLE

#include "config.h"

void parseData(StringArray data, Table *currentTable);
void printAllResult(Table *currentTable);
int searchColumnPlace(Table *currentTable, char *columnName);
Table searchSpecificData(Table *currentTable, int columnPlace, char *dataSearched);
Table searchData(Table *currentTable, int columnPlace, char *dataSearched);
void searchSpecificColumn(Table *currentTable, char *columnName);
void clearUnusedColumn(Table *currentTable, int columnPlace);
#endif

