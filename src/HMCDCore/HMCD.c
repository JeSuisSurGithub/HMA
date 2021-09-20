#include <HMCD.h>

hmcd_book hmcd_construct_book(unsigned int book_id, const char* book_name)
{
    hmcd_book cur_book;
    cur_book.book_id = book_id;
    cur_book.book_name = book_name;
    return cur_book;
}

hmcd_book* hmcd_get_glb_books()
{
    hmcd_book* book_list = malloc(sizeof(hmcd_book) * GB_CHAPTER_COUNT);
    book_list[0]  = hmcd_construct_book(HMCD_GLB_AICHAN_FACTS,    HMCD_GLB_AICHAN_FACTS_NAME   );
    book_list[1]  = hmcd_construct_book(HMCD_GLB_GRATITUDE,       HMCD_GLB_GRATITUDE_NAME      );
    book_list[2]  = hmcd_construct_book(HMCD_GLB_AE_INVASION,     HMCD_GLB_AE_INVASION_NAME    );
    book_list[3]  = hmcd_construct_book(HMCD_GLB_AZURE_WATER,     HMCD_GLB_AZURE_WATER_NAME    );
    book_list[4]  = hmcd_construct_book(HMCD_GLB_2ND_ERUPTION,    HMCD_GLB_2ND_ERUPTION_NAME   );
    book_list[5]  = hmcd_construct_book(HMCD_GLB_GEMINA_TALES,    HMCD_GLB_GEMINA_TALES_NAME   );
    book_list[6]  = hmcd_construct_book(HMCD_GLB_WORLD_SERPENT,   HMCD_GLB_WORLD_SERPENT_NAME  );
    book_list[7]  = hmcd_construct_book(HMCD_GLB_KIANA_PLAYS_HI3, HMCD_GLB_KIANA_PLAYS_HI3_NAME);
    book_list[8]  = hmcd_construct_book(HMCD_GLB_LONDON_HOLIDAY,  HMCD_GLB_LONDON_HOLIDAY_NAME );
    book_list[9]  = hmcd_construct_book(HMCD_GLB_MOON_SHADOW,     HMCD_GLB_MOON_SHADOW_NAME    );
    book_list[10] = hmcd_construct_book(HMCD_GLB_ELAN_PALATINUS,  HMCD_GLB_ELAN_PALATINUS_NAME );
    book_list[11] = hmcd_construct_book(HMCD_GLB_SPRINGFEST,      HMCD_GLB_SPRINGFEST_NAME     );
    book_list[12] = hmcd_construct_book(HMCD_GLB_ELF,             HMCD_GLB_ELF_NAME            );
    book_list[13] = hmcd_construct_book(HMCD_GLB_SECOND_KEY,      HMCD_GLB_SECOND_KEY_NAME     );
    book_list[14] = hmcd_construct_book(HMCD_GLB_NAGAZORA,        HMCD_GLB_NAGAZORA_NAME       );
    book_list[15] = hmcd_construct_book(HMCD_GLB_FREYA_HIGH,      HMCD_GLB_FREYA_HIGH_NAME     );
    book_list[16] = hmcd_construct_book(HMCD_GLB_GEMINA_INVASION, HMCD_GLB_GEMINA_INVASION_NAME);
    book_list[17] = hmcd_construct_book(HMCD_GLB_DIVINE_KEY,      HMCD_GLB_DIVINE_KEY_NAME     );
    book_list[18] = hmcd_construct_book(HMCD_GLB_CWV,             HMCD_GLB_CWV_NAME            );
    book_list[19] = hmcd_construct_book(HMCD_GLB_EMPYREAN_BLADE,  HMCD_GLB_EMPYREAN_BLADE_NAME );
    book_list[20] = hmcd_construct_book(HMCD_GLB_ALIEN_SPACE,     HMCD_GLB_ALIEN_SPACE_NAME    );
    book_list[21] = hmcd_construct_book(HMCD_GLB_SPRING_FESTIVAL, HMCD_GLB_SPRING_FESTIVAL_NAME);
    return book_list;
}

