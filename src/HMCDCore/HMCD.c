#include <HMCD.h>

int hmcd_set_https_cert(CURL* curl_handle, const char* certificate_path)
{
    if (curl_handle == NULL)
    {
        fprintf(stderr, "hmcd_set_https_cert(), uninitialized handle\n");
        return -4;
    }
    FILE* cert_file;
    if ((cert_file = fopen(certificate_path, "r")) != NULL)
    {
        printf("Certificate exists!\n");
        curl_easy_setopt(curl_handle, CURLOPT_CAINFO, certificate_path);
        return 0;
    }
    else
    {
        printf("Certificate does not exists downloading from %s...\n", HMCD_CURL_HTTPS_CERT_URL);
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

        // Download
        long http_code = 0;
        curl_easy_setopt(cert_dl_handle, CURLOPT_URL, HMCD_CURL_HTTPS_CERT_URL);
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
            fprintf(stderr, "hmcd_set_https_cert():curl_easy_perform() failed, error: %li\n", http_code);
            fclose(cert_file);
            curl_easy_cleanup(cert_dl_handle);
            return -3;
        }
        curl_easy_setopt(curl_handle, CURLOPT_CAINFO, certificate_path);

        curl_easy_cleanup(cert_dl_handle);
        printf("Certificate downloaded from %s...\n", HMCD_CURL_HTTPS_CERT_URL);
        fclose(cert_file);
        return 1;
    }
}

unsigned long long int hmcd_get_dir_size(const char* dir_name)
{
    if (dir_name == NULL)
    {
        fprintf(stderr, "hmcd_get_dir_size(), dir_name is a null pointer\n");
    }
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

unsigned int hmcd_get_chap_cnt(const HmcdServer* target_server, unsigned int book_index)
{
    if (book_index >= target_server->book_count)
    {
        fprintf(stderr, "hmcd_get_chap_cnt(), book_index out of range\n");
        return 0;
    }
    const char* target_base_url = target_server->base_url;
    unsigned int book_id = target_server->books[book_index].book_id;

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
        sprintf(first_page_url, "%s/%i/%i/0001.jpg", target_base_url, book_id, chap_count);
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

int hmcd_dl_book(const HmcdServer* target_server, unsigned int book_index, unsigned int first_chap, unsigned int last_chap)
{
    if (book_index >= target_server->book_count)
    {
        fprintf(stderr, "hmcd_dl_book(), book_index out of range\n");
        return -8;
    }
    const char* target_base_url = target_server->base_url;
    const char* target_out_dir = target_server->out_dir;
    unsigned int book_id = target_server->books[book_index].book_id;
    const char* book_name = target_server->books[book_index].book_name;

    clock_t clock_start = clock();

    char* book_dirname = malloc(strlen(target_out_dir) + 1 + 4 + 1);
    sprintf(book_dirname, "%s_%i", target_out_dir, book_id);

#ifdef _WIN32
    mkdir(book_dirname);
#else
    mkdir(book_dirname, 0777);
#endif

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

    // We only want to check to http code so we do not need the http body
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

    // We only want the page data without the header
    curl_easy_setopt(dl_handle, CURLOPT_HEADER, 0L);

    // For all chapter in the range
    for (unsigned int chap_count = first_chap; chap_count <= last_chap; chap_count++)
    {
        char* chap_url = (chap_count >= 10) ?
            malloc(strlen(target_base_url) + 1 + 4 + 1 + 2 + 1 + 1) :
            malloc(strlen(target_base_url) + 1 + 4 + 1 + 1 + 1 + 1);
        sprintf(chap_url, "%s/%i/%i/", target_base_url, book_id, chap_count);

        char* chap_dirname = malloc(strlen(book_dirname) + strlen("/Chapter") + 2 + 1);
        sprintf(chap_dirname, "%s/Chapter%02i", book_dirname, chap_count);

#ifdef _WIN32
        mkdir(chap_dirname);
#else
        mkdir(chap_dirname, 0777);
#endif

        printf("Downloading chapter %i\n", chap_count);
        unsigned int page_count = 0;

        // For all pages until 404 or 403 error
        for (page_count = 1; ; page_count++)
        {
            long http_code = 0;
            char* page_url = malloc(strlen(chap_url) + 8 + 1);
            sprintf(page_url, "%s00%02i.jpg", chap_url, page_count);

            char* page_name = malloc(strlen(target_out_dir) + 4 + 8 + 2 + 1 + 2 + 2 + 4 + 1);;
            sprintf(page_name, "%s/%02i%02i.jpg", chap_dirname, chap_count, page_count);

            // Get http_code
            curl_easy_setopt(check_handle, CURLOPT_URL, page_url);
            curl_easy_perform(check_handle);

            curl_easy_getinfo(check_handle, CURLINFO_HTTP_CODE, &http_code);

            // If page exists
            if (http_code == 200)
            {
                // Download it and write the data
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

                // If somehow we can't get it
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
            // Other errors will fallback here
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
        book_name,
        (target_server->dl_server == CHINA) ? "china" : "global",
        first_chap,
        last_chap,
        elapsed_sec);
    return 0;
}
