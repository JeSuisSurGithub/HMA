#include <HMCD.h>

hmcd_book hmcd_construct_book(unsigned int book_id, const char* book_name)
{
    hmcd_book cur_book;
    cur_book.book_id = book_id;
    cur_book.book_name = malloc(strlen(book_name) + 1);
    strcpy(cur_book.book_name, book_name);
    return cur_book;
}

hmcd_book* hmcd_get_glb_books()
{
    hmcd_book* book_list = malloc(sizeof(hmcd_book) * GB_CHAPTER_COUNT);
    book_list[0]  = hmcd_construct_book(glb_aichan_facts,    "Ai-Chan Facts"         );
    book_list[1]  = hmcd_construct_book(glb_gratitude,       "Gratitude Arc"         );
    book_list[2]  = hmcd_construct_book(glb_ae_invasion,     "AE Invasion"           );
    book_list[3]  = hmcd_construct_book(glb_azure_water,     "Azure Waters"          );
    book_list[4]  = hmcd_construct_book(glb_2nd_eruption,    "Second Eruption"       );
    book_list[5]  = hmcd_construct_book(glb_gemina_tales,    "Gemina: Tales"         );
    book_list[6]  = hmcd_construct_book(glb_world_serpent,   "World Serpent"         );
    book_list[7]  = hmcd_construct_book(glb_kiana_plays_hi3, "Kiana Plays Honkai"    );
    book_list[8]  = hmcd_construct_book(glb_london_holiday,  "London Holiday"        );
    book_list[9]  = hmcd_construct_book(glb_moon_shadow,     "Moon Shadow"           );
    book_list[10] = hmcd_construct_book(glb_elan_palatinus,  "Elan Palatinus"        );
    book_list[11] = hmcd_construct_book(glb_springfest,      "SpringFest"            );
    book_list[12] = hmcd_construct_book(glb_elf,             "ELF"                   );
    book_list[13] = hmcd_construct_book(glb_second_key,      "Second Key"            );
    book_list[14] = hmcd_construct_book(glb_nagazora,        "Escape From Nagazora"  );
    book_list[15] = hmcd_construct_book(glb_freya_high,      "St. Freya High"        );
    book_list[16] = hmcd_construct_book(glb_gemina_invasion, "Gemina: Invasion"      );
    book_list[17] = hmcd_construct_book(glb_divine_key,      "Divine Key"            );
    book_list[18] = hmcd_construct_book(glb_cwv,             "Cooking With Valkyries");
    book_list[19] = hmcd_construct_book(glb_empyrean_blade,  "Empyrean Blade"        );
    book_list[20] = hmcd_construct_book(glb_alien_space,     "Alien Space"           );
    book_list[21] = hmcd_construct_book(glb_spring_festival, "Spring Festival Trip"  );
    return book_list;
}

hmcd_book* hmcd_get_cn_books()
{
    hmcd_book* book_list = malloc(sizeof(hmcd_book) * CN_CHAPTER_COUNT);
    book_list[0]  = hmcd_construct_book(cn_nagazora,        u8"逃离长空篇");
    book_list[1]  = hmcd_construct_book(cn_hi3,             u8"樱花追忆篇");
    book_list[2]  = hmcd_construct_book(cn_bohai,           u8"绀海篇");
    book_list[3]  = hmcd_construct_book(cn_sakura,          u8"绯樱篇");
    book_list[4]  = hmcd_construct_book(cn_ae_invasion,     u8"逆熵入侵篇");
    book_list[5]  = hmcd_construct_book(cn_gratitude_arc,   u8"恩返篇");
    book_list[6]  = hmcd_construct_book(cn_moon_shadow,     u8"月影篇");
    book_list[7]  = hmcd_construct_book(cn_elan_palatinus,  u8"紫鸢篇");
    book_list[8]  = hmcd_construct_book(cn_god_key,         u8"神之键秘话");
    book_list[9]  = hmcd_construct_book(cn_kiana,           u8"玩崩坏3的琪亚娜");
    book_list[10] = hmcd_construct_book(cn_2nd_lawman,      u8"第二次崩坏");
    book_list[11] = hmcd_construct_book(cn_cwv,             u8"女武神的餐桌");
    book_list[12] = hmcd_construct_book(cn_summer_mem,      u8"夏日回忆-预告篇");
    book_list[13] = hmcd_construct_book(cn_gemina_origins,  u8"双子：起源");
    book_list[14] = hmcd_construct_book(cn_gemina_invasion, u8"双子：入侵");
    book_list[15] = hmcd_construct_book(cn_world_serpent,   u8"蛇之篇");
    book_list[16] = hmcd_construct_book(cn_london_holiday,  u8"雾都假日");
    book_list[17] = hmcd_construct_book(cn_springfest,      u8"年岁");
    book_list[18] = hmcd_construct_book(cn_elf,             u8"武装人偶");
    book_list[19] = hmcd_construct_book(cn_second_key,      u8"传承");
    book_list[20] = hmcd_construct_book(cn_empyrean_blade,  u8"云墨剑心");
    book_list[21] = hmcd_construct_book(cn_alien_space,     u8"异乡");
    book_list[22] = hmcd_construct_book(cn_spring_festival, u8"新春旅行");
    return book_list;
}

