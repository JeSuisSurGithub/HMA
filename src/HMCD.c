#include "HMCD.h"

Book constructBook(unsigned int ID, const char* Name)
{
    Book _Book;
    _Book.ID = ID;
    _Book.Name = malloc(strlen(Name) + 1);
    strcpy(_Book.Name, Name);
    return _Book;
}

Book* getGlobalBooks()
{
    Book* bookInfo = malloc(sizeof(Book) * GB_CHAPTER_COUNT );
    bookInfo[0]  = constructBook(gbaichanFacts,    "Ai-Chan Facts"         );
    bookInfo[1]  = constructBook(gbGratitude,      "Gratitude Arc"         );
    bookInfo[2]  = constructBook(gbAEInvasion,     "AE Invasion"           );
    bookInfo[3]  = constructBook(gbAzureWater,     "Azure Waters"          );
    bookInfo[4]  = constructBook(gbSecondEruption, "Second Eruption"       );
    bookInfo[5]  = constructBook(gbGeminaTales,    "Gemina: Tales"         );
    bookInfo[6]  = constructBook(gbWorldSerpent,   "World Serpent"         );
    bookInfo[7]  = constructBook(gbKianaPlayHI3,   "Kiana Plays Honkai"    );
    bookInfo[8]  = constructBook(gbLondonHoliday,  "London Holiday"        );
    bookInfo[9]  = constructBook(gbMoonShadow,     "Moon Shadow"           );
    bookInfo[10] = constructBook(gbElanPalatinus,  "Elan Palatinus"        );
    bookInfo[11] = constructBook(gbSpringfest,     "SpringFest"            );
    bookInfo[12] = constructBook(gbELF,            "ELF"                   );
    bookInfo[13] = constructBook(gbSecondKey,      "Second Key"            );
    bookInfo[14] = constructBook(gbNagazora,       "Escape From Nagazora"  );
    bookInfo[15] = constructBook(gbFreyaHigh,      "St. Freya High"        );
    bookInfo[16] = constructBook(gbGeminaInvasion, "Gemina: Invasion"      );
    bookInfo[17] = constructBook(gbDivineKey,      "Divine Key"            );
    bookInfo[18] = constructBook(gbCWV,            "Cooking With Valkyries");
    bookInfo[19] = constructBook(gbEmpyreanBlade,  "Empyrean Blade"        );
    bookInfo[20] = constructBook(gbAlienSpace,     "Alien Space"           );
    bookInfo[21] = constructBook(gbSpringFestival, "Spring Festival Trip"  );
    return bookInfo;
}

Book* getChinaBook()
{
    Book* bookInfo = malloc(sizeof(Book) * CN_CHAPTER_COUNT);
    bookInfo[0]  = constructBook(cnNagazora,       u8"逃离长空篇");
    bookInfo[1]  = constructBook(cnHI3,            u8"樱花追忆篇");
    bookInfo[2]  = constructBook(cnBohai,          u8"绀海篇");
    bookInfo[3]  = constructBook(cnSakura,         u8"绯樱篇");
    bookInfo[4]  = constructBook(cnAEInvasion,     u8"逆熵入侵篇");   
    bookInfo[5]  = constructBook(cnGratitudeArc,   u8"恩返篇");  
    bookInfo[6]  = constructBook(cnMoonShadow,     u8"月影篇");  
    bookInfo[7]  = constructBook(cnElanPalatinus,  u8"紫鸢篇");  
    bookInfo[8]  = constructBook(cnGodKey,         u8"神之键秘话");  
    bookInfo[9]  = constructBook(cnKiana,          u8"玩崩坏3的琪亚娜");  
    bookInfo[10] = constructBook(cnSecondLawman,   u8"第二次崩坏");  
    bookInfo[11] = constructBook(cnCWV,            u8"女武神的餐桌");  
    bookInfo[12] = constructBook(cnSummerMem,      u8"夏日回忆-预告篇");  
    bookInfo[13] = constructBook(cnGeminaOrigin,   u8"双子：起源");  
    bookInfo[14] = constructBook(cnGeminaInvasion, u8"双子：入侵");  
    bookInfo[15] = constructBook(cnWorldSerpent,   u8"蛇之篇");  
    bookInfo[16] = constructBook(cnLondonHoliday,  u8"雾都假日"); 
    bookInfo[17] = constructBook(cnSpringfest,     u8"年岁");  
    bookInfo[18] = constructBook(cnELF,            u8"武装人偶");  
    bookInfo[19] = constructBook(cnSecondKey,      u8"传承");  
    bookInfo[20] = constructBook(cnEmpyreanBlade,  u8"云墨剑心");  
    bookInfo[21] = constructBook(cnAlienSpace,     u8"异乡"); 
    bookInfo[22] = constructBook(cnSpringFestival, u8"新春旅行"); 
    return bookInfo;
}

