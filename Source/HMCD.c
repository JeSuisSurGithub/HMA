#include "HMCD.h"

size_t writeData(void* Data, unsigned int long long Size, unsigned int long long Count, void* File)
{
	size_t Written = fwrite(Data, Size, Count, (FILE*)File);
	return Written;
}

Book constructBook(unsigned int ID, char* Name)
{
    Book _Book;
    _Book.ID = ID;
    _Book.Name = malloc(strlen(Name) + 1);
    strcpy(_Book.Name, Name);
    return _Book;
}

Book* getGlobalBooks()
{
    Book* bookInfo = malloc(sizeof(Book) * 21);
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
    return bookInfo;
}

Book* getChinaBook()
{
    Book* bookInfo = malloc(sizeof(Book) * 22);
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
    return bookInfo;
}

void setCA(CURL* curlHandle, char* certificatePath)
{
    FILE* Certificate;
    if((Certificate = fopen(certificatePath, "r+")) != NULL)
    {
        printf("Certificate exists!\n");
        curl_easy_setopt(curlHandle, CURLOPT_CAINFO, certificatePath);
    }
    else
    {
        printf("Certificate does not exists downloading from https://curl.haxx.se/ca/cacert.pem...\n");
        CURL* getCert = curl_easy_init();
        curl_easy_setopt(getCert, CURLOPT_URL, "https://curl.haxx.se/ca/cacert.pem");
        curl_easy_setopt(getCert, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(getCert, CURLOPT_NOBODY, 0L);
        curl_easy_setopt(getCert, CURLOPT_VERBOSE, 0L);
        curl_easy_setopt(getCert, CURLOPT_NOPROGRESS, 1L);
        curl_easy_setopt(getCert, CURLOPT_WRITEFUNCTION, writeData);
	    FILE* Page = fopen(certificatePath, "wb+");
        curl_easy_setopt(getCert, CURLOPT_WRITEDATA, Page);
        curl_easy_perform(getCert);
        curl_easy_setopt(curlHandle, CURLOPT_CAINFO, certificatePath);

        fclose(Page);
        curl_easy_cleanup(getCert);
        printf("Certificate downloaded from https://curl.haxx.se/ca/cacert.pem...\n");
    }
}

unsigned long long int getFolderSize(char* folderName)
{
    unsigned int fileCount = 0;
    char** filePaths = getAllFilePaths(folderName, &fileCount);
    unsigned long long int totalFolderSize = 0;
    for (unsigned int Index = 0; Index < fileCount; Index++)
    {
        totalFolderSize += getFileSize(filePaths[Index]);
    }
    for (unsigned int Index = 0; Index < fileCount; Index++)
        free(filePaths[Index]);
    free(filePaths);
    return totalFolderSize;
}

unsigned int getChapterCountGlobal(Book bookToScan)
{
    printf("Getting chapter count...\n");
    CURL* curlCheck = curl_easy_init();
    setCA(curlCheck, "./cacert.pem");
    curl_easy_setopt(curlCheck, CURLOPT_NOBODY, 1L);
    unsigned int chapterCount = 0;
    for (chapterCount = 1; chapterCount <= 100; chapterCount++)
    {
        char* bookURL = (chapterCount >= 10) ?
        malloc(strlen(globalBaseURL) + 1 + 4 + 1 + 2 + 9 + 1) : malloc(strlen(globalBaseURL) + 1 + 4 + 1 + 1 + 9 + 1);
        sprintf(bookURL, "%s/%i/%i/0001.jpg", globalBaseURL, bookToScan.ID, chapterCount);
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

unsigned int getChapterCountChina(Book bookToScan)
{
    printf("Getting chapter count...\n");
    CURL* curlCheck = curl_easy_init();
    setCA(curlCheck, "./cacert.pem");
    curl_easy_setopt(curlCheck, CURLOPT_NOBODY, 1L);
    unsigned int chapterCount = 0;
    for (chapterCount = 1; chapterCount <= 100; chapterCount++)
    {
        char* bookURL = (chapterCount >= 10) ?
        malloc(strlen(chinaBaseURL) + 1 + 4 + 1 + 2 + 9 + 1) : malloc(strlen(chinaBaseURL) + 1 + 4 + 1 + 1 + 9 + 1);
        sprintf(bookURL, "%s/%i/%i/0001.jpg", chinaBaseURL, bookToScan.ID, chapterCount);
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

void downloadGlobalBook(Book globalBook, unsigned int startRange, unsigned int endRange)
{
    clock_t Start = clock();
#ifdef _WIN32
    char* lv0dirName = malloc(8 + 4 + 1);
    sprintf(lv0dirName, "./GBBook%i", globalBook.ID);
    mkdir(lv0dirName);
    free(lv0dirName);
#else
    char* lv0dirName = malloc(8 + 4 + 1);
    sprintf(lv0dirName, "./GBBook%i", globalBook.ID);
    mkdir(lv0dirName, 0777);
    free(lv0dirName);
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
        char* lv1concatURL = (chapterCount >= 10) ?
        malloc(strlen(globalBaseURL) + 1 + 4 + 1 + 2 + 1 + 1) : malloc(strlen(globalBaseURL) + 1 + 4 + 1 + 1 + 1 + 1);
        sprintf(lv1concatURL, "%s/%i/%i/", globalBaseURL, globalBook.ID, chapterCount);
        char* lv1dirName = malloc(strlen("./GBBook") + 4 + strlen("/Chapter") + 2 + 1);
        (chapterCount >= 10) ?
        sprintf(lv1dirName, "./GBBook%i/Chapter%i", globalBook.ID, chapterCount) : sprintf(lv1dirName, "./GBBook%i/Chapter0%i", globalBook.ID, chapterCount);
#ifdef _WIN32
        mkdir(lv1dirName);
#else
        mkdir(lv1dirName, 0777);
#endif
        printf("Downloading chapter %i\n", chapterCount);
        unsigned int pageCount = 0;
        for (pageCount = 1; pageCount <= 100; pageCount++)
        {
            char* lv2concatURL = malloc(strlen(lv1concatURL) + 8 + 1);
            (pageCount >= 10) ?
            sprintf(lv2concatURL, "%s00%i.jpg", lv1concatURL, pageCount) : sprintf(lv2concatURL, "%s000%i.jpg", lv1concatURL, pageCount);
            char* lv1localFileName = malloc(strlen("./GBBook") + 4 + 8 + 2 + 1 + 2 + 2 + 4 + 1);;
            if (chapterCount < 10)
            {
                if (pageCount < 10)
                    sprintf(lv1localFileName, "./GBBook%i/Chapter0%i/0%i0%i.jpg", globalBook.ID, chapterCount, chapterCount, pageCount);
                
                else 
                    sprintf(lv1localFileName, "./GBBook%i/Chapter0%i/0%i%i.jpg", globalBook.ID, chapterCount, chapterCount, pageCount);
            }
            else 
            {
                if (pageCount < 10)
                    sprintf(lv1localFileName, "./GBBook%i/Chapter0%i/%i0%i.jpg", globalBook.ID, chapterCount, chapterCount, pageCount);

                else 
                    sprintf(lv1localFileName, "./GBBook%i/Chapter0%i/%i%i.jpg", globalBook.ID, chapterCount, chapterCount, pageCount);
            }
            curl_easy_setopt(curlCheck, CURLOPT_URL, lv2concatURL);
            curl_easy_perform(curlCheck);
            long httpCode = 0;
            curl_easy_getinfo(curlCheck, CURLINFO_HTTP_CODE, &httpCode);
            if (httpCode == 200)
            {
                curl_easy_setopt(curlDownload, CURLOPT_URL, lv2concatURL);
                curl_easy_setopt(curlDownload, CURLOPT_VERBOSE, 0L);
	            curl_easy_setopt(curlDownload, CURLOPT_NOPROGRESS, 1L);
                curl_easy_setopt(curlDownload, CURLOPT_WRITEFUNCTION, writeData);
                FILE* Page = fopen(lv1localFileName, "wb+");
	            curl_easy_setopt(curlDownload, CURLOPT_WRITEDATA, Page);
	            curl_easy_perform(curlDownload);
	            fclose(Page);
                printf("%s => %s\n", lv2concatURL, lv1localFileName);
                free(lv2concatURL);
                free(lv1localFileName);
            }
            else 
            {
                printf("Can't download page %i of chapter %i\n", pageCount, chapterCount);
                free(lv2concatURL);
                free(lv1localFileName);
                break;
            }
        }
        free(lv1dirName);
        free(lv1concatURL);
    }
    curl_easy_cleanup(curlCheck);
    curl_easy_cleanup(curlDownload);
    clock_t End = clock();
    End = clock();
    float elapsedSeconds = ((float)(End - Start)) / CLOCKS_PER_SEC;
    printf("Finished downloading book %s on Global server(chapter %i to %i) in %f (seconds)\n", globalBook.Name, startRange, endRange, elapsedSeconds);
}

void downloadChinaBook(Book cnBook, unsigned int startRange, unsigned int endRange)
{
    clock_t Start = clock();
#ifdef _WIN32
    char* lv0dirName = malloc(8 + 4 + 1);
    sprintf(lv0dirName, "./CNBook%i", cnBook.ID);
    mkdir(lv0dirName);
    free(lv0dirName);
#else
    char* lv0dirName = malloc(8 + 4 + 1);
    sprintf(lv0dirName, "./CNBook%i", cnBook.ID);
    mkdir(lv0dirName, 0777);
    free(lv0dirName);
#endif
    // For all chapter in the range
    CURL* curlCheck = curl_easy_init();
    setCA(curlCheck, "./cacert.pem");
    curl_easy_setopt(curlCheck, CURLOPT_NOBODY, 1L);
    
    CURL* curlDownload = curl_easy_init();
    setCA(curlDownload, "./cacert.pem");;
    curl_easy_setopt(curlDownload, CURLOPT_HEADER, 0L);

    for (unsigned int chapterCount = startRange; chapterCount <= endRange; chapterCount++)
    {
        char* lv1concatURL = (chapterCount >= 10) ?
        malloc(strlen(chinaBaseURL) + 1 + 4 + 1 + 2 + 1 + 1) : malloc(strlen(chinaBaseURL) + 1 + 4 + 1 + 1 + 1 + 1);
        sprintf(lv1concatURL, "%s/%i/%i/", chinaBaseURL, cnBook.ID, chapterCount);
        char* lv1dirName = malloc(strlen("./CNBook") + 4 + strlen("/Chapter") + 2 + 1);
        (chapterCount >= 10) ?
        sprintf(lv1dirName, "./CNBook%i/Chapter%i", cnBook.ID, chapterCount) : sprintf(lv1dirName, "./CNBook%i/Chapter0%i", cnBook.ID, chapterCount);
#ifdef _WIN32
	mkdir(lv1dirName);
#else
	mkdir(lv1dirName, 0777);
#endif
	printf("Downloading chapter %i\n", chapterCount);
        unsigned int pageCount = 0;
        for (pageCount = 1; pageCount <= 100; pageCount++)
        {
            char* lv2concatURL = malloc(strlen(lv1concatURL) + 8 + 1);
            (pageCount >= 10) ?
            sprintf(lv2concatURL, "%s00%i.jpg", lv1concatURL, pageCount) : sprintf(lv2concatURL, "%s000%i.jpg", lv1concatURL, pageCount);
            char* lv1localFileName = malloc(strlen("./CNBook") + 4 + 8 + 2 + 1 + 2 + 2 + 4 + 1);;
            if (chapterCount < 10)
            {
                if (pageCount < 10)
                    sprintf(lv1localFileName, "./CNBook%i/Chapter0%i/0%i0%i.jpg", cnBook.ID, chapterCount, chapterCount, pageCount);
                
                else 
                    sprintf(lv1localFileName, "./CNBook%i/Chapter0%i/0%i%i.jpg", cnBook.ID, chapterCount, chapterCount, pageCount);
            }
            else 
            {
                if (pageCount < 10)
                    sprintf(lv1localFileName, "./CNBook%i/Chapter0%i/%i0%i.jpg", cnBook.ID, chapterCount, chapterCount, pageCount);

                else 
                    sprintf(lv1localFileName, "./CNBook%i/Chapter0%i/%i%i.jpg", cnBook.ID, chapterCount, chapterCount, pageCount);
            }
            curl_easy_setopt(curlCheck, CURLOPT_URL, lv2concatURL);
            curl_easy_perform(curlCheck);
            long httpCode = 0;
            curl_easy_getinfo(curlCheck, CURLINFO_HTTP_CODE, &httpCode);
            if (httpCode == 200)
            {
                curl_easy_setopt(curlDownload, CURLOPT_URL, lv2concatURL);
                curl_easy_setopt(curlDownload, CURLOPT_VERBOSE, 0L);
	            curl_easy_setopt(curlDownload, CURLOPT_NOPROGRESS, 1L);
                curl_easy_setopt(curlDownload, CURLOPT_WRITEFUNCTION, writeData);
                FILE* Page = fopen(lv1localFileName, "wb+");
	            curl_easy_setopt(curlDownload, CURLOPT_WRITEDATA, Page);
	            curl_easy_perform(curlDownload);
	            fclose(Page);
                printf("%s => %s\n", lv2concatURL, lv1localFileName);
                free(lv2concatURL);
                free(lv1localFileName);
            }
            else 
            {
                printf("Can't download page %i of chapter %i (%s)\n", pageCount, chapterCount, lv2concatURL);
                free(lv2concatURL);
                free(lv1localFileName);
                break;
            }
        }
        free(lv1dirName);
        free(lv1concatURL);
    }
    curl_easy_cleanup(curlCheck);
    curl_easy_cleanup(curlDownload);
    clock_t End = clock();
    End = clock();
    float elapsedSeconds = ((float)(End - Start)) / CLOCKS_PER_SEC;
    printf(u8"Finished downloading book %s on Global server(chapter %i to %i) in %f (seconds\n", cnBook.Name, startRange, endRange, elapsedSeconds);
}

// Gets range from input
void getRange(unsigned int* Start, unsigned int* End, unsigned int chapterCount)
{
    while(true)
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
    printf("Get more help or report issues at https://github.com/JeComtempleDuCodeSource/HMPD\n");
    unsigned int serverInt = 0;
    printf("Type 1 to download from global server.And Type 2 to download from CN server: ");
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
        unsigned int chapterCount = getChapterCountGlobal(bookInfo[bookIndex]);
        getRange(&Start, &End, chapterCount);
        downloadGlobalBook(bookInfo[bookIndex], Start, End);
        char* folderName = malloc(8 + 4 + 1 + 1);
        sprintf(folderName, "./GBBook%i/", bookInfo[bookIndex].ID);
        printf("Total diskspace taken by \"./GBBook%i\": %llu (bytes)\n", bookInfo[bookIndex].ID, getFolderSize(folderName));
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
        unsigned int chapterCount = getChapterCountChina(bookInfo[bookIndex]);
        getRange(&Start, &End, chapterCount);
        downloadChinaBook(bookInfo[bookIndex], Start, End);
        char* folderName = malloc(8 + 4 + 1 + 1);
        sprintf(folderName, "./CNBook%i/", bookInfo[bookIndex].ID);
        printf("Total diskspace taken by \"./CNBook%i\": %llu (bytes)\n", bookInfo[bookIndex].ID, getFolderSize(folderName));
        free(bookInfo);
    }
    else 
        printf("Not a valid choice\n");

    printf("Press any key to exit...");
    fflush(stdin);   
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
        unsigned int chapterCount = getChapterCountGlobal(bookInfo[bookIndex]);
        if ((startRange <= endRange) && (endRange <= (chapterCount - 1) && endRange >= 1) && (startRange >= 1 || startRange <= (chapterCount - 1)))
            downloadGlobalBook(bookInfo[bookIndex], startRange, endRange);
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
        unsigned int chapterCount = getChapterCountChina(bookInfo[bookIndex]);
        if ((startRange <= endRange) && (endRange <= (chapterCount - 1) && endRange >= 1) && (startRange >= 1 || startRange <= (chapterCount - 1)))
            downloadChinaBook(bookInfo[bookIndex], startRange, endRange);
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
    printf("Press any key to exit...");
    fflush(stdin);   
    getchar();
}