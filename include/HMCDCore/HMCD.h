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
static const char* glb_base_url = "https://d2tpbmzklky1cl.cloudfront.net/manga/static/comic/book";

// China Base URL
static const char* cn_base_url = "https://comicstatic.bh3.com/new_static_v2/comic/book";

// Global output directory
static const char* glb_out_dir = "./GBBook";

// China output directory
static const char* cn_out_dir = "./CNBook";

// GLOBAL Manga IDs
static const unsigned int glb_aichan_facts      = 1001;
static const unsigned int glb_gratitude         = 1002;
static const unsigned int glb_ae_invasion       = 1003;
static const unsigned int glb_azure_water       = 1004;
static const unsigned int glb_2nd_eruption      = 1005;
static const unsigned int glb_gemina_tales      = 1006;
static const unsigned int glb_world_serpent     = 1007;
static const unsigned int glb_kiana_plays_hi3   = 1008;
static const unsigned int glb_london_holiday    = 1009;
static const unsigned int glb_moon_shadow       = 1010;
static const unsigned int glb_elan_palatinus    = 1011;
static const unsigned int glb_springfest         = 1012;
static const unsigned int glb_elf               = 1013;
static const unsigned int glb_second_key        = 1014;
static const unsigned int glb_nagazora          = 1015;
static const unsigned int glb_freya_high        = 1016;
static const unsigned int glb_gemina_invasion   = 1017;
static const unsigned int glb_divine_key        = 1018;
static const unsigned int glb_cwv               = 1019;
static const unsigned int glb_empyrean_blade    = 1020;
static const unsigned int glb_alien_space       = 1021;
static const unsigned int glb_spring_festival   = 1022;

// CN Manga IDs
static const unsigned int cn_nagazora           = 1001;
static const unsigned int cn_hi3                = 1002;
static const unsigned int cn_bohai              = 1004;
static const unsigned int cn_sakura             = 1005;
static const unsigned int cn_ae_invasion        = 1006;
static const unsigned int cn_gratitude_arc      = 1007;
static const unsigned int cn_moon_shadow        = 1008;
static const unsigned int cn_elan_palatinus     = 1009;
static const unsigned int cn_god_key            = 1010;
static const unsigned int cn_kiana              = 1011;
static const unsigned int cn_2nd_lawman         = 1012;
static const unsigned int cn_cwv                = 1013;
static const unsigned int cn_summer_mem         = 1014;
static const unsigned int cn_gemina_origins     = 1015;
static const unsigned int cn_gemina_invasion    = 1016;
static const unsigned int cn_world_serpent      = 1017;
static const unsigned int cn_london_holiday     = 1018;
static const unsigned int cn_springfest          = 1019;
static const unsigned int cn_elf                = 1020;
static const unsigned int cn_second_key         = 1021;
static const unsigned int cn_empyrean_blade     = 1022;
static const unsigned int cn_alien_space        = 1023;
static const unsigned int cn_spring_festival    = 1024;

// Return book
hmcd_book hmcd_construct_book(unsigned int book_id, const char* book_name);

// Get all books
hmcd_book* hmcd_get_glb_books();

// Get all CN books
hmcd_book* hmcd_get_cn_books();

// Check that certificate for https is here
void hmcd_set_https_cert(CURL* curl_handle, const char* certificate_path);

// Get folder size
unsigned long long int hmcd_get_dir_size(const char* dir_name);

// Get chapter count
unsigned int hmcd_get_chap_cnt(hmcd_book target_book, DL_SERVER target_server);

// Download book
void hmcd_dl_book(hmcd_book target_book, unsigned int first_chap, unsigned int last_chap, DL_SERVER target_server);

#endif /* HMCD_H */