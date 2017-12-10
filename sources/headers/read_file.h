//
//  read_file.h
//  yaml_db
//
//  Created by pierre piron on 10/12/2017.
//  Copyright © 2017 Groupe 16. All rights reserved.
//

#ifndef read_file_h
#define read_file_h

#include <stdio.h>
#include "config.h"

void findAllRecords(char* db_name, char* table_name);
void findSpecificRecords(char* db_name, char* table_name, char* column_name, char* record_value, char* conditionType);
void findColumnsName(char* db_name, char* table_name);
StringArray readColumnsName(FILE* f);
StringArray readData(FILE* f);
int filesFound(char *db_name, char *table_name, char *table_file);

#endif
