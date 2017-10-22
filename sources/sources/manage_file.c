//
//  Filename: manage_file.c
//  Made by: Quentin Brunet
//  Date: 22/10/2017
//  Description: Functions to handle files
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/manage_file.h"

#define STRING_SIZE 255
#define DB_PATH "databases/"

//
// Create a file named `filename` and write its first line
//
void createFile(const char *filename)
{
    char *path = createPath(filename);
    
    // file already exists, stop here
    if (fileExists(path)) {
        free(path);
        return;
    }
    
    // filename is not valid, stop here
    if (isValidFilename(filename) == 0) {
        free(path);
        printf("Name should only contain 0-9 a-z A-Z and _ characters.\n");
        exit(EXIT_FAILURE);
    }
    
    FILE *file = fopen(path, "w");
    
    if (file == NULL) {
        printf("fopen() failed in file %s at line #%d\n", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
    
    fprintf(file, "%s:\n", filename);
    fclose(file);
    free(path);
}

//
// Create a path `databases/filename.yml` for a given `filename`
// Return a fullpath (directory + filename + extension)
//
char* createPath(const char *filename)
{
    char path[STRING_SIZE] = DB_PATH;
    strcat(path, filename);
    strcat(path, ".yml");
    
    char *fullPath = malloc(sizeof(char)*STRING_SIZE);
    strcpy(fullPath, path);
    
    return fullPath;
}

//
// Check if a file exists
// Return 1 if exists, else 0
//
int fileExists(const char *filename)
{
    FILE *file = fopen(filename, "r");
    
    if (file != NULL) {
        fclose(file);
        return 1;
    }
    
    return 0;
}

//
// Check if given filename is a valid one (0-9 a-z A-Z and _ allowed)
// Return 1 if yes, else 0
//
int isValidFilename(const char *filename)
{
    int i = 0;
    
    for (i = 0; i < strlen(filename); ++i) {
        if (filename[i] != '_' &&
            !(filename[i] >= 48 && filename[i] <= 57) &&
            !(filename[i] >= 65 && filename[i] <= 90) &&
            !(filename[i] >= 97 && filename[i] <= 122)) {
            return 0;
        }
    }
    
    return 1;
}

//
// Find if query name exists in file
// Return line number if found, else 0
//
int findMatchingLine(const char *filename, const char *query)
{
    FILE *file = fopen(filename, "r");
    
    if (file == NULL) {
        printf("fopen() failed in file %s at line #%d\n", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
    
    int pos = 0;
    char line[STRING_SIZE] = "";
    
    while (fgets(line, STRING_SIZE, file) != NULL) {
        char stripedLine[STRING_SIZE] = "";
        int i = 0;
        int j = 0;
        
        for (i = 0; i < strlen(line); ++i) {
            if (line[i] == '\t' || line[i] == '\n' || line[i] == '-' || line[i] == ' ') {
                continue;
            }
            
            stripedLine[j] = line[i];
            ++j;
        }
        
        if (strcmp(stripedLine, query) == 0) {
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
void removeMatchingLine(const char *filename, int lineNumber)
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
// Remove a file named `filename`
//
void removeFile(const char *filename)
{
    char *path = createPath(filename);
    
    // file doesn't exist, stop here
    if (fileExists(path) == 0) {
        free(path);
        return;
    }
    
    remove(path);
    free(path);
}