hmcd_book* hmcd_get_cn_books()
{
    hmcd_book* book_list = malloc(sizeof(hmcd_book) * CN_CHAPTER_COUNT);
    book_list[0]  = hmcd_construct_book(HMCD_CN_NAGAZORA,        HMCD_CN_NAGAZORA_NAME       );
    book_list[1]  = hmcd_construct_book(HMCD_CN_HI3,             HMCD_CN_HI3_NAME            );
    book_list[2]  = hmcd_construct_book(HMCD_CN_BOHAI,           HMCD_CN_BOHAI_NAME     	 );
    book_list[3]  = hmcd_construct_book(HMCD_CN_SAKURA,          HMCD_CN_SAKURA_NAME         );
    book_list[4]  = hmcd_construct_book(HMCD_CN_AE_INVASION,     HMCD_CN_AE_INVASION_NAME    );
    book_list[5]  = hmcd_construct_book(HMCD_CN_GRATITUDE_ARC,   HMCD_CN_GRATITUDE_ARC_NAME  );
    book_list[6]  = hmcd_construct_book(HMCD_CN_MOON_SHADOW,     HMCD_CN_MOON_SHADOW_NAME    );
    book_list[7]  = hmcd_construct_book(HMCD_CN_ELAN_PALATINUS,  HMCD_CN_ELAN_PALATINUS_NAME );
    book_list[8]  = hmcd_construct_book(HMCD_CN_GOD_KEY,         HMCD_CN_GOD_KEY_NAME        );
    book_list[9]  = hmcd_construct_book(HMCD_CN_KIANA,           HMCD_CN_KIANA_NAME          );
    book_list[10] = hmcd_construct_book(HMCD_CN_2ND_LAWMAN,      HMCD_CN_2ND_LAWMAN_NAME     );
    book_list[11] = hmcd_construct_book(HMCD_CN_CWV,             HMCD_CN_CWV_NAME            );
    book_list[12] = hmcd_construct_book(HMCD_CN_SUMMER_MEM,      HMCD_CN_SUMMER_MEM_NAME     );
    book_list[13] = hmcd_construct_book(HMCD_CN_GEMINA_ORIGINS,  HMCD_CN_GEMINA_ORIGINS_NAME );
    book_list[14] = hmcd_construct_book(HMCD_CN_GEMINA_INVASION, HMCD_CN_GEMINA_INVASION_NAME);
    book_list[15] = hmcd_construct_book(HMCD_CN_WORLD_SERPENT,   HMCD_CN_WORLD_SERPENT_NAME  );
    book_list[16] = hmcd_construct_book(HMCD_CN_LONDON_HOLIDAY,  HMCD_CN_LONDON_HOLIDAY_NAME );
    book_list[17] = hmcd_construct_book(HMCD_CN_SPRINGFEST,      HMCD_CN_SPRINGFEST_NAME     );
    book_list[18] = hmcd_construct_book(HMCD_CN_ELF,             HMCD_CN_ELF_NAME            );
    book_list[19] = hmcd_construct_book(HMCD_CN_SECOND_KEY,      HMCD_CN_SECOND_KEY_NAME     );
    book_list[20] = hmcd_construct_book(HMCD_CN_EMPYREAN_BLADE,  HMCD_CN_EMPYREAN_BLADE_NAME );
    book_list[21] = hmcd_construct_book(HMCD_CN_ALIEN_SPACE,     HMCD_CN_ALIEN_SPACE_NAME    );
    book_list[22] = hmcd_construct_book(HMCD_CN_SPRING_FESTIVAL, HMCD_CN_SPRING_FESTIVAL_NAME);
    return book_list;
}

