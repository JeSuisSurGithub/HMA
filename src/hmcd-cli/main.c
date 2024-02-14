/**
 * Honkai Manhua C Downloader (HMCD)
 * Copyright (C) 2022 JeFaisDesSpaghettis
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "hmcd-cli_config.h"

#include <hmcd/hmcd.h>

#include <getopt.h>
#include <stdlib.h>

typedef enum CLI_ERROR_CODE_
{
    SUCCESS = EXIT_SUCCESS,
    WRONG_DL_MODE = -1,
    WRONG_SERVER = -2,
    WRONG_BOOK = -3,
    GET_CHAP_CNT_FAIL = -4,
    DL_BOOK_FAIL = -5,
    INIT_FAIL = -6
}CLI_ERROR_CODE;

typedef enum MENU_JUMP_TABLE_
{
    QUIT = 0,
    SELECT_DOWNLOAD_MODE = 1,
    SELECT_SERVER = 2,
    SELECT_BOOK = 3,
    SELECT_RANGE = 4,
    DOWNLOAD = 5,
    SELECT_JUMP = 6
}MENU_JUMP_TABLE;

void print_help(const char* argv_0)
{
    printf("Usage : %s [OPTIONS]\n", argv_0);
    printf("\t NO ARGS, launches guided mode\n");
    printf("\t-h, --help\t\t Prints out this help\n");
    printf("\t-v, --version\t\t Prints version\n");
    printf("\t-s, --server[int]\t 1 = MAINLAND CHINA, 2 = GLOBAL\n");
    printf("\t-b, --book[int]\t\t The 4-digit integer on the HI3 COMIC Official site when you read a specific book i.e: http://....com/book/[BookID]\n");
    printf("\t-f, --first[int]\t First chapter to download, optional\n");
    printf("\t-l, --last[int]\t\t Last chapter to download, optional\n");
    printf("\t-n, --quiet\t\t No output messages\n");
    printf("\t-o, --onedirectory\t All pages in one directory\n");
    printf("Get more help or report issues at https://github.com/JeFaisDesSpaghettis/HMCD\n");
}

void print_version()
{
    printf("HMCD: v%i.%i.%i\n", HMCD_VERSION_MAJOR, HMCD_VERSION_MINOR, HMCD_VERSION_PATCH);
    printf("HMCD-CLI: v%i.%i.%i\n", HMCD_CLI_VERSION_MAJOR, HMCD_CLI_VERSION_MINOR, HMCD_CLI_VERSION_PATCH);
    printf("This program is licensed under GPL-3.0, see gpl-3.0.md\n");
    printf("This program uses libcURL and Mozilla's certificate store (cacert.pem), see https://curl.se/docs/copyright.html and https://curl.se/docs/caextract.html\n");
}

int phone_style_ui()
{
    char input_buf[32] = {0};
    int exit_status = 0;
    bool quit = false;

    unsigned int one_directory = false;
    HMCD_SRV_ID server_id = HMCD_NONE;
    unsigned int book_index = 0;
    unsigned int first_chap = 0;
    unsigned int last_chap = 0;
    unsigned int chap_count = 0;
    hmcd_ctx* ctx = NULL;
    if (hmcd_init_ctx(
        &ctx,
        true,
        "hmcdoutput",
        NULL) != HMCD_SUCCESS)
    {
        fprintf(stderr, "hmcd_init_ctx() failed\n");
        return INIT_FAIL;
    }

    printf("Welcome to HMCD's menu\n");
    printf("Get more help or report issues at https://github.com/JeFaisDesSpaghettis/HMCD\n");

    MENU_JUMP_TABLE jump_index = SELECT_DOWNLOAD_MODE;
    while (!quit)
    {
        switch (jump_index)
        {
            case QUIT:
                printf("Quitting...");
                quit = true;
                break;
            case SELECT_DOWNLOAD_MODE:
                printf("Type 1 to download all pages sorted into directories\n"
                        "Type 2 to download all pages in one single directory\n"
                        "Type the corresponding number: ");
                fgets(input_buf, 32, stdin);
                sscanf(input_buf, "%u", &one_directory);

                if (one_directory == 1) { one_directory = false; }
                else if (one_directory == 2) { one_directory = true; }
                else
                {
                    fprintf(stderr, "Invalid response\n");
                    exit_status = WRONG_DL_MODE;
                    quit = true;
                    break;
                }
                /* NO BREAK */
            case SELECT_SERVER:
                // Get server
                printf( "Type 1 to download from china server\n"
                        "Type 2 to download from global server\n"
                        "Type the corresponding number: ");
                fgets(input_buf, 32, stdin);
                sscanf(input_buf, "%u", &server_id);

                if (server_id == HMCD_CHINA) { ctx->server = &HMCD_CN_SERVER; }
                else if (server_id == HMCD_GLOBAL) { ctx->server = &HMCD_GLB_SERVER; }
                else
                {
                    fprintf(stderr, "Invalid server\n");
                    exit_status = WRONG_SERVER;
                    quit = true;
                    break;
                }
                /* NO BREAK */
            case SELECT_BOOK:
                // Show available books and get range from user input
                for (size_t index = 0; index < ctx->server->book_count; index++)
                    printf("Type %zu to download %i (%s)\n",
                        index,
                        ctx->server->books[index].book_id,
                        ctx->server->books[index].book_name);

                fgets(input_buf, 32, stdin);
                sscanf(input_buf, "%u", &book_index);
                if (book_index >= ctx->server->book_count)
                {
                    fprintf(stderr, "Invalid book\n");
                    exit_status = WRONG_BOOK;
                    quit = true;
                    break;
                }
                /* NO BREAK */
            case SELECT_RANGE:
                first_chap = 0;
                last_chap = 0;
                if (hmcd_get_chap_cnt(ctx, &chap_count, book_index) != HMCD_SUCCESS)
                {
                    fprintf(stderr, "hmcd_get_chap_cnt() failed\n");
                    exit_status = GET_CHAP_CNT_FAIL;
                    quit = true;
                    break;
                }
                while (true)
                {
                    printf("Chapter 1 to %i are available\n", (chap_count - 1));
                    printf("Please type the first chapter to download (0 is same as 1): ");
                    fgets(input_buf, 32, stdin);
                    sscanf(input_buf, "%u", &first_chap);
                    if (first_chap == 0)
                        first_chap = 1;

                    if (first_chap > (chap_count - 1))
                        printf("Start range cannot be greater or equal to %u\n", chap_count);

                    printf("Please type the last chapter to download (0 is all the way to last chapter): ");
                    fgets(input_buf, 32, stdin);
                    sscanf(input_buf, "%u", &last_chap);
                    if (last_chap == 0)
                        last_chap = (chap_count - 1);

                    if (last_chap > (chap_count - 1))
                        printf("End range cannot be greater or equal to %u\n", chap_count);

                    if (first_chap > last_chap)
                        printf("Start range cannot be greater than end range\n");

                    if ((first_chap <= last_chap) &&
                        (last_chap <= (chap_count - 1)) && (first_chap <= (chap_count - 1)))
                        break;
                }

                /* NO BREAK */
            case DOWNLOAD:
                if (hmcd_dl_book(ctx, book_index, first_chap, last_chap, (bool)one_directory) != HMCD_SUCCESS)
                {
                    fprintf(stderr, "hmcd_dl_book() failed\n");
                    exit_status = DL_BOOK_FAIL;
                    quit = true;
                    break;
                }
                /* NO BREAK */
            case SELECT_JUMP:
                printf( "Type 0 to quit\n"
                        "Type 1 to the download mode selection\n"
                        "Type 2 to the server selection\n"
                        "Type 3 to the book selection\n"
                        "Type 4 to the range selection\n"
                        "Type 5 to download again\n"
                        "Type the corresponding number: ");
                fgets(input_buf, 32, stdin);
                sscanf(input_buf, "%u", &jump_index);
                if (jump_index > DOWNLOAD)
                    jump_index = SELECT_JUMP;
                break;
        }
    }
    hmcd_free_ctx(ctx);
    return exit_status;
}

