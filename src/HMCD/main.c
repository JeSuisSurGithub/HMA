#include <HMCD.h>
#include "HMCDVersion.h"
#include <getopt.h>

// Gets range from input
void get_range(unsigned int* first_chap, unsigned int* last_chap, unsigned int chap_count)
{
    while (true)
    {
        char in_buf[32] = {0};
        printf("The max range is 1-%i\n", (chap_count - 1));
        printf("Please type the first chapter to download: ");
        fgets(in_buf, 32, stdin);
        if (sscanf(in_buf, "%i", first_chap) == EOF)
            continue;

        if (*first_chap < 1 || *first_chap > (chap_count - 1))
            printf("Start range cannot be smaller than 1 or greater than %i\n", chap_count);

        printf("Please type the last chapter to download: ");
        fgets(in_buf, 32, stdin);
        if (sscanf(in_buf, "%i", last_chap) == EOF)
            continue;

        if (*last_chap > (chap_count - 1) || *last_chap < 1)
            printf("End range cannot be greater than %i or smaller than 1\n", chap_count);

        if (*first_chap > *last_chap)
            printf("Start range cannot be greater than end range");
        if ((*first_chap <= *last_chap) && (*last_chap <= (chap_count - 1) && *last_chap >= 1) && (*first_chap >= 1 || *first_chap <= (chap_count - 1)))
            break;
    }
}

void guided_ui()
{
    // Get server
    printf("Welcome to HI3 manga C downloader!(The C version of HMPD)\n\n");
    printf("Get more help or report issues at https://github.com/JeFaitDesSpaghettis/HMCD\n");
    char in_buf[32] = {0};
    unsigned int target_server = 0;
    printf("Type 1 to download from global server\nType 2 to download from CN server\nType the corresponding number: ");
    fgets(in_buf, 32, stdin);
    if (sscanf(in_buf, "%i", &target_server) == EOF)
    {
        fprintf(stderr, "Invalid input\n");
        exit(-1);
    }
    putchar('\n');

    // Show available books and get range from user input
    if (target_server == 1)
    {
        hmcd_book* book_list = hmcd_get_glb_books();
        for (unsigned int index = 0; index < GB_CHAPTER_COUNT; index++)
            printf("Type %i to download the book %i AKA %s\n", index, book_list[index].book_id, book_list[index].book_name);

        int book_index = 0;
        fgets(in_buf, 32, stdin);
        sscanf(in_buf, "%i", &book_index);
        if (sscanf(in_buf, "%i", &book_index) == EOF)
        {
            fprintf(stderr, "Invalid input\n");
            exit(-2);
        }
        if (book_index < 0 || book_index >= GB_CHAPTER_COUNT)
        {
            fprintf(stderr, "Book not in range input\n");
            exit(-3);
        }

        unsigned int first_chap = 0;
        unsigned int last_chap = 0;
        unsigned int chap_count = hmcd_get_chap_cnt(book_list[book_index], GLOBAL);
        if (chap_count == 0)
        {
            fprintf(stderr, "guided_ui():hmcd_get_chap_cnt():1 failed\n");
            exit(-4);
        }
        get_range(&first_chap, &last_chap, chap_count);

        int res = hmcd_dl_book(book_list[book_index], first_chap, last_chap, GLOBAL);
        if (res != 0)
        {
            fprintf(stderr, "guided_ui():hmcd_dl_book():1 failed\n");
            exit(-5);
        }
        char* dir_name = malloc(8 + 1 + 4 + 1 + 1);
        sprintf(dir_name, "%s_%i/", glb_out_dir, book_list[book_index].book_id);
        printf("Total diskspace taken by \"%s%i\": %llu (bytes)\n", glb_out_dir, book_list[book_index].book_id, hmcd_get_dir_size(dir_name));
        free(book_list);
        free(dir_name);
    }
    else if (target_server == 2)
    {
#ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
#endif
        hmcd_book* book_list = hmcd_get_cn_books();
        for (unsigned int index = 0; index < CN_CHAPTER_COUNT; index++)
            printf(u8"Type %i to download the book %i AKA %s\n", index, book_list[index].book_id, book_list[index].book_name);

        printf("Names may haven't been shown correctly use the Windows Terminal instead\n");

        int book_index = 0;
        fgets(in_buf, 32, stdin);
        sscanf(in_buf, "%i", &book_index);
        if (sscanf(in_buf, "%i", &book_index) == EOF)
        {
            fprintf(stderr, "Invalid input\n");
            exit(-6);
        }
        if (book_index < 0 || book_index >= CN_CHAPTER_COUNT)
        {
            fprintf(stderr, "Book not in range input\n");
            exit(-7);
        }
        unsigned int first_chap = 0;
        unsigned int last_chap = 0;
        unsigned int chap_count = hmcd_get_chap_cnt(book_list[book_index], CHINA);
        if (chap_count == 0)
        {
            fprintf(stderr, "guided_ui():hmcd_get_chap_cnt():2 failed\n");
            exit(-8);
        }
        get_range(&first_chap, &last_chap, chap_count);

        int res = hmcd_dl_book(book_list[book_index], first_chap, last_chap, CHINA);
        if (res != 0)
        {
            fprintf(stderr, "guided_ui():hmcd_dl_book():2 failed\n");
            exit(-9);
        }
        char* dir_name = malloc(8 + 1 + 4 + 1 + 1);
        sprintf(dir_name, "%s_%i/", cn_out_dir, book_list[book_index].book_id);
        printf("Total diskspace taken by \"%s%i\": %llu (bytes)\n", cn_out_dir, book_list[book_index].book_id, hmcd_get_dir_size(dir_name));
        free(book_list);
        free(dir_name);
    }
    else
        printf("Not a valid choice\n");

    printf("Press any key to exit...\n");
    getchar();
}

