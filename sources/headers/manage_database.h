//
//  Filename: manage_database.h
//  Made by: Quentin Brunet
//  Date: 22/10/2017
//  Description: Header of manage_database.c
//

#ifndef MANAGE_DB
#define MANAGE_DB

#include "config.h"

int createDatabase(const Manager manager, const Database database);
int dropDatabase(const Manager manager, const Database database);
int hasDatabase(const Manager manager, const Database database);
int isDatabase(const Database database);

#endif
