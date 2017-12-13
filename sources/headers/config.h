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
#define ARRAY_CAPACITY 20
#define DB_FILENAME "databases"
#define DB_PATH DB_FILENAME "/"

typedef struct {
    char *data;
} Cell;

typedef struct {
    int nbCells;
    Cell *cells;
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
    int nbColumns;
    int nbRows;
    Column *columns;
    Row *rows;
} Table;

typedef struct {
    char *name;
    int nbTables;
    Table *tables;
} Database;

typedef struct {
    char *name;
    char *path;
    int nbDatabases;
    Database *databases;
} Manager;

typedef struct {
    char *column;
    char *value;
    char *type;
} Condition;

typedef struct {
    char **data;
    int size;
    int capacity;
    
} StringArray;

typedef struct {
    int *data;
    int size;
    int capacity;
    
} IntArray;

#endif
