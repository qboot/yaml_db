//
//  special_parsing.h
//  yaml_db
//
//  Created by Léo LEGRON on 03/12/2017.
//  Copyright © 2017 Groupe 16. All rights reserved.
//

#ifndef SPECIAL_PARSING
#define SPECIAL_PARSING

#include "config.h"

char ** getValuesOfInsert(char *valueToParse, int *numberOfValue);
Column * getValuesOfTableCreation(char *valueToParse, int *numberOfValue);
void replaceQuotes(int numberOfValue, char** valuesOfInsert);

#endif
