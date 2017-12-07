//
//  conf.h
//  yaml_db
//
//  Created by Léo LEGRON on 04/12/2017.
//  Copyright © 2017 Groupe 16. All rights reserved.
//

#ifndef conf_h
#define conf_h

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
    Column *columns;
    int nbColumns;
    int nbRows;
    Row *rows;
} Table;

typedef struct {
    char *name;
    char *path;
    Table *tables;
} Database;

#endif /* conf_h */
