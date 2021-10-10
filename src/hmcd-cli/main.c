#include "hmcdcore_version.h"
#include "hmcd-cli_version.h"

#include <hmcdcore/hmcd.h>
#include <getopt.h>

#define WRONG_DL_MODE -1
#define WRONG_SERVER -2
#define WRONG_BOOK -3
#define GET_CHAP_CNT_FAIL -4
#define DL_BOOK_FAIL -5

void phone_style_ui()
{
    char in_buf[32] = {0};
    HMCD_SERVER_ID server_id = HMCD_NONE;
    const HmcdBook* book_list = NULL;
    const HmcdServer* target_server = NULL;
    unsigned int book_index = 0;
    unsigned int one_big_dir = false;
    unsigned int first_chap = 0;
    unsigned int last_chap = 0;
    unsigned int chap_count = 0;
    int dl_book_res;
    bool quit = false;
    printf("Welcome to HI3 manga C downloader!\n");
    printf("Get more help or report issues at https://github.com/JeFaitDesSpaghettis/HMCD\n");

    /**
     * jump_index table
     * 0: Quit
     * 1: Select download mode
     * 2: Get server
     * 3: Select book
     * 4: Select range
     * 5: Download
     * 6: Select next thing to do
    */
    unsigned int jump_index = 1;
    while (!quit)
    {
        switch (jump_index)
        {
            case 0:
                quit = true;
                printf("Quitting...");
                break;
            case 1:
                printf( "Type 1 to download all pages in different directories\n"
                        "Type 2 to download all pages in one directory\n"
                        "Type the corresponding number: ");
                fgets(in_buf, 32, stdin);
                sscanf(in_buf, "%u", &one_big_dir);

                if (one_big_dir == 1) { one_big_dir = false; }
                else if (one_big_dir == 2) { one_big_dir = true; }
                else { fprintf(stderr, "Invalid response\n"); exit(WRONG_DL_MODE); }
                /* NO BREAK */
            case 2:
                // Get server
                printf( "Type 1 to download from CN server\n"
                        "Type 2 to download from global server\n"
                        "Type the corresponding number: ");

                fgets(in_buf, 32, stdin);
                sscanf(in_buf, "%u", (int*)&server_id);

                if (server_id == HMCD_CHINA) { target_server = &HMCD_CN_SERVER; }
                else if (server_id == HMCD_GLOBAL) { target_server = &HMCD_GLB_SERVER; }
                else { fprintf(stderr, "Invalid server\n"); exit(WRONG_SERVER); }
                /* NO BREAK */
            case 3:
                // Show available books and get range from user input
                book_list = &target_server->books;
                for (size_t index = 0; index < target_server->book_count; index++)
                    printf("Type %llu to download %i (%s)\n", index, book_list[index].book_id, book_list[index].book_name);

                fgets(in_buf, 32, stdin);
                sscanf(in_buf, "%u", &book_index);
                if (book_index >= target_server->book_count)
                {
                    fprintf(stderr, "Invalid book\n");
                    exit(WRONG_BOOK);
                }
                /* NO BREAK */
            case 4:
                first_chap = 0;
                last_chap = 0;
                chap_count = hmcd_get_chap_cnt(target_server, book_index);
                if (chap_count == 0)
                {
                    fprintf(stderr, "hmcd_get_chap_cnt() failed\n");
                    exit(GET_CHAP_CNT_FAIL);
                }
                while (true)
                {
                    printf("Chapter 1 to %i are available\n", (chap_count - 1));
                    printf("Please type the first chapter to download (0 is same as 1): ");
                    fgets(in_buf, 32, stdin);
                    sscanf(in_buf, "%u", &first_chap);

                    if (first_chap > (chap_count - 1))
                        printf("Start range cannot be greater or equal to %u\n", chap_count);

                    printf("Please type the last chapter to download (0 is all the way to last chapter): ");
                    fgets(in_buf, 32, stdin);
                    sscanf(in_buf, "%u", &last_chap);

                    if (last_chap > (chap_count - 1))
                        printf("End range cannot be greater or equal to %u\n", chap_count);

                    if (first_chap > last_chap)
                        printf("Start range cannot be greater than end range\n");

                    if ((first_chap <= last_chap) &&
                        (last_chap <= (chap_count - 1)) && (first_chap <= (chap_count - 1)))
                        break;
                }
                if (first_chap == 0)
                    first_chap = 1;
                if (last_chap == 0)
                    last_chap = (chap_count - 1);
                /* NO BREAK */
            case 5:
                dl_book_res = hmcd_dl_book(target_server, book_index, first_chap, last_chap, (bool)one_big_dir);
                if (dl_book_res != 0)
                {
                    fprintf(stderr, "hmcd_dl_book() failed\n");
                    exit(DL_BOOK_FAIL);
                }
                char* dir_name = malloc(strlen(target_server->out_dir) + 1 + 4 + 1 + 1);
                sprintf(dir_name, "%s_%i/", target_server->out_dir, book_list[book_index].book_id);
                printf(
                    "Total diskspace taken by \"%s%i\": %llu (bytes)\n",
                    target_server->out_dir,
                    book_list[book_index].book_id,
                    hmcd_get_dir_size(dir_name));
                free(dir_name);
                /* NO BREAK */
            case 6:
                printf( "Type 0 to quit\n"
                        "Type 1 to the download mode selection\n"
                        "Type 2 to the server selection\n"
                        "Type 3 to the book selection\n"
                        "Type 4 to the range selection\n"
                        "Type 5 to download again\n"
                        "Type the corresponding number: ");
                fgets(in_buf, 32, stdin);
                sscanf(in_buf, "%u", &jump_index);
                break;
        }
    }
}