void cmd_args_ui(unsigned int server_id, unsigned int book_id, unsigned int first_chap, unsigned int last_chap)
{
    if (server_id == 1)
    {
        printf("Target server: GLOBAL\n");
        hmcd_book* book_list = hmcd_get_glb_books();
        bool book_id_is_valid = false;
        unsigned int book_index = 0;
        for (book_index = 0; book_index < GB_CHAPTER_COUNT; book_index++)
        {
            if (book_id == book_list[book_index].book_id)
            {
                book_id_is_valid = true;
                printf("Target book: %s\n", book_list[book_index].book_name);
                break;
            }
        }
        if (book_id_is_valid == false)
        {
            fprintf(stderr, "Invalid book ID\n");
            exit(-1);
        }
        unsigned int chap_count = hmcd_get_chap_cnt(book_list[book_index], GLOBAL);
        if (chap_count == 0)
        {
            fprintf(stderr, "cmd_args_ui():hmcd_get_chap_cnt():1 failed\n");
            exit(-2);
        }
        if ((first_chap <= last_chap) && (last_chap <= (chap_count - 1)
            && last_chap >= 1) && (first_chap >= 1 || first_chap <= (chap_count - 1)))
        {
            printf("Range: %i-%i\n", first_chap, last_chap);
            int res = hmcd_dl_book(book_list[book_index], first_chap, last_chap, GLOBAL);
            if (res != 0)
            {
                fprintf(stderr, "cmd_args_ui():hmcd_dl_book():1 failed\n");
                exit(-3);
            }
        }
        else
        {
            printf("Invalid range\n");
        }
        free(book_list);
    }
    else if (server_id == 2)
    {
#ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
#endif
        printf("Target server: CHINA\n");
        hmcd_book* book_list = hmcd_get_cn_books();
        bool book_id_is_valid = false;
        unsigned int book_index = 0;
        for (book_index = 0; book_index < CN_CHAPTER_COUNT; book_index++)
        {
            if (book_id == book_list[book_index].book_id)
            {
                book_id_is_valid = true;
                printf(u8"Target book: %s\n", book_list[book_index].book_name);
                break;
            }
        }
        if (book_id_is_valid == false)
        {
            fprintf(stderr, "Invalid book ID");
            exit(-4);
        }
        unsigned int chap_count = hmcd_get_chap_cnt(book_list[book_index], CHINA);
        if (chap_count == 0)
        {
            fprintf(stderr, "cmd_args_ui():hmcd_get_chap_cnt():2 failed\n");
            exit(-5);
        }
        if ((first_chap <= last_chap) && (last_chap <= (chap_count - 1) && last_chap >= 1) && (first_chap >= 1 || first_chap <= (chap_count - 1)))
        {
            printf("Range: %i-%i\n", first_chap, last_chap);
            int res = hmcd_dl_book(book_list[book_index], first_chap, last_chap, CHINA);
            if (res != 0)
            {
                fprintf(stderr, "cmd_args_ui():hmcd_dl_book():2 failed\n");
                exit(-6);
            }
        }
        else
        {
            fprintf(stderr, "Invalid range\n");
            exit(-7);
        }
        free(book_list);
    }
    else
    {
        printf("First value is invalid, GLOBAL = 1, CN = 2\n");
        exit(-8);
    }
    printf("Press any key to exit...\n");
    getchar();
}

