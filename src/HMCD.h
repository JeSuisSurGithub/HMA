#pragma once
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

#include "directScanUtil.h"

#define GB_CHAPTER_COUNT 21
#define CN_CHAPTER_COUNT 22

typedef struct _BOOK
{
    unsigned int ID;    
    char* Name;
}Book;

// Global Base URL
static const char* globalBaseURL = "https://d2tpbmzklky1cl.cloudfront.net/manga/static/comic/book";

// China Base URL 
static const char* chinaBaseURL = "https://comicstatic.bh3.com/new_static_v2/comic/book";

// GLOBAL Manga IDs
static const unsigned int gbaichanFacts    = 1001;
static const unsigned int gbGratitude      = 1002;
static const unsigned int gbAEInvasion     = 1003;
static const unsigned int gbAzureWater     = 1004;
static const unsigned int gbSecondEruption = 1005;
static const unsigned int gbGeminaTales    = 1006;
static const unsigned int gbWorldSerpent   = 1007;
static const unsigned int gbKianaPlayHI3   = 1008;
static const unsigned int gbLondonHoliday  = 1009;
static const unsigned int gbMoonShadow     = 1010;
static const unsigned int gbElanPalatinus  = 1011;
static const unsigned int gbSpringfest     = 1012;
static const unsigned int gbELF            = 1013;
static const unsigned int gbSecondKey      = 1014;
static const unsigned int gbNagazora       = 1015;
static const unsigned int gbFreyaHigh      = 1016;
static const unsigned int gbGeminaInvasion = 1017;
static const unsigned int gbDivineKey      = 1018;
static const unsigned int gbCWV            = 1019;
static const unsigned int gbEmpyreanBlade  = 1020;
static const unsigned int gbAlienSpace     = 1021;

// CN Manga IDs
static const unsigned int cnNagazora       = 1001;
static const unsigned int cnHI3            = 1002;
static const unsigned int cnBohai          = 1004;
static const unsigned int cnSakura         = 1005;
static const unsigned int cnAEInvasion     = 1006;
static const unsigned int cnGratitudeArc   = 1007;
static const unsigned int cnMoonShadow     = 1008;
static const unsigned int cnElanPalatinus  = 1009;
static const unsigned int cnGodKey         = 1010;
static const unsigned int cnKiana          = 1011;
static const unsigned int cnSecondLawman   = 1012;
static const unsigned int cnCWV            = 1013;
static const unsigned int cnSummerMem      = 1014;
static const unsigned int cnGeminaOrigin   = 1015;
static const unsigned int cnGeminaInvasion = 1016;
static const unsigned int cnWorldSerpent   = 1017;
static const unsigned int cnLondonHoliday  = 1018;
static const unsigned int cnSpringfest     = 1019;
static const unsigned int cnELF            = 1020;
static const unsigned int cnSecondKey      = 1021;
static const unsigned int cnEmpyreanBlade  = 1022;
static const unsigned int cnAlienSpace     = 1023;

// Allow cURL to write to files
size_t writeData(void* Data, unsigned int long long Size, unsigned int long long Count, void* File);

// Return book
Book constructBook(unsigned int ID, char* Name);

// Get all books
Book* getGlobalBooks();

// Get folder size
unsigned long long int getFolderSize(char* folderName);

// Get chapter count
unsigned int getChapterCountASCII(Book bookToScan);

// Download book
void downloadASCIIBook(Book globalBook, unsigned int startRange, unsigned int endRange);

// Get range from user input
void getRange(unsigned int* Start, unsigned int* End, unsigned int chapterCount);

// Guided interface
void guidedInterface();

// Argument
void commandLineInterface(unsigned int serverID, unsigned int bookID, unsigned int startRange, unsigned int endRange);


#endif /* HMCD_H */