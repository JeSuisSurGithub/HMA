#ifndef HMCD_H
#define HMCD_H

#ifdef __cplusplus
    extern "C" {
#endif

// Standard
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Non-std libraries
#include <curl/curl.h>

// Unicode output
#ifdef _WIN32
    #include <windows.h>
#endif

// Other headers
#include "DirScanUtil.h"
#include "HMCD_Version.h"

// Constants

// https certificate url
static const char* HMCD_CURL_HTTPS_CERT_URL = "https://curl.se/ca/cacert.pem";

// Number of books available on the global server last checked: 27/09/2021
static const unsigned int HMCD_GLB_BOOK_COUNT = 22;

// Number of books available on the chinese server last checked: 27/09/2021
static const unsigned int HMCD_CN_BOOK_COUNT = 23;

// Download server
typedef enum _HMCD_SERVER_ID
{
    NONE = 0,
    CHINA = 1,
    GLOBAL = 2
}HMCD_SERVER_ID;

// Structs

/**
 * Structure representing a book
*/
typedef struct _HmcdBook
{
    unsigned int book_id;   //!< ID of the book starting at 1001
    const char* book_name;  //!< Name of the book, may be unicode
}HmcdBook;

/**
 * Structure representing a server
*/
typedef struct _HmcdServer
{
    unsigned int book_count;    //!< Number of books available on this server
    HMCD_SERVER_ID dl_server;   //!< Server identifier indicated by the HMCD_SERVER_ID enum
    const char* base_url;       //!< Base url for books (not ending in /)
    char* out_dir;              //!< Default output directory
    HmcdBook books[];           //!< Array of books available
}HmcdServer;


/**
 * Global server
*/
static const HmcdServer HMCD_GLB_SERVER =
{
    HMCD_GLB_BOOK_COUNT,
    GLOBAL,
    "https://d2tpbmzklky1cl.cloudfront.net/manga/static/comic/book",
    "./GBBook",
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

/**
 * China server
*/
static const HmcdServer HMCD_CN_SERVER =
{
    HMCD_CN_BOOK_COUNT,
    CHINA,
    "https://comicstatic.bh3.com/new_static_v2/comic/book",
    "./CNBook",
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

// Check that certificate for https is here
// return >= 0 means success, return < 0 means error
int hmcd_set_https_cert(CURL* curl_handle, const char* certificate_path);

// Get folder size
unsigned long long int hmcd_get_dir_size(const char* dir_name);

// Get chapter count
unsigned int hmcd_get_chap_cnt(const HmcdServer* target_server, unsigned int book_index);

// Download book
// return != 0 means error
int hmcd_dl_book(const HmcdServer* target_server, unsigned int book_index, unsigned int first_chap, unsigned int last_chap);

#ifdef __cplusplus
    }
#endif
#endif /* HMCD_H */