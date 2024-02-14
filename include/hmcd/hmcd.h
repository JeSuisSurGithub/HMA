/**
 * Honkai Manhua C Downloader (HMCD)
 * Copyright (C) 2022 JeFaisDesSpaghettis
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

#ifndef HMCD_H
#define HMCD_H

#ifdef __cplusplus
    extern "C" {
#endif

#include "hmcd_config.h"

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

#define PROGRESS_BAR_WIDTH 30

typedef uint32_t hmcd_u32;

typedef enum HMCD_SRV_ID_
{
    HMCD_NONE = 0,
    HMCD_CHINA = 1,
    HMCD_GLOBAL = 2
}HMCD_SRV_ID;

typedef struct hmcd_book_
{
    hmcd_u32 book_id;
    const char* book_name;
}hmcd_book;

typedef struct hmcd_srv_
{
    HMCD_SRV_ID server_id;
    const char* name;
    const char* base_url;
    hmcd_u32 book_count;
    hmcd_book books[];
}hmcd_srv;

typedef struct _hmcd_ctx
{
    bool enable_logs;
    char* output_dir;
    const hmcd_srv* server;
    CURL* curl_handle;
}hmcd_ctx;

typedef enum HMCD_ERR_
{
    HMCD_SUCCESS = 0,
    HMCD_UNDEFINED_ERROR = 1,
    HMCD_ERR_FAILED_CURL_INIT = 2,
    HMCD_ERR_FAILED_FOPEN = 3,
    HMCD_ERR_FAILED_CURL_PERFORM = 4,
    HMCD_ERR_FAILED_BOOK_OUT_OF_RANGE = 5
}HMCD_ERR;

static const char* HMCD_CURL_HTTPS_CERT_URL = "https://curl.se/ca/cacert.pem";
static const char* HMCD_GLB_NAME = "global";
static const char* HMCD_CN_NAME = "china";
static const char* HMCD_CERTIFICATE_PATH = "cacert.pem";

static const hmcd_srv HMCD_GLB_SERVER =
{
    HMCD_GLOBAL,
    "global",
    "https://d2tpbmzklky1cl.cloudfront.net/manga/static/comic/book",
    22,
    {
        {1001, "Ai-Chan Facts"         },
        {1002, "Gratitude Arc"         },
        {1003, "AE Invasion"           },
        {1004, "Azure Waters"          },
        {1005, "Second Eruption"       },
        {1006, "Gemina: Tales"         },
        {1007, "World Serpent"         },
        {1008, "Kiana Plays Honkai"    },
        {1009, "London Holiday"        },
        {1010, "Moon Shadow"           },
        {1011, "Elan Palatinus"        },
        {1012, "SpringFest"            },
        {1013, "ELF"                   },
        {1014, "Second Key"            },
        {1015, "Escape From Nagazora"  },
        {1016, "St. Freya High"        },
        {1017, "Gemina: Invasion"      },
        {1018, "Divine Key"            },
        {1019, "Cooking With Valkyries"},
        {1020, "Empyrean Blade"        },
        {1021, "Alien Space"           },
        {1022, "Spring Festival Trip"  }
    }
};

static const hmcd_srv HMCD_CN_SERVER =
{
    HMCD_CHINA,
    "china",
    "https://comicstatic.bh3.com/new_static_v2/comic/book",
    23,
    {
        {1001, u8"逃离长空篇"},
        {1002, u8"樱花追忆篇"},
        {1004, u8"绀海篇"},
        {1005, u8"绯樱篇"},
        {1006, u8"逆熵入侵篇"},
        {1007, u8"恩返篇"},
        {1008, u8"月影篇"},
        {1009, u8"紫鸢篇"},
        {1010, u8"神之键秘话"},
        {1011, u8"玩崩坏3的琪亚娜"},
        {1012, u8"第二次崩坏"},
        {1013, u8"女武神的餐桌"},
        {1014, u8"夏日回忆-预告篇"},
        {1015, u8"双子：起源"},
        {1016, u8"双子：入侵"},
        {1017, u8"蛇之篇"},
        {1018, u8"雾都假日"},
        {1019, u8"年岁"},
        {1020, u8"武装人偶"},
        {1021, u8"传承"},
        {1022, u8"云墨剑心"},
        {1023, u8"异乡"},
        {1024, u8"新春旅行"}
    }
};

void* hmcd_malloc_(size_t size);
char* hmcd_strdup_(const char* str);
void hmcd_log_(bool enable_logs, const char* str);
void hmcd_logf_(bool enable_logs, char* fmt, ...);
void hmcd_mkdir_(const char* path, mode_t mode);

HMCD_ERR hmcd_init_ctx(hmcd_ctx** pcontext, bool enable_logs, const char* output_dir, const hmcd_srv* server);
void hmcd_free_ctx(hmcd_ctx* context);

HMCD_ERR hmcd_get_chap_cnt(hmcd_ctx* context, hmcd_u32* ret_chap_count, hmcd_u32 book_index);

int hmcd_curl_progress_callback_(
    void *clientp,
    curl_off_t dltotal,
    curl_off_t dlnow,
    curl_off_t ultotal,
    curl_off_t ulnow);

HMCD_ERR hmcd_dl_book(
    hmcd_ctx* context,
    hmcd_u32 book_index,
    hmcd_u32 first_chap,
    hmcd_u32 last_chap,
    bool one_directory);

#ifdef __cplusplus
    }
#endif

#endif /* HMCD_H */