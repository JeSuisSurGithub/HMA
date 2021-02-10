#include "HMCD.h"
#include "HMCDVersion.h"

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
        printf("This program uses libcURL and Mozilla's certificate store (cacert.pem), see LIBCURL.COPYING.txt and https://curl.se/docs/caextract.html\n");
        printf("Get more help or report issues at https://github.com/JeComtempleDuCodeSource/HMCD\n");
    }
    curl_global_cleanup();
}
