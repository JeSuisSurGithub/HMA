#ifndef DIRECTSCANUTIL_H
#define DIRECTSCANUTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

// Return true if directory
int isDirectory(const char* Path);

// Return true if regular file
int isRegularFile(const char* Path);

// Return file size
unsigned int getFileSize(char* fileName);

// Get number of files(not directory) of a given directory
unsigned int getFileCount(char* Path);

// Do not call
void getAllFilePathRecursion(char* Path, unsigned int* fileCount, char** fileNames);

// Return all file paths of a given directory
char** getAllFilePaths(char* Path, unsigned int* fileCount);

// Get number of directories(not files) of a given directory please set false on call
unsigned int getDirectoryCount(char* Path, bool Recursive);

// Do not call
void getAllDirectoryPathRecursion(char* Path, unsigned int* directoryCount, char** directoryNames);

// Return all directory paths of a given directory
char** getAllDirectoryPaths(char* Path, unsigned int* directoryCount);

// Return element count of a directory
unsigned int getSurfaceElementCount(char* Path);

// Scan only given directory no recursion
char** getAllSurfaceElements(char* Path, unsigned int* elementCount);

#endif /* DIRECTSCANUTIL_H */