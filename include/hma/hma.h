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

#define PROGRESS_BAR_WIDTH 30

typedef uint32_t hma_u32;

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

static const char* HMA_CURL_HTTPS_CERT_URL = "https://curl.se/ca/cacert.pem";
static const char* HMA_GLB_NAME = "global";
static const char* HMA_CN_NAME = "china";
static const char* HMA_CERTIFICATE_PATH = "cacert.pem";

static const hma_srv HMA_GLB_SERVER =
{
    HMA_GLOBAL,
    "global",
    "https://act-webstatic.hoyoverse.com/manga/static/comic/book",
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

static const hma_srv HMA_CN_SERVER =
{
    HMA_CHINA,
    "china",
    "https://act-webstatic.mihoyo.com/new_static_v2/comic/book",
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

void* hma_malloc_(size_t size);
char* hma_strdup_(const char* str);
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
    curl_off_t ultotal,
    curl_off_t ulnow);

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