//
//  Filename: config.h
//  Made by: Quentin Brunet
//  Date: 12/11/2017
//  Description: Configuration header
//

#ifndef CONFIG
#define CONFIG

#define TAB "    "
#define STRING_SIZE 255
#define ARRAY_SIZE 20
#define DB_FILENAME "databases"
#define DB_PATH DB_FILENAME "/"

typedef struct {
    char **data;
} Row;

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

typedef struct {
    char *name;
    char *path;
    int nbColumns;
    int nbRows;
    Column *columns;
    Row *rows;
} Table;

typedef struct {
    char *name;
    char *path;
    int nbTables;
    Table *tables;
} Database;

#endif
