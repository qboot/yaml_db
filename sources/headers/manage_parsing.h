//
//  special_parsing.h
//  yaml_db
//
//  Created by Léo LEGRON on 03/12/2017.
//  Copyright © 2017 Groupe 16. All rights reserved.
//

#ifndef MANAGE_PARSING
#define MANAGE_PARSING

#include "config.h"

Column* getColumns(char *string, int *nbColumns, int onlyName);
Row getRow(char *string);
StringArray splitWithDelimiter(char *string, char delimiter);
char* secureInput(char *string);

#endif