int cmd_args_ui(
    HMCD_SRV_ID server_id,
    unsigned int book_id,
    unsigned int first_chap,
    unsigned int last_chap,
    bool one_directory,
    bool enable_logs)
{
    const hmcd_srv* target_server = NULL;

    // Check server
    if (server_id == HMCD_CHINA) target_server = &HMCD_CN_SERVER;
    else if (server_id == HMCD_GLOBAL) target_server = &HMCD_GLB_SERVER;
    else
    {
        fprintf(stderr, "Invalid server, CHINA = 1, GLOBAL = 2\n");
        return WRONG_SERVER;
    }

    hmcd_ctx* ctx = NULL;
    if (hmcd_init_ctx(
        &ctx,
        enable_logs,
        "hmcdoutput",
        target_server) != HMCD_SUCCESS)
    {
        fprintf(stderr, "hmcd_init_ctx() failed\n");
        return INIT_FAIL;
    }

    // Check book
    bool book_id_is_valid = false;
    unsigned int book_index;
    for (book_index = 0; book_index < ctx->server->book_count; book_index++)
    {
        if (book_id == ctx->server->books[book_index].book_id)
        {
            book_id_is_valid = true;
            break;
        }
    }
    if (book_id_is_valid == false)
    {
        hmcd_free_ctx(ctx);
        fprintf(stderr, "Invalid book ID\n");
        return WRONG_BOOK;
    }

    // Check chapters
    unsigned int chap_count;
    if (hmcd_get_chap_cnt(ctx, &chap_count, book_index) != HMCD_SUCCESS)
    {
        hmcd_free_ctx(ctx);
        fprintf(stderr, "hmcd_get_chap_cnt() failed\n");
        return GET_CHAP_CNT_FAIL;
    }

    // Special chapter counts
    if (first_chap == 0)
        first_chap = 1;
    if (last_chap == 0)
        last_chap = (chap_count - 1);

    // Download
    if ((first_chap <= last_chap) && (last_chap <= (chap_count - 1)
        && last_chap >= 1) && (first_chap >= 1 || first_chap <= (chap_count - 1)))
    {
        if (hmcd_dl_book(ctx, book_index, first_chap, last_chap, one_directory) != HMCD_SUCCESS)
        {
            hmcd_free_ctx(ctx);
            fprintf(stderr, "hmcd_dl_book() failed\n");
            return DL_BOOK_FAIL;
        }
    }
    else { printf("Invalid range\n"); }
    hmcd_free_ctx(ctx);
    return EXIT_SUCCESS;
}

