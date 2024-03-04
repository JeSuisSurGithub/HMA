/**
 * Honkai Manhua Archiver (HMA)
 * Copyright (C) 2022 JeSuis
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef HMA_H
#define HMA_H

#ifdef __cplusplus
    extern "C" {
#endif

#include "hma_config.h"

#include <curl/curl.h>

#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <sys/stat.h>
#endif

typedef uint32_t hma_u32;

constexpr hma_u32 PROGRESS_BAR_WIDTH = 30;

typedef enum HMA_SRV_ID_
{
    HMA_NONE = 0,
    HMA_CHINA = 1,
    HMA_GLOBAL = 2
}HMA_SRV_ID;

typedef struct hma_book_
{
    hma_u32 book_id;
    const char* book_name;
}hma_book;

typedef struct hma_srv_
{
    HMA_SRV_ID server_id;
    const char* name;
    const char* base_url;
    hma_u32 book_count;
    hma_book books[];
}hma_srv;

typedef struct _hma_ctx
{
    bool enable_logs;
    char* output_dir;
    const hma_srv* server;
    CURL* curl_handle;
}hma_ctx;

typedef enum HMA_ERR_
{
    HMA_SUCCESS = 0,
    HMA_UNDEFINED_ERROR = 1,
    HMA_ERR_FAILED_CURL_INIT = 2,
    HMA_ERR_FAILED_FOPEN = 3,
    HMA_ERR_FAILED_CURL_PERFORM = 4,
    HMA_ERR_FAILED_BOOK_OUT_OF_RANGE = 5
}HMA_ERR;

extern const char* HMA_CURL_HTTPS_CERT_URL;
extern const char* HMA_CERTIFICATE_PATH;

extern const hma_srv HMA_GLB_SERVER;
extern const hma_srv HMA_CN_SERVER;

void hma_log_(bool enable_logs, const char* str);
void hma_logf_(bool enable_logs, char* fmt, ...);
void hma_mkdir_(const char* path, mode_t mode);

HMA_ERR hma_init_ctx(hma_ctx** pcontext, bool enable_logs, const char* output_dir, const hma_srv* server);
void hma_free_ctx(hma_ctx* context);

HMA_ERR hma_get_chap_cnt(hma_ctx* context, hma_u32* ret_chap_count, hma_u32 book_index);

int hma_curl_progress_callback_(
    void *clientp,
    curl_off_t dltotal,
    curl_off_t dlnow,
    curl_off_t,
    curl_off_t);

HMA_ERR hma_dl_book(
    hma_ctx* context,
    hma_u32 book_index,
    hma_u32 first_chap,
    hma_u32 last_chap,
    bool one_directory);

#ifdef __cplusplus
    }
#endif

#endif /* HMA_H */