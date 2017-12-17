//
//  Filename: manage_file.c
//  Made by: Quentin Brunet
//  Date: 22/10/2017
//  Description: Functions to handle files
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "../headers/manage_file.h"

static char* createPath(const char *name, const int isFile);
static char* generateFile(const char *filepath, const char *filename);

//
// Create a path `databases/name` for a given `name`
// Return a fullpath (root + name)
//
static char* createPath(const char *name, const int isFile)
{
    char path[STRING_SIZE] = DB_PATH;
    strcat(path, name);
    
    if (isFile == 1) {
        strcat(path, ".yml");
    }
    
    char *fullPath = malloc(sizeof(char)*STRING_SIZE);
    strcpy(fullPath, path);
    
    return fullPath;
}

//
// Create a path `databases/filename.yml` for a given `filename`
// Return a fullpath (root + filename + extension)
//
char* createFilePath(const char *filename)
{
    return createPath(filename, 1);
}

//
// Create a path `databases/dirname` for a given `dirname`
// Return a fullpath (root + dirname)
//
char* createDirPath(const char *dirname)
{
    return createPath(dirname, 0);
}

//
// Create a path `databases/dirname/filename.yml` for a given `dirname` and `filename`
// Return a fullpath (root + dirname + filename + extension)
//
char* createFileInDirPath(const char *filename, const char *dirname)
{
    char path[STRING_SIZE] = "";
    strcat(path, dirname);
    strcat(path, "/");
    strcat(path, filename);
    
    return createPath(path, 1);
}