int main(int argc, char* argv[])
{
    curl_global_init(CURL_GLOBAL_ALL);
    setbuf(stdout, NULL);
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
        unsigned int server_id = 0;
        unsigned int book_id = 0;
        unsigned int first_chap = 0;
        unsigned int last_chap = 0;
        bool enable_logs = true;
        bool one_directory = false;
        struct option longopts[] =
        {
            { "help", no_argument, &help_flag, 1 },
            { "version", no_argument, &v_flag, 1 },
            { "server", required_argument, NULL, 's' },
            { "book", required_argument, NULL, 'b' },
            { "first", optional_argument, NULL, 'f' },
            { "last", optional_argument, NULL, 'l' },
            { "quiet", no_argument, NULL, 'n' },
            { "onedirectory", no_argument, NULL, 'o' },
            { 0 }
        };
        int opt;
        while((opt = getopt_long(argc, argv, "hvs:b:f:l:no", longopts, 0)) != -1)
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
                    enable_logs = false;
                    break;
                case 'o':
                    one_directory = true;
                    break;
                case ':':
                    printf("No argument given for option\n");
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
            print_help(argv[0]);
            curl_global_cleanup();
            exit(EXIT_SUCCESS);
        }
        if (v_flag)
        {
            print_version();
            curl_global_cleanup();
            exit(EXIT_SUCCESS);
        }
        int exit_status = cmd_args_ui(server_id, book_id, first_chap, last_chap, one_directory, enable_logs);
        curl_global_cleanup();
        exit(exit_status);
    }
}
