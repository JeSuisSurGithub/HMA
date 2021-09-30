#include <HMCD/DirScanUtil.h>

#ifdef __cplusplus
    extern "C" {
#endif

int dsu_is_directory(const char* path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISDIR(path_stat.st_mode);
}

int dsu_is_regfile(const char* path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

unsigned int dsu_get_filesize(const char* filepath)
{
    FILE* file;
    if ((file = fopen(filepath, "rb")) == NULL)
    {
        fprintf(stderr, "dsu_get_filesize(), cannot open: %s\n", filepath);
        return 0;
    }
    fseek(file, 0L, SEEK_END);
    unsigned int filesize = ftell(file);
    fclose(file);
    return filesize;
}

unsigned int dsu_get_file_cnt(const char* path)
{
    DIR* target_dir;
    struct dirent* entries;
    unsigned int file_count = 0;
    if ((target_dir = opendir(path)) == NULL)
    {
        fprintf(stderr, "dsu_get_file_cnt(), cannot open: %s\n", path);
        return 0;
    }
    while ((entries = readdir(target_dir)) != NULL)
    {
        if (strcmp(entries->d_name, ".") != 0 && strcmp(entries->d_name, "..") != 0)
        {
            char* fullpath = (char*)malloc(strlen(path) + strlen(entries->d_name) + 1);
            sprintf(fullpath, "%s%s", path, entries->d_name);
            if (dsu_is_regfile(fullpath))
                file_count++;
            else if (dsu_is_directory(path))
            {
                char* dirpath = (char*)malloc(strlen(path) + strlen(entries->d_name) + 2);
                sprintf(dirpath, "%s%s/", path, entries->d_name);
                file_count += dsu_get_file_cnt(dirpath);
                free(dirpath);
            }
            free(fullpath);
        }
    }
    closedir(target_dir);
    return file_count;
}

void __dsu_get_all_filepath(const char* path, unsigned int* list_iter, char** path_list)
{
    DIR* target_dir;
    struct dirent* entries;
    if ((target_dir = opendir(path)) == NULL)
    {
        fprintf(stderr, "__dsu_get_all_filepath(), cannot open: %s\n", path);
        return;
    }
    while ((entries = readdir(target_dir)) != NULL)
    {
        if (strcmp(entries->d_name, ".") != 0 && strcmp(entries->d_name, "..") != 0)
        {
            char* fullpath = (char*)malloc(strlen(path) + strlen(entries->d_name) + 1);
            sprintf(fullpath, "%s%s", path, entries->d_name);
            if (dsu_is_regfile(fullpath))
            {
                path_list[(*list_iter)] = (char*)malloc(strlen(fullpath) + 1);
                strcpy(path_list[(*list_iter)], fullpath);
                (*list_iter)++;
            }
            else if (dsu_is_directory(path))
            {
                char* dirpath = (char*)malloc(strlen(path) + strlen(entries->d_name) + 2);
                sprintf(dirpath, "%s%s/", path, entries->d_name);
                __dsu_get_all_filepath(dirpath, list_iter, path_list);
                free(dirpath);
            }
            free(fullpath);
        }
    }
    closedir(target_dir);
}

char** dsu_get_all_filepath(const char* path, unsigned int* file_count)
{
    (*file_count) = dsu_get_file_cnt(path);
    unsigned int list_iter = 0;
    char** path_list = (char**)malloc((*file_count) * sizeof(char*));
    DIR* target_dir;
    struct dirent* entries;
    if ((target_dir = opendir(path)) == NULL)
    {
        fprintf(stderr, "dsu_get_all_filepath(), cannot open: %s\n", path);
        return NULL;
    }
    while ((entries = readdir(target_dir)) != NULL)
    {
        if (strcmp(entries->d_name, ".") != 0 && strcmp(entries->d_name, "..") != 0)
        {
            char* fullpath = (char*)malloc(strlen(path) + strlen(entries->d_name) + 1);
            sprintf(fullpath, "%s%s", path, entries->d_name);
            if (dsu_is_regfile(fullpath))
            {
                path_list[list_iter] = (char*)malloc(strlen(fullpath) + 1);
                strcpy(path_list[list_iter], fullpath);
                list_iter++;
            }
            else if (dsu_is_directory(path))
            {
                char* dirpath = (char*)malloc(strlen(path) + strlen(entries->d_name) + 2);
                sprintf(dirpath, "%s%s/", path, entries->d_name);
                __dsu_get_all_filepath(dirpath, &list_iter, path_list);
                free(dirpath);
            }
            free(fullpath);
        }
    }
    closedir(target_dir);
    return path_list;
}

unsigned int dsu_get_dir_cnt(const char* path, bool is_recur)
{
    DIR* target_dir;
    struct dirent* entries;
    unsigned int dir_count = 0;
    if (is_recur == false)
        dir_count = 1;
    if ((target_dir = opendir(path)) == NULL)
    {
        fprintf(stderr, "dsu_get_dir_cnt(), cannot open: %s\n", path);
        return 0;
    }
    while ((entries = readdir(target_dir)) != NULL)
    {
        if (strcmp(entries->d_name, ".") != 0 && strcmp(entries->d_name, "..") != 0)
        {
            char* fullpath = (char*)malloc(strlen(path) + strlen(entries->d_name) + 1);
            sprintf(fullpath, "%s%s", path, entries->d_name);
            if (dsu_is_directory(fullpath))
            {
                dir_count++;
                char* dirpath = (char*)malloc(strlen(path) + strlen(entries->d_name) + 2);
                sprintf(dirpath, "%s%s/", path, entries->d_name);
                dir_count += dsu_get_dir_cnt(dirpath, true);
                free(dirpath);
            }
            free(fullpath);
        }
    }
    closedir(target_dir);
    return dir_count;
}

void __dsu_get_all_dirpath(const char* path, unsigned int* list_iter, char** path_list)
{
    DIR* target_dir;
    struct dirent* entries;
    if ((target_dir = opendir(path)) == NULL)
    {
        fprintf(stderr, "__dsu_get_all_dirpath(), cannot open: %s\n", path);
        return;
    }
    while ((entries = readdir(target_dir)) != NULL)
    {
        if (strcmp(entries->d_name, ".") != 0 && strcmp(entries->d_name, "..") != 0)
        {
            char* fullpath = (char*)malloc(strlen(path) + strlen(entries->d_name) + 1);
            sprintf(fullpath, "%s%s", path, entries->d_name);
            if (dsu_is_directory(fullpath))
            {
                path_list[(*list_iter)] = (char*)malloc(strlen(fullpath) + 1);
                strcpy(path_list[(*list_iter)], fullpath);
                (*list_iter)++;
                char* dirpath = (char*)malloc(strlen(path) + strlen(entries->d_name) + 2);
                sprintf(dirpath, "%s%s/", path, entries->d_name);
                __dsu_get_all_dirpath(dirpath, list_iter, path_list);
                free(dirpath);
            }
            free(fullpath);
        }
    }
    closedir(target_dir);
}

char** dsu_get_all_dirpath(const char* path, unsigned int* dir_count)
{
    (*dir_count) = dsu_get_dir_cnt(path, false);
    unsigned int list_iter = 1;
    char** path_list = (char**)malloc((*dir_count) * sizeof(char*));

    path_list[0] = (char*)malloc(strlen(path) + 1);
    strcpy(path_list[0], path);

    DIR* target_dir;
    struct dirent* entries;
    if ((target_dir = opendir(path)) == NULL)
    {
        fprintf(stderr, "dsu_get_all_dirpath(), cannot open: %s\n", path);
        return NULL;
    }
    while ((entries = readdir(target_dir)) != NULL)
    {
        if (strcmp(entries->d_name, ".") != 0 && strcmp(entries->d_name, "..") != 0)
        {
            char* fullpath = (char*)malloc(strlen(path) + strlen(entries->d_name) + 1);
            sprintf(fullpath, "%s%s", path, entries->d_name);
            if (dsu_is_directory(fullpath))
            {
                path_list[list_iter] = (char*)malloc(strlen(fullpath) + 1);
                strcpy(path_list[list_iter], fullpath);
                list_iter++;
                char* dirpath = (char*)malloc(strlen(path) + strlen(entries->d_name) + 2);
                sprintf(dirpath, "%s%s/", path, entries->d_name);
                __dsu_get_all_dirpath(dirpath, &list_iter, path_list);
                free(dirpath);
            }
            free(fullpath);
        }
    }
    closedir(target_dir);
    return path_list;
}

unsigned int dsu_get_entry_cnt_no_recur(const char* path)
{
    DIR* target_dir;
    struct dirent* entries;
    unsigned int entry_count = 0;
    if ((target_dir = opendir(path)) == NULL)
    {
        fprintf(stderr, "dsu_get_entry_cnt_no_recur(), cannot open: %s\n", path);
        return 0;
    }
    while ((entries = readdir(target_dir)) != NULL)
    {
        if (strcmp(entries->d_name, ".") != 0 && strcmp(entries->d_name, "..") != 0)
            entry_count++;
    }
    closedir(target_dir);
    return entry_count;
}

char** dsu_get_all_entry_no_recur(const char* path, unsigned int* entry_count)
{
    (*entry_count) = dsu_get_entry_cnt_no_recur(path);
    unsigned int list_iter = 0;
    char** entry_list = (char**)malloc((*entry_count) * sizeof(char*));

    DIR* target_dir;
    struct dirent* entries;
    if ((target_dir = opendir(path)) == NULL)
    {
        fprintf(stderr, "dsu_get_all_entry_no_recur(), cannot open: %s\n", path);
        return NULL;
    }
    while ((entries = readdir(target_dir)) != NULL)
    {
        if (strcmp(entries->d_name, ".") != 0 && strcmp(entries->d_name, "..") != 0)
        {
            char* fullpath = (char*)malloc(strlen(path) + strlen(entries->d_name) + 1);
            sprintf(fullpath, "%s%s", path, entries->d_name);
            if (dsu_is_regfile(fullpath))
            {
                entry_list[list_iter] = (char*)malloc(strlen(fullpath) + 1);
                strcpy(entry_list[list_iter], fullpath);
                list_iter++;
            }
            else if (dsu_is_directory(path))
            {
                entry_list[list_iter] = (char*)malloc(strlen(path) + strlen(entries->d_name) + 2);
                sprintf(entry_list[list_iter], "%s%s/", path, entries->d_name);
                list_iter++;
            }
            free(fullpath);
        }
    }
    closedir(target_dir);
    return entry_list;
}

#ifdef __cplusplus
    }
#endif