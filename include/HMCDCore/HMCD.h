#ifndef HMCD_H
#define HMCD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <curl/curl.h>

#ifdef _WIN32
    #include <windows.h>
#endif

#include "DirScanUtil.h"
#include "HMCDCoreVersion.h"

#define GB_CHAPTER_COUNT 22
#define CN_CHAPTER_COUNT 23

typedef struct _hmcd_book
{
    unsigned int book_id;
    char* book_name;
}hmcd_book;

typedef enum _DL_SERVER
{
    CHINA = 0,
    GLOBAL = 1
}DL_SERVER;

// Global Base URL
static const char* GLB_BASE_URL = "https://d2tpbmzklky1cl.cloudfront.net/manga/static/comic/book";

// China Base URL
static const char* CN_BASE_URL = "https://comicstatic.bh3.com/new_static_v2/comic/book";

// Global output directory
static const char* GLB_OUT_DIR = "./GBBook";

// China output directory
static const char* CN_OUT_DIR = "./CNBook";

// GLOBAL Manga IDs
static const unsigned int HMCD_GLB_AICHAN_FACTS     = 1001;
static const unsigned int HMCD_GLB_GRATITUDE        = 1002;
static const unsigned int HMCD_GLB_AE_INVASION      = 1003;
static const unsigned int HMCD_GLB_AZURE_WATER      = 1004;
static const unsigned int HMCD_GLB_2ND_ERUPTION     = 1005;
static const unsigned int HMCD_GLB_GEMINA_TALES     = 1006;
static const unsigned int HMCD_GLB_WORLD_SERPENT    = 1007;
static const unsigned int HMCD_GLB_KIANA_PLAYS_HI3  = 1008;
static const unsigned int HMCD_GLB_LONDON_HOLIDAY   = 1009;
static const unsigned int HMCD_GLB_MOON_SHADOW      = 1010;
static const unsigned int HMCD_GLB_ELAN_PALATINUS   = 1011;
static const unsigned int HMCD_GLB_SPRINGFEST       = 1012;
static const unsigned int HMCD_GLB_ELF              = 1013;
static const unsigned int HMCD_GLB_SECOND_KEY       = 1014;
static const unsigned int HMCD_GLB_NAGAZORA         = 1015;
static const unsigned int HMCD_GLB_FREYA_HIGH       = 1016;
static const unsigned int HMCD_GLB_GEMINA_INVASION  = 1017;
static const unsigned int HMCD_GLB_DIVINE_KEY       = 1018;
static const unsigned int HMCD_GLB_CWV              = 1019;
static const unsigned int HMCD_GLB_EMPYREAN_BLADE   = 1020;
static const unsigned int HMCD_GLB_ALIEN_SPACE      = 1021;
static const unsigned int HMCD_GLB_SPRING_FESTIVAL  = 1022;

static const char* HMCD_GLB_AICHAN_FACTS_NAME       = "Ai-Chan Facts"         ;
static const char* HMCD_GLB_GRATITUDE_NAME          = "Gratitude Arc"         ;
static const char* HMCD_GLB_AE_INVASION_NAME        = "AE Invasion"           ;
static const char* HMCD_GLB_AZURE_WATER_NAME        = "Azure Waters"          ;
static const char* HMCD_GLB_2ND_ERUPTION_NAME       = "Second Eruption"       ;
static const char* HMCD_GLB_GEMINA_TALES_NAME       = "Gemina: Tales"         ;
static const char* HMCD_GLB_WORLD_SERPENT_NAME      = "World Serpent"         ;
static const char* HMCD_GLB_KIANA_PLAYS_HI3_NAME    = "Kiana Plays Honkai"    ;
static const char* HMCD_GLB_LONDON_HOLIDAY_NAME     = "London Holiday"        ;
static const char* HMCD_GLB_MOON_SHADOW_NAME        = "Moon Shadow"           ;
static const char* HMCD_GLB_ELAN_PALATINUS_NAME     = "Elan Palatinus"        ;
static const char* HMCD_GLB_SPRINGFEST_NAME         = "SpringFest"            ;
static const char* HMCD_GLB_ELF_NAME                = "ELF"                   ;
static const char* HMCD_GLB_SECOND_KEY_NAME         = "Second Key"            ;
static const char* HMCD_GLB_NAGAZORA_NAME           = "Escape From Nagazora"  ;
static const char* HMCD_GLB_FREYA_HIGH_NAME         = "St. Freya High"        ;
static const char* HMCD_GLB_GEMINA_INVASION_NAME    = "Gemina: Invasion"      ;
static const char* HMCD_GLB_DIVINE_KEY_NAME         = "Divine Key"            ;
static const char* HMCD_GLB_CWV_NAME                = "Cooking With Valkyries";
static const char* HMCD_GLB_EMPYREAN_BLADE_NAME     = "Empyrean Blade"        ;
static const char* HMCD_GLB_ALIEN_SPACE_NAME        = "Alien Space"           ;
static const char* HMCD_GLB_SPRING_FESTIVAL_NAME    = "Spring Festival Trip"  ;

