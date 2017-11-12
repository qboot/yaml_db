//
//  Filename: manage_file.h
//  Made by: Quentin Brunet
//  Date: 22/10/2017
//  Description: Header of manage_file.c
//

#ifndef MANAGE_FILE
#define MANAGE_FILE

char* createPath(const char *name, const int isFile);
char* createFilePath(const char *filename);
char* createDirPath(const char *dirname);
void generateFile(const char *filename, const char *name);
void createFile(const char *filename);
void createFileInDir(const char *filename, const char *dirname);
void removeFile(const char *filename);
int isFile(const char *filename);
void createDir(const char *dirname);
void removeDir(const char *dirname);
int isValidName(const char *name);
int hasProperty(const char *filename, const char *property);
void removeLine(const char *filename, int lineNumber);

#endif
