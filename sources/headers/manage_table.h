//
//  Filename: manage_table.h
//  Made by: Quentin Brunet
//  Date: 12/11/2017
//  Description: Header of manage_table.c
//

#ifndef MANAGE_TABLE
#define MANAGE_TABLE

typedef struct {
    char name[STRING_SIZE];
    char type[STRING_SIZE];
    int notNull;
    int length;
    char defaultValue[STRING_SIZE];
    int primaryKey;
    char foreignKey[STRING_SIZE];
    int autoIncrement;
} Column;

void createTable(const char *database, const char *name);
int hasTable(const char *database, const char *name);

#endif
