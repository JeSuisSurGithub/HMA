#ifndef DIRSCANUTIL_H
#define DIRSCANUTIL_H

/**
 * @file include/hmcdcore/dir_scan_util.h
 * @brief Utility to fetch directory contents
*/

#ifdef __cplusplus
    extern "C" {
#endif

// Other headers
#include "hmcdcore_version.h"

// Posix
#include <dirent.h>
#include <sys/stat.h>

// Standard
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * @brief Return true if directory
 * @param path Path to check
 * @return True if stat mode is directory
*/
int dsu_is_directory(const char* path);

/**
 * @brief Return true if regular file
 * @param path Path to check
 * @return True if stat mode is regular file
*/
int dsu_is_regfile(const char* path);

/**
 * @brief Returns file size
 * @param filepath Path to a file
 * @return File size, 0 if unable to open
*/
unsigned int dsu_get_filesize(const char* filepath);

/**
 * @brief Get number of files(no directories) of a given directory
 * @param path Path to a directory
 * @return Number of files
*/
unsigned int dsu_get_file_cnt(const char* path);

/**
 * @brief Do not call
*/
void _dsu_get_all_filepath(const char* path, unsigned int* file_count, char** path_list);

/**
 * @brief Get list of files
 * @param path Path to a directory
 * @param file_count Returns by reference array length
 * @return List of files path relative to directory
*/
char** dsu_get_all_filepath(const char* path, unsigned int* file_count);

/**
 * @brief Get number of directories of a given directory
 * @param path Path to a directory
 * @param is_recur Must be false
 * @return Number of directories
*/
unsigned int dsu_get_dir_cnt(const char* path, bool is_recur);

/**
 * @brief Do not call
*/
void _dsu_get_all_dirpath(const char* path, unsigned int* dir_count, char** path_list);

/**
 * @brief Get list of directory
 * @param path Path to a directory
 * @param dir_count Returns by reference array length
 * @return List of directories path relative to directory given
*/
char** dsu_get_all_dirpath(const char* path, unsigned int* dir_count);

/**
 * @brief Get number of entries on the first level of a directory
 * @param path Path to a directory
 * @return Number of entries
*/
unsigned int dsu_get_entry_cnt_no_recur(const char* path);

/**
 * @brief Get list of entries path on the first level of a directory
 * @param path Path to a directory
 * @param entry_count Returns by reference array length
 * @return List of entries
*/
char** dsu_get_all_entry_no_recur(const char* path, unsigned int* entry_count);

/**
 * @brief Helper to free char** returned by function above
 * @param char_pp Initialized list of c strings
 * @param p_len Length of char_pp
*/
void dsu_free_char_pp(char** char_pp, unsigned int p_len);

#ifdef __cplusplus
    }
#endif

#endif /* DIRSCANUTIL_H */