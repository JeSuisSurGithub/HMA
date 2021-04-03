#include "HMCD.h"
#include "HMCDVersion.h"

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

int main(int argc, char* argv[])
{
    curl_global_init(CURL_GLOBAL_ALL);
    printf("HMCD Version: %i.%i.%i\n", HMCD_VERSION_MAJOR, HMCD_VERSION_MINOR, HMCD_VERSION_PATCH);
    if (argc == 5)
    {
        printf("Ensure that you have internet connection!\n");
        commandLineInterface(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), atoi(argv[4]));
    }
    else if (argc == 1)
    {
        printf("Ensure that you have internet connection!\n");
        guidedInterface();
    }
    else 
    {
        printf("Usage : %s [SERVER_ID] [BOOK_ID] [START_CHAPTER] [END_CHAPTER]\n", argv[0]);
        printf("\t SERVER_ID: 1 = GLOBAL, 2 = MAINLAND CHINA\n");
        printf("\t BOOK_ID: The 4-digit integer on the HI3 COMIC Official site when you read a specific book i.e: http://....com/book/[BookID]\n");
        printf("\t START_CHAPTER: The chapter from where you want to start download, it includes it\n");
        printf("\t END_CHAPTER: The chapter from where you want to finish download, it includes it\n");
        printf("This program uses libcURL and Mozilla's certificate store (cacert.pem), see https://curl.se/docs/copyright.html and https://curl.se/docs/caextract.html\n");
        printf("Get more help or report issues at https://github.com/JeFaitDesSpaghettis/HMCD\n");
    }
    curl_global_cleanup();
}
