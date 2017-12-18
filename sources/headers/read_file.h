//
//  read_file.h
//  yaml_db
//
//  Created by pierre piron on 10/12/2017.
//  Copyright © 2017 Groupe 16. All rights reserved.
//

#ifndef READ_FILE
#define READ_FILE

#include "config.h"

void showDatabases(void);
void showTables(Database database);
Table findAllRecords(char *db_name, char *table_name);
StringArray readColumnsName(FILE *f);
StringArray readData(FILE *f);
int filesFound(char *db_name, char *table_name, char *table_file);

#endif