void cmd_args_ui(HMCD_SERVER_ID server_id, unsigned int book_id, unsigned int first_chap, unsigned int last_chap, bool one_big_dir)
{
    const HmcdServer* target_server = NULL;
    const HmcdBook* book_list = NULL;
    unsigned int book_index = 0;

    // Check download mode
    printf("Downloading %s\n", (one_big_dir) ? "everything in one directory" : "pages to separate directories by chapter");

    // Check server
    if (server_id == HMCD_CHINA) target_server = &HMCD_CN_SERVER;
    else if (server_id == HMCD_GLOBAL) target_server = &HMCD_GLB_SERVER;
    else { fprintf(stderr, "Invalid server, CN = 1, GLOBAL = 2\n"); exit(-1); }
    printf("Target server: %s\n", hmcd_get_server_name(server_id));

    // Check book
    bool book_id_is_valid = false;
    book_list = &target_server->books;
    for (book_index = 0; book_index < target_server->book_count; book_index++)
    {
        if (book_id == book_list[book_index].book_id)
        {
            book_id_is_valid = true;
            break;
        }
    }
    if (book_id_is_valid == false)
    {
        fprintf(stderr, "Invalid book ID\n");
        exit(WRONG_BOOK);
    }
    printf("Target book: %i (%s)\n", book_id, book_list[book_index].book_name);

    // Check chapters
    unsigned int chap_count = hmcd_get_chap_cnt(target_server, book_index);
    if (chap_count == 0)
    {
        fprintf(stderr, "hmcd_get_chap_cnt() failed\n");
        exit(GET_CHAP_CNT_FAIL);
    }

    if (first_chap == 0)
        first_chap = 1;
    if (last_chap == 0)
        last_chap = (chap_count - 1);

    printf("Range: %i-%i\n", first_chap, last_chap);

    // Download
    if ((first_chap <= last_chap) && (last_chap <= (chap_count - 1)
        && last_chap >= 1) && (first_chap >= 1 || first_chap <= (chap_count - 1)))
    {
        int dl_book_res = hmcd_dl_book(target_server, book_index, first_chap, last_chap, one_big_dir);
        if (dl_book_res != 0)
        {
            fprintf(stderr, "hmcd_dl_book() failed\n");
            exit(DL_BOOK_FAIL);
        }
    }
    else { printf("Invalid range\n"); }
}

int main(int argc, char* argv[])
{
    curl_global_init(CURL_GLOBAL_ALL);
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    if (argc == 1)
    {
        phone_style_ui();
        curl_global_cleanup();
        exit(EXIT_SUCCESS);
    }
    else
    {
        int help_flag = 0;
        int v_flag = 0;
        struct option longopts[] =
        {
            { "help", no_argument, &help_flag, 1 },
            { "version", no_argument, &v_flag, 'v' },
            { "server", required_argument, NULL, 's' },
            { "book", required_argument, NULL, 'b' },
            { "first", optional_argument, NULL, 'f' },
            { "last", optional_argument, NULL, 'l' },
            { "lessverbose", no_argument, NULL, 'n' },
            { "noseparation", no_argument, NULL, 'r' },
            { 0 }
        };
        int opt;
        unsigned int server_id = 0;
        unsigned int book_id = 0;
        unsigned int first_chap = 0;
        unsigned int last_chap = 0;
        bool one_big_dir = false;
        while((opt = getopt_long(argc, argv, "hvs:b:f:l:nr", longopts, 0)) != -1)
        {
            switch(opt)
            {
                case 'h':
                    help_flag = 1;
                    break;
                case 'v':
                    v_flag = 1;
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
                case 'n':
                    hmcd_enable_logs(false);
                    break;
                case 'r':
                    one_big_dir = true;
                    break;
                case ':':
                    printf("No value given\n");
                    break;
                case '?':
                    curl_global_cleanup();
                    exit(EXIT_FAILURE);
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
            printf("\t -s, --server[int]\t 1 = MAINLAND CHINA, 2 = GLOBAL\n");
            printf("\t -b, --book[int]\t The 4-digit integer on the HI3 COMIC Official site when you read a specific book i.e: http://....com/book/[BookID]\n");
            printf("\t -f, --first[int]\t First chapter to download, optional\n");
            printf("\t -l, --last[int]\t Last chapter to download, optional\n");
            printf("\t -n, --lessverbose\t Silent libhmcd\n");
            printf("\t -r, --noseparation\t All pages in one big directory\n");
            printf("Get more help or report issues at https://github.com/JeFaitDesSpaghettis/HMCD\n");
            curl_global_cleanup();
            exit(EXIT_SUCCESS);
        }
        if (v_flag)
        {
            printf("HMCDCORE: v%i.%i.%i\n", HMCDCORE_VERSION_MAJOR, HMCDCORE_VERSION_MINOR, HMCDCORE_VERSION_PATCH);
            printf("HMCD-CLI: v%i.%i.%i\n", HMCD_CLI_VERSION_MAJOR, HMCD_CLI_VERSION_MINOR, HMCD_CLI_VERSION_PATCH);
            printf("This program is licensed under the Apache 2.0 License, for more information see LICENSE.md\n");
            printf("This program uses libcURL and Mozilla's certificate store (cacert.pem), see https://curl.se/docs/copyright.html and https://curl.se/docs/caextract.html\n");
            curl_global_cleanup();
            exit(EXIT_SUCCESS);
        }
        cmd_args_ui(server_id, book_id, first_chap, last_chap, one_big_dir);
    }
    curl_global_cleanup();
    exit(EXIT_SUCCESS);
}
