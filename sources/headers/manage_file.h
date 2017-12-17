//
//  Filename: manage_file.h
//  Made by: Quentin Brunet
//  Date: 22/10/2017
//  Description: Header of manage_file.c
//

#ifndef MANAGE_FILE
#define MANAGE_FILE

#include "config.h"

char* createFilePath(const char *filename);
char* createDirPath(const char *dirname);
char* createFileInDirPath(const char *filename, const char *dirname);
char* createFile(const char *filename);
char* createFileInDir(const char *filename, const char *dirname);
void removeFile(const char *filename);
void removeFileInDir(const char *filename, const char *dirname);
int isFile(const char *filename);
void createRootDir();
void createDir(const char *dirname);
void removeDir(const char *dirname);
int isValidName(const char *name);
int hasProperty(const char *filename, const char *property);
void removeLine(const char *filename, int lineNumber);
void replaceLine(const char *filename, int lineNumber, char *newLine);

#endif
