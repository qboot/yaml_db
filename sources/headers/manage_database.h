//
//  Filename: manage_database.h
//  Made by: Quentin Brunet
//  Date: 22/10/2017
//  Description: Header of manage_database.c
//

#ifndef MANAGE_DB
#define MANAGE_DB

#include "config.h"

void createDatabase(const char *manager, const char *name);
void dropDatabase(const char *manager, const char *name);
int hasDatabase(const char *manager, const char *name);
int isDatabase(const char *name);

#endif
