//
//  Filename: manage_table.h
//  Made by: Quentin Brunet
//  Date: 12/11/2017
//  Description: Header of manage_table.c
//

#ifndef MANAGE_TABLE
#define MANAGE_TABLE

typedef struct {
    char *name;
    char *type;
    int notNull;
    int length;
    char *defaultValue;
    int primaryKey;
    char *foreignKey;
    int autoIncrement;
} Column;

void createTable(const char *database, const char *name);
int hasTable(const char *database, const char *name);
void createColumn(const char *database, const char *table, const Column col);
int hasColumn(const char *table, const char *name);

#endif
