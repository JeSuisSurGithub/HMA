#ifndef DIRSCANUTIL_H
#define DIRSCANUTIL_H

#ifdef __cplusplus
    extern "C" {
#endif

// Other headers
#include "HMCD_Version.h"

// Posix
#include <dirent.h>
#include <sys/stat.h>

// Standard
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

// Return true if directory
int dsu_is_directory(const char* path);

// Return true if regular file
int dsu_is_regfile(const char* path);

// Return file size
unsigned int dsu_get_filesize(const char* filepath);

// Get number of files(not directory) of a given directory
unsigned int dsu_get_file_cnt(const char* path);

// Do not call
//void __dsu_get_all_filepath(const char* path, unsigned int* file_count, char** path_list);

// Return all file paths of a given directory
char** dsu_get_all_filepath(const char* path, unsigned int* file_count);

// Get number of directories(not files) of a given directory please set false on call
unsigned int dsu_get_dir_cnt(const char* path, bool is_recur);

// Do not call
//void __dsu_get_all_dirpath(const char* path, unsigned int* dir_count, char** path_list);

// Return all directory paths of a given directory
char** dsu_get_all_dirpath(const char* path, unsigned int* dir_count);

// Return element count of a directory
unsigned int dsu_get_entry_cnt_no_recur(const char* path);

// Scan only given directory no recursion
char** dsu_get_all_entry_no_recur(const char* path, unsigned int* entry_count);

#ifdef __cplusplus
    }
#endif

#endif /* DIRSCANUTIL_H */