void setCA(CURL* curlHandle, const char* certificatePath)
{
    FILE* Certificate;
    if ((Certificate = fopen(certificatePath, "r+")) != NULL)
    {
        printf("Certificate exists!\n");
        curl_easy_setopt(curlHandle, CURLOPT_CAINFO, certificatePath);
    }
    else
    {
        printf("Certificate does not exists downloading from https://curl.se/ca/cacert.pem...\n");
        CURL* certHandle = curl_easy_init();
        curl_easy_setopt(certHandle, CURLOPT_URL, "https://curl.se/ca/cacert.pem");
        curl_easy_setopt(certHandle, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(certHandle, CURLOPT_NOBODY, 0L);
        curl_easy_setopt(certHandle, CURLOPT_VERBOSE, 0L);
        curl_easy_setopt(certHandle, CURLOPT_NOPROGRESS, 1L);
        curl_easy_setopt(certHandle, CURLOPT_WRITEFUNCTION, fwrite);

	    FILE* Certificate = fopen(certificatePath, "wb+");
        curl_easy_setopt(certHandle, CURLOPT_WRITEDATA, Certificate);
        curl_easy_perform(certHandle);
        curl_easy_setopt(curlHandle, CURLOPT_CAINFO, certificatePath);
        fclose(Certificate);

        curl_easy_cleanup(certHandle);
        printf("Certificate downloaded from https://curl.se/ca/cacert.pem...\n");
    }
}

unsigned long long int getFolderSize(const char* folderName)
{
    unsigned int fileCount = 0;
    char** filePaths = getAllFilePaths(folderName, &fileCount);
    unsigned long long int totalFolderSize = 0;
    for (unsigned int Index = 0; Index < fileCount; Index++)
        totalFolderSize += getFileSize(filePaths[Index]);

    for (unsigned int Index = 0; Index < fileCount; Index++)
        free(filePaths[Index]);
    free(filePaths);
    return totalFolderSize;
}

unsigned int getChapterCount(Book bookToScan, SERVER Target)
{
    const char* targetBaseURL = (Target == CHINA) ? chinaBaseURL : globalBaseURL;
    printf("Getting chapter count...\n");
    CURL* curlCheck = curl_easy_init();
    setCA(curlCheck, "./cacert.pem");
    curl_easy_setopt(curlCheck, CURLOPT_NOBODY, 1L);
    unsigned int chapterCount = 0;
    for (chapterCount = 1; chapterCount <= 100; chapterCount++)
    {
        char* bookURL = (chapterCount >= 10) ?
            malloc(strlen(targetBaseURL) + 1 + 4 + 1 + 2 + 9 + 1) : malloc(strlen(targetBaseURL) + 1 + 4 + 1 + 1 + 9 + 1);
        sprintf(bookURL, "%s/%i/%i/0001.jpg", targetBaseURL, bookToScan.ID, chapterCount);
        curl_easy_setopt(curlCheck, CURLOPT_URL, bookURL);
        curl_easy_perform(curlCheck);
        long httpCode = 0;
        curl_easy_getinfo(curlCheck, CURLINFO_RESPONSE_CODE, &httpCode);
        if (httpCode != 200)
        {
            printf("Chapter %i does not exists\n", chapterCount);
            printf("Chapter count is %i\n", (chapterCount - 1));
            curl_easy_cleanup(curlCheck);
            free(bookURL);
            return chapterCount;
        }
        else 
        {
            printf("Chapter %i exists\n", chapterCount);
        }
        free(bookURL);
    }
    return chapterCount;
}

void downloadBook(Book _Book, unsigned int startRange, unsigned int endRange, SERVER Target)
{
    const char* targetBaseURL = (Target == CHINA) ? chinaBaseURL : globalBaseURL;
    const char* targetOutput = (Target == CHINA) ? chinaOutput : globalOutput;
    clock_t Start = clock();
    char* bookDirName = malloc(strlen(targetOutput) + 4 + 1);
    sprintf(bookDirName, "%s%i", targetOutput, _Book.ID);
#ifdef _WIN32
    mkdir(bookDirName);
#else
    mkdir(bookDirName, 0777);
#endif
    // For all chapter in the range
    CURL* curlCheck = curl_easy_init();
    setCA(curlCheck, "./cacert.pem");
    curl_easy_setopt(curlCheck, CURLOPT_NOBODY, 1L);
    
    CURL* curlDownload = curl_easy_init();
    setCA(curlDownload, "./cacert.pem");
    curl_easy_setopt(curlDownload, CURLOPT_HEADER, 0L);

    for (unsigned int chapterCount = startRange; chapterCount <= endRange; chapterCount++)
    {
        char* chapterURL = (chapterCount >= 10) ?
            malloc(strlen(targetBaseURL) + 1 + 4 + 1 + 2 + 1 + 1) : malloc(strlen(targetBaseURL) + 1 + 4 + 1 + 1 + 1 + 1);
        sprintf(chapterURL, "%s/%i/%i/", targetBaseURL, _Book.ID, chapterCount);
        char* chapterDirName = malloc(strlen(bookDirName) + strlen("/Chapter") + 2 + 1);
        (chapterCount >= 10) ?
            sprintf(chapterDirName, "%s/Chapter%i", bookDirName, chapterCount) : 
            sprintf(chapterDirName, "%s/Chapter0%i", bookDirName, chapterCount);
#ifdef _WIN32
        mkdir(chapterDirName);
#else
        mkdir(chapterDirName, 0777);
#endif
        printf("Downloading chapter %i\n", chapterCount);
        unsigned int pageCount = 0;
        for (pageCount = 1; pageCount <= 100; pageCount++) // Arbitrary great number
        {
            char* pageURL = malloc(strlen(chapterURL) + 8 + 1);
            (pageCount >= 10) ?
                sprintf(pageURL, "%s00%i.jpg", chapterURL, pageCount) : sprintf(pageURL, "%s000%i.jpg", chapterURL, pageCount);
            char* pageName = malloc(strlen(targetOutput) + 4 + 8 + 2 + 1 + 2 + 2 + 4 + 1);;

            if (pageCount < 10)
                sprintf(pageName, "%s/0%i0%i.jpg", chapterDirName, chapterCount, pageCount);
            
            else 
                sprintf(pageName, "%s/0%i%i.jpg", chapterDirName, chapterCount, pageCount);

            curl_easy_setopt(curlCheck, CURLOPT_URL, pageURL);
            curl_easy_perform(curlCheck);
            long httpCode = 0;
            curl_easy_getinfo(curlCheck, CURLINFO_HTTP_CODE, &httpCode);
            if (httpCode == 200)
            {
                curl_easy_setopt(curlDownload, CURLOPT_URL, pageURL);
                curl_easy_setopt(curlDownload, CURLOPT_VERBOSE, 0L);
	            curl_easy_setopt(curlDownload, CURLOPT_NOPROGRESS, 1L);
                curl_easy_setopt(curlDownload, CURLOPT_WRITEFUNCTION, fwrite);
                FILE* Page = fopen(pageName, "wb+");
	            curl_easy_setopt(curlDownload, CURLOPT_WRITEDATA, Page);
	            curl_easy_perform(curlDownload);
	            fclose(Page);
                printf("%s => %s\n", pageURL, pageName);
                free(pageURL);
                free(pageName);
            }
            else 
            {
                printf("Can't download page %i of chapter %i\n", pageCount, chapterCount);
                free(pageURL);
                free(pageName);
                break;
            }
        }
        free(chapterDirName);
        free(chapterURL);
    }
    free(bookDirName);
    curl_easy_cleanup(curlCheck);
    curl_easy_cleanup(curlDownload);
    clock_t End = clock();
    End = clock();
    float elapsedSeconds = ((float)(End - Start)) / CLOCKS_PER_SEC;
    printf("Finished downloading book %s on %s server(chapter %i to %i) in %f (seconds)\n", 
        _Book.Name, 
        (Target == CHINA) ? "china" : "global", 
        startRange, 
        endRange, 
        elapsedSeconds);
}

// Gets range from input
void getRange(unsigned int* Start, unsigned int* End, unsigned int chapterCount)
{
    while (true)
    {
        printf("The max range is 1-%i\n", (chapterCount - 1));
        printf("Please type the chapter from where to start from(including it): ");
        scanf("%i", Start);
        if (*Start < 1 || *Start > (chapterCount - 1))
            printf("Start range cannot be smaller than 1 or greater than %i\n", chapterCount);

        printf("Please type the chapter from where to stop(including it): ");
        scanf("%i", End);
        if (*End > (chapterCount - 1) || *End < 1)
            printf("End range cannot be greater than %i or smaller than 1\n", chapterCount);

        if (*Start > *End)
            printf("Start range cannot be greater than end range");
        if ((*Start <= *End) && (*End <= (chapterCount - 1) && *End >= 1) && (*Start >= 1 || *Start <= (chapterCount - 1)))
            break;
    }
}

void guidedInterface()
{
    // Get server
    printf("Welcome to HI3 manga C downloader!(The C version of HMPD)\n\n");
    printf("Get more help or report issues at https://github.com/JeFaitDesSpaghettis/HMCD\n");
    unsigned int serverInt = 0;
    printf("Type 1 to download from global server\nType 2 to download from CN server\nType the corresponding number: ");
    scanf("%i", &serverInt);
    putchar('\n');
    // Show available books and get range from user input
    if (serverInt == 1)
    {
        Book* bookInfo = getGlobalBooks();
        for (unsigned int Index = 0; Index < GB_CHAPTER_COUNT; Index++)
        {
            printf("Type %i to download the book %i AKA %s\n", Index, bookInfo[Index].ID, bookInfo[Index].Name);
        }
        
        unsigned int bookIndex = 0;
        scanf("%i", &bookIndex);
        if (bookIndex < 0 || bookIndex >= GB_CHAPTER_COUNT)
        {
            printf("Not valid input\n");
            exit(-1);
        }

        unsigned int Start = 0;
        unsigned int End = 0;
        unsigned int chapterCount = getChapterCount(bookInfo[bookIndex], GLOBAL);
        getRange(&Start, &End, chapterCount);
        downloadBook(bookInfo[bookIndex], Start, End, GLOBAL);
        char* folderName = malloc(8 + 4 + 1 + 1);
        sprintf(folderName, "%s%i/", globalOutput, bookInfo[bookIndex].ID);
        printf("Total diskspace taken by \"%s%i\": %llu (bytes)\n", globalOutput, bookInfo[bookIndex].ID, getFolderSize(folderName));
        free(bookInfo);
    }
    else if (serverInt == 2)
    {
#ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
#endif
        Book* bookInfo = getChinaBook();
        for (unsigned int Index = 0; Index < CN_CHAPTER_COUNT; Index++)
        {
            printf(u8"Type %i to download the book %i AKA %s\n", Index, bookInfo[Index].ID, bookInfo[Index].Name);
        }
        printf("Names may haven't been shown correctly use the Windows Terminal instead\n");
        
        unsigned int bookIndex = 0;
        scanf("%i", &bookIndex);
        if (bookIndex < 0 || bookIndex >= CN_CHAPTER_COUNT)
        {
            printf("Not valid input\n");
            exit(-1);
        }
        unsigned int Start = 0;
        unsigned int End = 0;
        unsigned int chapterCount = getChapterCount(bookInfo[bookIndex], CHINA);
        getRange(&Start, &End, chapterCount);
        downloadBook(bookInfo[bookIndex], Start, End, CHINA);
        char* folderName = malloc(8 + 4 + 1 + 1);
        sprintf(folderName, "%s%i/", chinaOutput, bookInfo[bookIndex].ID);
        printf("Total diskspace taken by \"%s%i\": %llu (bytes)\n", chinaOutput, bookInfo[bookIndex].ID, getFolderSize(folderName));
        free(bookInfo);
    }
    else 
        printf("Not a valid choice\n");

    printf("Press any key to exit...\n");
    getchar();
}

void commandLineInterface(unsigned int serverID, unsigned int bookID, unsigned int startRange, unsigned int endRange)
{
    if (serverID == 1)
    {
        Book* bookInfo = getGlobalBooks();
        bool bookIDValid = false;
        unsigned int bookIndex = 0;
        for (bookIndex = 0; bookIndex < GB_CHAPTER_COUNT; bookIndex++)
        {
            if (bookID == bookInfo[bookIndex].ID)
            {
                bookIDValid = true;
                printf("Target book: %s\n", bookInfo[bookIndex].Name);
                break;
            }
        }
        if (bookIDValid == false)
        {
            printf("Invalid book ID");
            return;
        }
        unsigned int chapterCount = getChapterCount(bookInfo[bookIndex], GLOBAL);
        if ((startRange <= endRange) && (endRange <= (chapterCount - 1) && endRange >= 1) && (startRange >= 1 || startRange <= (chapterCount - 1)))
            downloadBook(bookInfo[bookIndex], startRange, endRange, GLOBAL);
        else
        {
            printf("Invalid range\n");
        }
    }
    else if (serverID == 2)
    {
#ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
#endif
        Book* bookInfo = getChinaBook();
        bool bookIDValid = false;
        unsigned int bookIndex = 0;
        for (bookIndex = 0; bookIndex < CN_CHAPTER_COUNT; bookIndex++)
        {
            if (bookID == bookInfo[bookIndex].ID)
            {
                bookIDValid = true;
                printf(u8"Target book: %s\n", bookInfo[bookIndex].Name);
                break;
            }
        }
        if (bookIDValid == false)
        {
            printf("Invalid book ID");
            return;
        }
        unsigned int chapterCount = getChapterCount(bookInfo[bookIndex], CHINA);
        if ((startRange <= endRange) && (endRange <= (chapterCount - 1) && endRange >= 1) && (startRange >= 1 || startRange <= (chapterCount - 1)))
            downloadBook(bookInfo[bookIndex], startRange, endRange, CHINA);
        else
        {
            printf("Invalid range\n");
        }
        
    }
    else 
    {
        printf("First value is invalid, GLOBAL = 1, CN = 2\n");
        exit(-1);
    }
    printf("Press any key to exit...\n");
    getchar();
}