// CN Manga IDs
static const unsigned int HMCD_CN_NAGAZORA          = 1001;
static const unsigned int HMCD_CN_HI3               = 1002;
static const unsigned int HMCD_CN_BOHAI     	    = 1004;
static const unsigned int HMCD_CN_SAKURA            = 1005;
static const unsigned int HMCD_CN_AE_INVASION       = 1006;
static const unsigned int HMCD_CN_GRATITUDE_ARC     = 1007;
static const unsigned int HMCD_CN_MOON_SHADOW       = 1008;
static const unsigned int HMCD_CN_ELAN_PALATINUS    = 1009;
static const unsigned int HMCD_CN_GOD_KEY           = 1010;
static const unsigned int HMCD_CN_KIANA             = 1011;
static const unsigned int HMCD_CN_2ND_LAWMAN        = 1012;
static const unsigned int HMCD_CN_CWV               = 1013;
static const unsigned int HMCD_CN_SUMMER_MEM        = 1014;
static const unsigned int HMCD_CN_GEMINA_ORIGINS    = 1015;
static const unsigned int HMCD_CN_GEMINA_INVASION   = 1016;
static const unsigned int HMCD_CN_WORLD_SERPENT     = 1017;
static const unsigned int HMCD_CN_LONDON_HOLIDAY    = 1018;
static const unsigned int HMCD_CN_SPRINGFEST        = 1019;
static const unsigned int HMCD_CN_ELF               = 1020;
static const unsigned int HMCD_CN_SECOND_KEY        = 1021;
static const unsigned int HMCD_CN_EMPYREAN_BLADE    = 1022;
static const unsigned int HMCD_CN_ALIEN_SPACE       = 1023;
static const unsigned int HMCD_CN_SPRING_FESTIVAL   = 1024;

static const char* HMCD_CN_NAGAZORA_NAME            = u8"逃离长空篇";
static const char* HMCD_CN_HI3_NAME                 = u8"樱花追忆篇";
static const char* HMCD_CN_BOHAI_NAME     	        = u8"绀海篇";
static const char* HMCD_CN_SAKURA_NAME              = u8"绯樱篇";
static const char* HMCD_CN_AE_INVASION_NAME         = u8"逆熵入侵篇";
static const char* HMCD_CN_GRATITUDE_ARC_NAME       = u8"恩返篇";
static const char* HMCD_CN_MOON_SHADOW_NAME         = u8"月影篇";
static const char* HMCD_CN_ELAN_PALATINUS_NAME      = u8"紫鸢篇";
static const char* HMCD_CN_GOD_KEY_NAME             = u8"神之键秘话";
static const char* HMCD_CN_KIANA_NAME               = u8"玩崩坏3的琪亚娜";
static const char* HMCD_CN_2ND_LAWMAN_NAME          = u8"第二次崩坏";
static const char* HMCD_CN_CWV_NAME                 = u8"女武神的餐桌";
static const char* HMCD_CN_SUMMER_MEM_NAME          = u8"夏日回忆-预告篇";
static const char* HMCD_CN_GEMINA_ORIGINS_NAME      = u8"双子：起源";
static const char* HMCD_CN_GEMINA_INVASION_NAME     = u8"双子：入侵";
static const char* HMCD_CN_WORLD_SERPENT_NAME       = u8"蛇之篇";
static const char* HMCD_CN_LONDON_HOLIDAY_NAME      = u8"雾都假日";
static const char* HMCD_CN_SPRINGFEST_NAME          = u8"年岁";
static const char* HMCD_CN_ELF_NAME                 = u8"武装人偶";
static const char* HMCD_CN_SECOND_KEY_NAME          = u8"传承";
static const char* HMCD_CN_EMPYREAN_BLADE_NAME      = u8"云墨剑心";
static const char* HMCD_CN_ALIEN_SPACE_NAME         = u8"异乡";
static const char* HMCD_CN_SPRING_FESTIVAL_NAME     = u8"新春旅行";

// Return book
hmcd_book hmcd_construct_book(unsigned int book_id, const char* book_name);

// Get all books
hmcd_book* hmcd_get_glb_books();

// Get all CN books
hmcd_book* hmcd_get_cn_books();

// Check that certificate for https is here
// return >= 0 means success, return < 0 means error
int hmcd_set_https_cert(CURL* curl_handle, const char* certificate_path);

// Get folder size
unsigned long long int hmcd_get_dir_size(const char* dir_name);

// Get chapter count
unsigned int hmcd_get_chap_cnt(hmcd_book target_book, DL_SERVER target_server);

// Download book
// return != 0 means error
int hmcd_dl_book(hmcd_book target_book, unsigned int first_chap, unsigned int last_chap, DL_SERVER target_server);

#endif /* HMCD_H */