int hmcd_set_https_cert(CURL* curl_handle, const char* certificate_path)
{
    FILE* cert_file;
    if ((cert_file = fopen(certificate_path, "r")) != NULL)
    {
        printf("Certificate exists!\n");
        curl_easy_setopt(curl_handle, CURLOPT_CAINFO, certificate_path);
        return 0;
    }
    else
    {
        printf("Certificate does not exists downloading from https://curl.se/ca/cacert.pem...\n");
        CURL* cert_dl_handle = curl_easy_init();
        if (cert_dl_handle == NULL)
        {
            fprintf(stderr, "hmcd_set_https_cert():curl_easy_init() failed\n");
            fclose(cert_file);
            return -1;
        }
        if ((cert_file = fopen(certificate_path, "wb+")) == NULL)
        {
            fprintf(stderr, "hmcd_set_https_cert():fopen() failed, with certificate_path = %s\n", certificate_path);
            curl_easy_cleanup(cert_dl_handle);
            return -2;
        }
        long http_code = 0;

        curl_easy_setopt(cert_dl_handle, CURLOPT_URL, "https://curl.se/ca/cacert.pem");
        curl_easy_setopt(cert_dl_handle, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(cert_dl_handle, CURLOPT_NOBODY, 0L);
        curl_easy_setopt(cert_dl_handle, CURLOPT_VERBOSE, 0L);
        curl_easy_setopt(cert_dl_handle, CURLOPT_NOPROGRESS, 1L);
        curl_easy_setopt(cert_dl_handle, CURLOPT_WRITEFUNCTION, fwrite);
        curl_easy_setopt(cert_dl_handle, CURLOPT_WRITEDATA, cert_file);
        curl_easy_perform(cert_dl_handle);

        curl_easy_getinfo(cert_dl_handle, CURLINFO_RESPONSE_CODE, &http_code);
        if (http_code != 200)
        {
            fprintf(stderr, "curl_easy_perform() failed, error: %li\n", http_code);
            fclose(cert_file);
            curl_easy_cleanup(cert_dl_handle);
            return -3;
        }
        curl_easy_setopt(curl_handle, CURLOPT_CAINFO, certificate_path);

        curl_easy_cleanup(cert_dl_handle);
        printf("Certificate downloaded from https://curl.se/ca/cacert.pem...\n");
        fclose(cert_file);
        return 1;
    }
}

unsigned long long int hmcd_get_dir_size(const char* dir_name)
{
    unsigned int file_count = 0;
    char** file_list = dsu_get_all_filepath(dir_name, &file_count);
    if (file_list == NULL)
    {
        fprintf(stderr, "hmcd_get_dir_size():dsu_get_all_filepath() failed, dir_name = %s\n", dir_name);
        for (unsigned int index = 0; index < file_count; index++)
            free(file_list[index]);
        free(file_list);
    }
    unsigned long long int total_dir_sz = 0;
    for (unsigned int index = 0; index < file_count; index++)
        total_dir_sz += dsu_get_filesize(file_list[index]);

    for (unsigned int index = 0; index < file_count; index++)
        free(file_list[index]);
    free(file_list);
    return total_dir_sz;
}

unsigned int hmcd_get_chap_cnt(hmcd_book target_book, DL_SERVER target_server)
{
    const char* target_base_url = (target_server == CHINA) ? CN_BASE_URL : GLB_BASE_URL;
    printf("Getting chapter count...\n");
    CURL* check_handle = curl_easy_init();
    if (check_handle == NULL)
    {
        fprintf(stderr, "hmcd_get_chap_cnt():curl_easy_init() failed\n");
        curl_easy_cleanup(check_handle);
        return 0;
    }

    int res = hmcd_set_https_cert(check_handle, "./cacert.pem");
    if (res < 0)
    {
        fprintf(stderr, "hmcd_get_chap_cnt():hmcd_set_https_cert() failed\n");
        curl_easy_cleanup(check_handle);
        return 0;
    }

    curl_easy_setopt(check_handle, CURLOPT_NOBODY, 1L);
    unsigned int chap_count = 0;
    for (chap_count = 1; ; chap_count++)
    {
        long http_code = 0;
        char* first_page_url = (chap_count >= 10) ?
            malloc(strlen(target_base_url) + 1 + 4 + 1 + 2 + 9 + 1)
            : malloc(strlen(target_base_url) + 1 + 4 + 1 + 1 + 9 + 1);
        sprintf(first_page_url, "%s/%i/%i/0001.jpg", target_base_url, target_book.book_id, chap_count);
        curl_easy_setopt(check_handle, CURLOPT_URL, first_page_url);
        curl_easy_perform(check_handle);

        curl_easy_getinfo(check_handle, CURLINFO_RESPONSE_CODE, &http_code);
        if (http_code == 404 || http_code == 403)
        {
            printf("Chapter %i does not exists\n", chap_count);
            printf("Chapter count is %i\n", (chap_count - 1));
            curl_easy_cleanup(check_handle);
            free(first_page_url);
            return chap_count;
        }
        else if (http_code == 200)
        {
            printf("Chapter %i exists\n", chap_count);
        }
        else
        {
            fprintf(stderr, "hmcd_get_chap_cnt():curl_easy_perform(), unknown error: %li", http_code);
            curl_easy_cleanup(check_handle);
            free(first_page_url);
            return chap_count;
        }
        free(first_page_url);
    }
}

int hmcd_dl_book(hmcd_book target_book, unsigned int first_chap, unsigned int last_chap, DL_SERVER target_server)
{
    const char* target_base_url = (target_server == CHINA) ? CN_BASE_URL : GLB_BASE_URL;
    const char* target_out_dir = (target_server == CHINA) ? CN_OUT_DIR : GLB_OUT_DIR;
    clock_t clock_start = clock();

    char* book_dirname = malloc(strlen(target_out_dir) + 1 + 4 + 1);
    sprintf(book_dirname, "%s_%i", target_out_dir, target_book.book_id);
#ifdef _WIN32
    mkdir(book_dirname);
#else
    mkdir(book_dirname, 0777);
#endif
    // For all chapter in the range
    CURL* check_handle = curl_easy_init();
    if (check_handle == NULL)
    {
        fprintf(stderr, "hmcd_dl_book():curl_easy_init():1 failed\n");
        curl_easy_cleanup(check_handle);
        return -1;
    }
    int res_chk = hmcd_set_https_cert(check_handle, "./cacert.pem");
    if (res_chk < 0)
    {
        fprintf(stderr, "hmcd_dl_book():hmcd_set_https_cert():1 failed\n");
        curl_easy_cleanup(check_handle);
        return -2;
    }

    curl_easy_setopt(check_handle, CURLOPT_NOBODY, 1L);

    CURL* dl_handle = curl_easy_init();
    if (dl_handle == NULL)
    {
        fprintf(stderr, "hmcd_dl_book():curl_easy_init():2 failed\n");
        curl_easy_cleanup(check_handle);
        curl_easy_cleanup(dl_handle);
        return -3;
    }
    int res_dl = hmcd_set_https_cert(dl_handle, "./cacert.pem");
    if (res_dl < 0)
    {
        fprintf(stderr, "hmcd_dl_book():hmcd_set_https_cert():2 failed\n");
        curl_easy_cleanup(check_handle);
        curl_easy_cleanup(dl_handle);
        return -4;
    }
    curl_easy_setopt(dl_handle, CURLOPT_HEADER, 0L);

    for (unsigned int chap_count = first_chap; chap_count <= last_chap; chap_count++)
    {
        char* chap_url = (chap_count >= 10) ?
            malloc(strlen(target_base_url) + 1 + 4 + 1 + 2 + 1 + 1) :
            malloc(strlen(target_base_url) + 1 + 4 + 1 + 1 + 1 + 1);
        sprintf(chap_url, "%s/%i/%i/", target_base_url, target_book.book_id, chap_count);

        char* chap_dirname = malloc(strlen(book_dirname) + strlen("/Chapter") + 2 + 1);
        sprintf(chap_dirname, "%s/Chapter%02i", book_dirname, chap_count);
#ifdef _WIN32
        mkdir(chap_dirname);
#else
        mkdir(chap_dirname, 0777);
#endif

        printf("Downloading chapter %i\n", chap_count);
        unsigned int page_count = 0;
        for (page_count = 1; ; page_count++)
        {
            long http_code = 0;
            char* page_url = malloc(strlen(chap_url) + 8 + 1);
            sprintf(page_url, "%s00%02i.jpg", chap_url, page_count);

            char* page_name = malloc(strlen(target_out_dir) + 4 + 8 + 2 + 1 + 2 + 2 + 4 + 1);;
            sprintf(page_name, "%s/%02i%02i.jpg", chap_dirname, chap_count, page_count);

            curl_easy_setopt(check_handle, CURLOPT_URL, page_url);
            curl_easy_perform(check_handle);

            curl_easy_getinfo(check_handle, CURLINFO_HTTP_CODE, &http_code);
            if (http_code == 200)
            {
                curl_easy_setopt(dl_handle, CURLOPT_URL, page_url);
                curl_easy_setopt(dl_handle, CURLOPT_VERBOSE, 0L);
	            curl_easy_setopt(dl_handle, CURLOPT_NOPROGRESS, 1L);
                curl_easy_setopt(dl_handle, CURLOPT_WRITEFUNCTION, fwrite);

                FILE* page_file = fopen(page_name, "wb+");
                if (page_file == NULL)
                {
                    fprintf(stderr, "hmcd_dl_book():fopen(), with page_name = %s\n", page_name);
                    free(page_url);
                    free(page_name);
                    free(chap_url);
                    free(chap_dirname);
                    free(book_dirname);
                    curl_easy_cleanup(check_handle);
                    curl_easy_cleanup(dl_handle);
                    return -6;
                }
	            curl_easy_setopt(dl_handle, CURLOPT_WRITEDATA, page_file);
	            curl_easy_perform(dl_handle);
                curl_easy_getinfo(check_handle, CURLINFO_HTTP_CODE, &http_code);
                if (http_code != 200)
                {
                    fprintf(stderr, "hmcd_dl_book():curl_easy_perform():2, unknown error: %li", http_code);
                    free(page_url);
                    free(page_name);
                    free(chap_url);
                    free(chap_dirname);
                    free(book_dirname);
                    curl_easy_cleanup(check_handle);
                    curl_easy_cleanup(dl_handle);
                    return -7;
                }

	            fclose(page_file);
                printf("%s => %s\n", page_url, page_name);
                free(page_url);
                free(page_name);
            }
            // Assume error 404(for GLOBAL server) or 403(for CN server) means end of chapter
            else if (http_code == 404 || http_code == 403)
            {
                printf("Can't download page %i of chapter %i\n", page_count, chap_count);
                free(page_url);
                free(page_name);
                break;
            }
            else
            {
                fprintf(stderr, "hmcd_dl_book():curl_easy_perform():1, unknown error: %li", http_code);
                free(page_url);
                free(page_name);
                free(chap_url);
                free(chap_dirname);
                free(book_dirname);
                curl_easy_cleanup(check_handle);
                curl_easy_cleanup(dl_handle);
                return -5;
            }
        }
        free(chap_url);
        free(chap_dirname);
    }
    free(book_dirname);
    curl_easy_cleanup(check_handle);
    curl_easy_cleanup(dl_handle);

    clock_t clock_end = clock();
    float elapsed_sec = ((float)(clock_end - clock_start)) / CLOCKS_PER_SEC;
    printf("Finished downloading book %s on %s server(chapter %i to %i) in %f (seconds)\n",
        target_book.book_name,
        (target_server == CHINA) ? "china" : "global",
        first_chap,
        last_chap,
        elapsed_sec);
    return 0;
}
