#ifndef DIRSCANUTIL_H
#define DIRSCANUTIL_H

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
unsigned int getFileSize(const char* fileName);

// Get number of files(not directory) of a given directory
unsigned int getFileCount(const char* Path);

// Do not call
void getAllFilePathRecursion(const char* Path, unsigned int* fileCount, char** fileNames);

// Return all file paths of a given directory
char** getAllFilePaths(const char* Path, unsigned int* fileCount);

// Get number of directories(not files) of a given directory please set false on call
unsigned int getDirectoryCount(const char* Path, bool Recursive);

// Do not call
void getAllDirectoryPathRecursion(const char* Path, unsigned int* directoryCount, char** directoryNames);

// Return all directory paths of a given directory
char** getAllDirectoryPaths(const char* Path, unsigned int* directoryCount);

// Return element count of a directory
unsigned int getSurfaceElementCount(const char* Path);

// Scan only given directory no recursion
char** getAllSurfaceElements(const char* Path, unsigned int* elementCount);

#endif /* DIRSCANUTIL_H */