int main(int argc, char* argv[])
{
    curl_global_init(CURL_GLOBAL_ALL);
    printf("HMCDCore Version: %i.%i.%i\n", HMCDCore_VERSION_MAJOR, HMCDCore_VERSION_MINOR, HMCDCore_VERSION_PATCH);
    printf("HMCD Version: %i.%i.%i\n", HMCD_VERSION_MAJOR, HMCD_VERSION_MINOR, HMCD_VERSION_PATCH);
    printf("Be sure that you have an internet connection!\n");
    if (argc == 1)
    {
        guided_ui();
        curl_global_cleanup();
        exit(0);
    }
    else
    {
        int help_flag = 0;
        struct option longopts[] =
        {
            { "help", no_argument, &help_flag, 1 },
            { "server", required_argument, NULL, 's' },
            { "book", required_argument, NULL, 'b' },
            { "first", required_argument, NULL, 'f' },
            { "last", required_argument, NULL, 'l' },
            { 0 }
        };
        int opt;
        unsigned int server_id = 0;
        unsigned int book_id = 0;
        unsigned int first_chap = 0;
        unsigned int last_chap = 0;
        while((opt = getopt_long(argc, argv, "hs:b:f:l:", longopts, 0)) != -1)
        {
            switch(opt)
            {
                case 'h':
                    help_flag = 1;
                    break;
                case 's':
                    server_id = atoi(optarg);
                    break;
                case 'b':
                    book_id = atoi(optarg);
                    break;
                case 'f':
                    first_chap = atoi(optarg);
                    break;
                case 'l':
                    last_chap = atoi(optarg);
                    break;
                case ':':
                    printf("No value given\n");
                break;
                case '?':
                    curl_global_cleanup();
                    exit(1);
                    break;
                default:
                    break;
                }
        }
        if (help_flag)
        {
            printf("Usage : %s [OPTIONS]\n", argv[0]);
            printf("\t NO ARGS, launches guided mode\n");
            printf("\t -h, --help\t Prints out this help\n");
            printf("\t -s, --server[int]\t 1 = GLOBAL, 2 = MAINLAND CHINA\n");
            printf("\t -b, --book[int]\t The 4-digit integer on the HI3 COMIC Official site when you read a specific book i.e: http://....com/book/[BookID]\n");
            printf("\t -f, --first[int]\t First chapter to download\n");
            printf("\t -l, --last[int]\t Last chapter to download\n");
            printf("This program uses libcURL and Mozilla's certificate store (cacert.pem), see https://curl.se/docs/copyright.html and https://curl.se/docs/caextract.html\n");
            printf("Get more help or report issues at https://github.com/JeFaitDesSpaghettis/HMCD\n");
            curl_global_cleanup();
            exit(1);
        }
        cmd_args_ui(server_id, book_id, first_chap, last_chap);
    }
    curl_global_cleanup();
    exit(0);
}