void hmcd_set_https_cert(CURL* curl_handle, const char* certificate_path)
{
    FILE* cert_file;
    if ((cert_file = fopen(certificate_path, "r+")) != NULL)
    {
        printf("Certificate exists!\n");
        curl_easy_setopt(curl_handle, CURLOPT_CAINFO, certificate_path);
    }
    else
    {
        printf("Certificate does not exists downloading from https://curl.se/ca/cacert.pem...\n");
        CURL* cert_dl_handle = curl_easy_init();
        curl_easy_setopt(cert_dl_handle, CURLOPT_URL, "https://curl.se/ca/cacert.pem");
        curl_easy_setopt(cert_dl_handle, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(cert_dl_handle, CURLOPT_NOBODY, 0L);
        curl_easy_setopt(cert_dl_handle, CURLOPT_VERBOSE, 0L);
        curl_easy_setopt(cert_dl_handle, CURLOPT_NOPROGRESS, 1L);
        curl_easy_setopt(cert_dl_handle, CURLOPT_WRITEFUNCTION, fwrite);

	    FILE* cert_file = fopen(certificate_path, "wb+");
        curl_easy_setopt(cert_dl_handle, CURLOPT_WRITEDATA, cert_file);
        curl_easy_perform(cert_dl_handle);
        curl_easy_setopt(curl_handle, CURLOPT_CAINFO, certificate_path);
        fclose(cert_file);

        curl_easy_cleanup(cert_dl_handle);
        printf("Certificate downloaded from https://curl.se/ca/cacert.pem...\n");
    }
}

unsigned long long int hmcd_get_dir_size(const char* dir_name)
{
    unsigned int file_count = 0;
    char** file_list = dsu_get_all_filepath(dir_name, &file_count);
    printf("%u\n", file_count);
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
    const char* target_base_url = (target_server == CHINA) ? cn_base_url : glb_base_url;
    printf("Getting chapter count...\n");
    CURL* check_handle = curl_easy_init();
    hmcd_set_https_cert(check_handle, "./cacert.pem");
    curl_easy_setopt(check_handle, CURLOPT_NOBODY, 1L);
    unsigned int chap_count = 0;
    for (chap_count = 1; ; chap_count++)
    {
        char* first_page_url = (chap_count >= 10) ?
            malloc(strlen(target_base_url) + 1 + 4 + 1 + 2 + 9 + 1) : malloc(strlen(target_base_url) + 1 + 4 + 1 + 1 + 9 + 1);
        sprintf(first_page_url, "%s/%i/%i/0001.jpg", target_base_url, target_book.book_id, chap_count);
        curl_easy_setopt(check_handle, CURLOPT_URL, first_page_url);
        curl_easy_perform(check_handle);
        long http_code = 0;
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
            printf("Unknown error: %li", http_code);
            return chap_count;
        }
        free(first_page_url);
    }
    return chap_count;
}

void hmcd_dl_book(hmcd_book target_book, unsigned int first_chap, unsigned int last_chap, DL_SERVER target_server)
{
    const char* target_base_url = (target_server == CHINA) ? cn_base_url : glb_base_url;
    const char* target_out_dir = (target_server == CHINA) ? cn_out_dir : glb_out_dir;
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
    hmcd_set_https_cert(check_handle, "./cacert.pem");
    curl_easy_setopt(check_handle, CURLOPT_NOBODY, 1L);

    CURL* dl_handle = curl_easy_init();
    hmcd_set_https_cert(dl_handle, "./cacert.pem");
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
            char* page_url = malloc(strlen(chap_url) + 8 + 1);
            sprintf(page_url, "%s00%02i.jpg", chap_url, page_count);
            char* page_name = malloc(strlen(target_out_dir) + 4 + 8 + 2 + 1 + 2 + 2 + 4 + 1);;
            sprintf(page_name, "%s/%02i%02i.jpg", chap_dirname, chap_count, page_count);
            curl_easy_setopt(check_handle, CURLOPT_URL, page_url);
            curl_easy_perform(check_handle);
            long http_code = 0;
            curl_easy_getinfo(check_handle, CURLINFO_HTTP_CODE, &http_code);
            if (http_code == 200)
            {
                curl_easy_setopt(dl_handle, CURLOPT_URL, page_url);
                curl_easy_setopt(dl_handle, CURLOPT_VERBOSE, 0L);
	            curl_easy_setopt(dl_handle, CURLOPT_NOPROGRESS, 1L);
                curl_easy_setopt(dl_handle, CURLOPT_WRITEFUNCTION, fwrite);
                FILE* page_file = fopen(page_name, "wb+");
	            curl_easy_setopt(dl_handle, CURLOPT_WRITEDATA, page_file);
	            curl_easy_perform(dl_handle);
	            fclose(page_file);
                printf("%s => %s\n", page_url, page_name);
                free(page_url);
                free(page_name);
            }
            // Assume error means end of chapter
            else if (http_code == 404 || http_code == 403)
            {
                printf("Can't download page %i of chapter %i\n", page_count, chap_count);
                free(page_url);
                free(page_name);
                break;
            }
            else
            {
                printf("Unknown error: %li", http_code);
            }
        }
        free(chap_dirname);
        free(chap_url);
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
}