//
// Generate a file named `filename` and write its first line
// Return a fullpath (root + filename + extension)
//
static char* generateFile(const char *filepath, const char *filename)
{
    char *path = (char*) filepath;
    
    // file already exists, stop here
    if (isFile(path)) {
        return path;
    }
    
    FILE *file = fopen(path, "w");
    
    if (file == NULL) {
        free(path);
        printf("fopen() failed in file %s at line #%d\n", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
    
    fprintf(file, "%s:\n", filename);
    fclose(file);
    return path;
}

//
// Create a file named `filename`
// Return a fullpath (root + filename + extension)
//
char* createFile(const char *filename)
{    
    char *filepath = createFilePath(filename);
    return generateFile(filepath, filename);
}

//
// Create a file named `filename` in directory `dirname`
// Return a fullpath (root + dirname + filename + extension)
//
char* createFileInDir(const char *filename, const char *dirname)
{
    char *filepath = createFileInDirPath(filename, dirname);
    return generateFile(filepath, filename);
}

//
// Remove a file named `filename`
//
void removeFile(const char *filename)
{
    char *path = createFilePath(filename);
    
    // file doesn't exist, stop here
    if (isFile(path) == 0) {
        free(path);
        return;
    }
    
    remove(path);
    free(path);
}

//
// Remove a file named `filename` in directory `dirname`
//
void removeFileInDir(const char *filename, const char *dirname)
{
    char *path = createFileInDirPath(filename, dirname);
    
    // file doesn't exist, stop here
    if (isFile(path) == 0) {
        free(path);
        return;
    }
    
    remove(path);
    free(path);
}

//
// Check if a file exists
// Return 1 if exists, else 0
//
int isFile(const char *filename)
{
    printf("%s", filename);
    
    FILE *file = fopen(filename, "r");
    
    if (file != NULL) {
        fclose(file);
        return 1;
    }
    
    return 0;
}

//
// Create a directory named `dirname`
//
void createDir(const char *dirname)
{
    char *path = createDirPath(dirname);
    struct stat st = {0};
    
    if (stat(path, &st) == -1) {
        mkdir(path, 0777);
    }
    
    free(path);
}

//
// Remove a directory named `dirname` and all its content
//
void removeDir(const char *dirname)
{
    char *path = createDirPath(dirname);
    struct stat st = {0};
    
    if (stat(path, &st) == -1) {
        free(path);
        return;
    }
    
    DIR *dir = opendir(path);
    struct dirent *file;
    
    if (dir == NULL) {
        free(path);
        printf("opendir() failed in file %s at line #%d\n", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
    
    while ((file = readdir(dir)) != NULL) {
        
        if (!strcmp(file->d_name, ".") || !strcmp(file->d_name, "..")) {
            continue;
        }
        
        char filepath[STRING_SIZE] = "";
        strcat(filepath, path);
        strcat(filepath, "/");
        strcat(filepath, file->d_name);
        
        remove(filepath);
    }
    
    closedir(dir);
    remove(path);
    free(path);
}

//
// Check if given name is a valid one (0-9 a-z A-Z and _ allowed)
// Return 1 if yes, else 0
//
int isValidName(const char *name)
{
    int i = 0;
    
    for (i = 0; i < strlen(name); ++i) {
        if (name[i] != '_' &&
            !(name[i] >= 48 && name[i] <= 57) &&
            !(name[i] >= 65 && name[i] <= 90) &&
            !(name[i] >= 97 && name[i] <= 122)) {
            return 0;
        }
    }
    
    return 1;
}

//
// Check if a file which contains only a YAML sequence has a specific property
// Return property line number if yes, else 0
//
int hasProperty(const char *filename, const char *property)
{
    FILE *file = fopen(filename, "r");
    
    if (file == NULL) {
        printf("fopen() failed in file %s at line #%d\n", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
    
    int pos = 0;
    char line[STRING_SIZE] = "";
    
    while (fgets(line, STRING_SIZE, file) != NULL) {
        
        // get the 6 first characters + null terminator `\0`
        char beginning[7] = "";
        strncpy(beginning, line, 6);
        
        if (strcmp(beginning, TAB "- ") != 0) {
            ++pos;
            continue;
        }
        
        char stripedLine[STRING_SIZE] = "";
        int i = 0;
        int j = 0;
        
        for (i = 0; i < strlen(line); ++i) {
            if (line[i] == '\n' || line[i] == '-' || line[i] == ' ') {
                continue;
            }
            
            if (line[i] == '#') {
                break;
            }
            
            stripedLine[j] = line[i];
            ++j;
        }
        
        if (strcmp(stripedLine, property) == 0) {
            return pos;
        }
        
        ++pos;
    }
    
    fclose(file);
    return 0;
}

//
// Remove a specific line in a file
//
void removeLine(const char *filename, int lineNumber)
{
    char newFilename[STRING_SIZE] = "";
    strcpy(newFilename, filename);
    newFilename[strlen(newFilename)-4] = '\0';
    strcat(newFilename, "_new.yml");
    
    FILE *oldFile = fopen(filename, "r");
    FILE *newFile = fopen(newFilename, "w");
    
    if (oldFile == NULL || newFile == NULL) {
        printf("fopen() failed in file %s at line #%d\n", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
    
    int pos = 0;
    char line[STRING_SIZE] = "";
    
    while (fgets(line, STRING_SIZE, oldFile) != NULL) {
        if (pos != lineNumber) {
            fputs(line, newFile);
        }
        
        ++pos;
    }
    
    fclose(oldFile);
    fclose(newFile);
    remove(filename);
    rename(newFilename, filename);
}

//
// Replace a specific line in a file
//
void replaceLine(const char *filename, int lineNumber, char *newLine)
{
    char newFilename[STRING_SIZE] = "";
    strcpy(newFilename, filename);
    newFilename[strlen(newFilename)-4] = '\0';
    strcat(newFilename, "_new.yml");
    
    FILE *oldFile = fopen(filename, "r");
    FILE *newFile = fopen(newFilename, "w");
    
    if (oldFile == NULL || newFile == NULL) {
        printf("fopen() failed in file %s at line #%d\n", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
    
    int pos = 0;
    char line[STRING_SIZE] = "";
    
    while (fgets(line, STRING_SIZE, oldFile) != NULL) {
        if (pos != lineNumber) {
            fputs(line, newFile);
        } else {
            fputs(newLine, newFile);
        }
        
        ++pos;
    }
    
    fclose(oldFile);
    fclose(newFile);
    remove(filename);
    rename(newFilename, filename);
}
