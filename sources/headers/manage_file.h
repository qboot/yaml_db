//
//  Filename: manage_file.h
//  Made by: Quentin Brunet
//  Date: 22/10/2017
//  Description: Header of manage_file.c
//

#ifndef MANAGE_FILE
#define MANAGE_FILE

void createFile(const char *filename);
char* createPath(const char *filename);
int isFile(const char *filename);
int isValidName(const char *name);
int hasProperty(const char *filename, const char *property);
void removeLine(const char *filename, int lineNumber);
void removeFile(const char *filename);

#endif
