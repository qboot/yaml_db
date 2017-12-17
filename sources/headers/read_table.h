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

void parseData(StringArray data, Table *currentTable);
void printAllResult(Table *currentTable);
int searchColumnPlace(Table *currentTable, char *columnName);
Table searchSpecificData(Table *currentTable, int columnPlace, char *dataSearched);
Table searchData(Table *currentTable, int columnPlace, char *dataSearched);
void searchSpecifiColumn(Table *currentTable, char *columnName);
void clearUnusedColumn(Table *currentTable, int columnPlace);
#endif

