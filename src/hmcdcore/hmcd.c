#include <hmcdcore/hmcd.h>

bool g_hmcd_enable_logs = true;

#define HMCD_LOG(FMT, ...) \
    if (hmcd_enabled_logs()) { printf("LOG: "FMT"\n", ##__VA_ARGS__); }

#define HMCD_LOG_ERR(FMT, ...)  \
    fprintf(stderr, "ERROR: (%s:%i), "FMT"\n", __FILE__, __LINE__, ##__VA_ARGS__);

#define HMCD_ASSERT_W_ERR_LOG(ASSERT, FMT, ...) \
    if(!(ASSERT))   \
    {   \
        HMCD_LOG_ERR("ASSERTION ERROR: "FMT, ##__VA_ARGS__)    \
        assert(ASSERT); \
    }

#define HMCD_FOPEN_W_ERR_CHK(FPTR, FN, MODE, CLEANUPS, RETCODE)  \
    if (!((FPTR) = fopen(FN, MODE))) \
    {   \
        HMCD_LOG_ERR("fopen(%s, %s) failed", FN, MODE)  \
        CLEANUPS  \
        return (RETCODE);   \
    }

#define HMCD_CURL_INIT_W_ERR_CHK(CHPTR, CLEANUPS, RETCODE)  \
    CHPTR = curl_easy_init();   \
    if (!CHPTR) \
    {   \
        HMCD_LOG_ERR("curl_easy_init() failed") \
        CLEANUPS    \
        return (RETCODE); \
    }

#define HMCD_CURL_PERF_W_ERR_CHK(CHPTR, CCODE_VN, CLEANUPS, RETCODE)  \
    CCODE_VN = curl_easy_perform(CHPTR);    \
    if ((CCODE_VN) != CURLE_OK) \
    {   \
        HMCD_LOG_ERR("curl_easy_perform() failed, %s", curl_easy_strerror(CCODE_VN)) \
        CLEANUPS    \
        return (RETCODE); \
    }

#define HMCD_DO_SILENT(FUNCTION_CALL)   \
    hmcd_enable_logs(false); \
    FUNCTION_CALL   \
    hmcd_enable_logs(true);

#define HMCD_MALLOC_W_CHK(PTR, SIZE)   \
    PTR = malloc(SIZE); \
    if (PTR == NULL)    \
    {   \
        HMCD_LOG_ERR("Out of memory")   \
        abort();    \
    }

void hmcd_enable_logs(bool enable)
{
    g_hmcd_enable_logs = enable;
}

bool hmcd_enabled_logs()
{
    return g_hmcd_enable_logs;
}

const char* hmcd_get_server_name(HMCD_SERVER_ID server_id)
{
    if (server_id == HMCD_CHINA) { return HMCD_CN_NAME; }
    else if (server_id == HMCD_GLOBAL) { return HMCD_GLB_NAME; }
    else { return NULL; }
}

int _hmcd_set_https_cert(CURL* curl_handle, const char* certificate_path)
{
    HMCD_ASSERT_W_ERR_LOG(curl_handle != NULL, "Uninitialized handle")
    HMCD_ASSERT_W_ERR_LOG(certificate_path != NULL, "No certificate path given")

    // If we already downloaded
    FILE* cert_file;
    if ((cert_file = fopen(certificate_path, "r")) != NULL)
    {
        HMCD_LOG("Certificate already exists")
        curl_easy_setopt(curl_handle, CURLOPT_CAINFO, certificate_path);
        return 0;
    }
    else
    {
        // Download
        HMCD_LOG("Certificate does not exists downloading from %s...", HMCD_CURL_HTTPS_CERT_URL)

        CURL* cert_dl_handle;
        HMCD_CURL_INIT_W_ERR_CHK(cert_dl_handle, fclose(cert_file);, -1)
        HMCD_FOPEN_W_ERR_CHK(cert_file, certificate_path, "wb+", curl_easy_cleanup(cert_dl_handle);, -2)

        // Download
        CURLcode curl_err_code = 0;
        long http_code = 0;

        curl_easy_setopt(cert_dl_handle, CURLOPT_URL, HMCD_CURL_HTTPS_CERT_URL);
        curl_easy_setopt(cert_dl_handle, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(cert_dl_handle, CURLOPT_NOBODY, 0L);
        curl_easy_setopt(cert_dl_handle, CURLOPT_VERBOSE, 0L);
        curl_easy_setopt(cert_dl_handle, CURLOPT_NOPROGRESS, 1L);
        curl_easy_setopt(cert_dl_handle, CURLOPT_WRITEFUNCTION, fwrite);
        curl_easy_setopt(cert_dl_handle, CURLOPT_WRITEDATA, cert_file);

        HMCD_CURL_PERF_W_ERR_CHK(
            cert_dl_handle,
            curl_err_code,
            fclose(cert_file); curl_easy_cleanup(cert_dl_handle);,
            -3
        )

        curl_easy_getinfo(cert_dl_handle, CURLINFO_RESPONSE_CODE, &http_code);
        if (http_code != 200)
        {
            HMCD_LOG_ERR("curl_easy_perform(cert_dl_handle) failed, http = %li", http_code)
            fclose(cert_file);
            curl_easy_cleanup(cert_dl_handle);
            return -4;
        }

        HMCD_LOG("Certificate downloaded from %s", HMCD_CURL_HTTPS_CERT_URL);
        curl_easy_setopt(curl_handle, CURLOPT_CAINFO, certificate_path);
        curl_easy_cleanup(cert_dl_handle);
        fclose(cert_file);
        return 1;
    }
}

unsigned long long int hmcd_get_dir_size(const char* dir_name)
{
    HMCD_ASSERT_W_ERR_LOG(dir_name != NULL, "dir_name is a null pointer")

    // Get list of file and add all of their sizes
    unsigned int file_count = 0;
    char** file_list = dsu_get_all_filepath(dir_name, &file_count);
    if (!file_list)
    {
        HMCD_LOG_ERR("dsu_get_all_filepath() failed, dir_name = %s", dir_name)
        dsu_free_char_pp(file_list, file_count);
        return 0;
    }
    unsigned long long int total_dir_sz = 0;
    for (size_t index = 0; index < file_count; index++)
        total_dir_sz += dsu_get_filesize(file_list[index]);

    dsu_free_char_pp(file_list, file_count);
    return total_dir_sz;
}

unsigned int hmcd_get_chap_cnt(const hmcd_server* target_server, unsigned int book_index)
{
    HMCD_ASSERT_W_ERR_LOG(target_server != NULL, "target_server is a null pointer")
    HMCD_ASSERT_W_ERR_LOG(book_index < target_server->book_count, "book_index(%i) is out of range", book_index)

#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    const char* target_base_url = target_server->base_url;
    unsigned int book_id = target_server->books[book_index].book_id;
    const char* book_name = target_server->books[book_index].book_name;

    HMCD_LOG(
        "Getting chapter count of %s (%i %s)...",
        book_name,
        book_id,
        hmcd_get_server_name(target_server->server_id))

    CURL* check_handle;
    HMCD_CURL_INIT_W_ERR_CHK(check_handle, , 0)
    int res = _hmcd_set_https_cert(check_handle, "cacert.pem");
    if (res < 0)
    {
        HMCD_LOG_ERR("_hmcd_set_https_cert(), failed")
        curl_easy_cleanup(check_handle);
        return 0;
    }
    curl_easy_setopt(check_handle, CURLOPT_NOBODY, 1L);

    long http_code = 0;
    CURLcode curl_err_code;

    // / + book_id + / + chap_count + / + "0001.jpg" + NULL
    char* first_page_url;
    HMCD_MALLOC_W_CHK(first_page_url, strlen(target_base_url) + 1 + 4 + 1 + 2 + 9 + 1)

    // Check if first page (0001.jpg) exist for every chapter
    // Assume http 404 or 403 means no chapter and 200 means it exists
    for (unsigned int chap_count = 1; ; chap_count++)
    {
        sprintf(first_page_url, "%s/%i/%i/0001.jpg", target_base_url, book_id, chap_count);

        curl_easy_setopt(check_handle, CURLOPT_URL, first_page_url);
        HMCD_CURL_PERF_W_ERR_CHK(
            check_handle,
            curl_err_code,
            curl_easy_cleanup(check_handle); free(first_page_url);,
            0)

        curl_easy_getinfo(check_handle, CURLINFO_RESPONSE_CODE, &http_code);
        if (http_code == 404 || http_code == 403)
        {
            HMCD_LOG("Chapter %i XX", chap_count);
            HMCD_LOG(
                "%s (%i %s) has %i chapters",
                book_name,
                book_id,
                hmcd_get_server_name(target_server->server_id),
                (chap_count - 1))

            curl_easy_cleanup(check_handle);
            free(first_page_url);
            return chap_count;
        }
        else if (http_code == 200) { HMCD_LOG("Chapter %i OK", chap_count); }
        else
        {
            HMCD_LOG_ERR("curl_easy_perform(check_handle) failed, http: %li", http_code)
            curl_easy_cleanup(check_handle);
            free(first_page_url);
            return chap_count;
        }
    }
}


int _hmcd_curl_progress_callback(
    void *clientp,
    curl_off_t dltotal,
    curl_off_t dlnow,
    curl_off_t ultotal,
    curl_off_t ulnow)
{
    if (g_hmcd_enable_logs)
    {
        if (dltotal == 0 || dlnow == 0)
            return 0;
        float ratio = (float)dltotal / (float)PROGRESS_BAR_WIDTH;
        float scaled_dlnow = (float)dlnow / ratio;
        char progress[PROGRESS_BAR_WIDTH + 1] = {0};
        memset(progress, '-', PROGRESS_BAR_WIDTH);
        for (size_t index = 0; index < (int)scaled_dlnow; index++)
            progress[index] = '#';
        printf("\r%03i%% [%s] %li/%li (bytes)", (int)(scaled_dlnow * (100.F / (float)PROGRESS_BAR_WIDTH)), progress, dlnow, dltotal);
    }
    return 0;
}

int hmcd_dl_book(
    const hmcd_server* target_server,
    unsigned int book_index,
    unsigned int first_chap,
    unsigned int last_chap,
    bool one_big_dir)
{
    HMCD_ASSERT_W_ERR_LOG(target_server != NULL, "target_server is a null pointer")
    HMCD_ASSERT_W_ERR_LOG(book_index < target_server->book_count, "book_index(%i) is out of range", book_index)

#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    const char* target_base_url = target_server->base_url;
    const char* target_out_dir = target_server->out_dir;
    unsigned int book_id = target_server->books[book_index].book_id;
    const char* book_name = target_server->books[book_index].book_name;

    HMCD_LOG("Downloading %s (%i %s chapter %i to %i)...",
        book_name,
        book_id,
        hmcd_get_server_name(target_server->server_id),
        first_chap,
        last_chap);

    clock_t clock_start = clock();

    // target_out_dir + _ + book_id + NULL
    char* book_dirname;
    HMCD_MALLOC_W_CHK(book_dirname, strlen(target_out_dir) + 1 + 4 + 1)
    sprintf(book_dirname, "%s_%i", target_out_dir, book_id);

#ifdef _WIN32
    mkdir(book_dirname);
#else
    mkdir(book_dirname, 0777);
#endif

    CURL* dl_handle;
    HMCD_CURL_INIT_W_ERR_CHK(dl_handle, , -1)
    int res_dl = _hmcd_set_https_cert(dl_handle, "cacert.pem");
    if (res_dl < 0)
    {
        HMCD_LOG_ERR("_hmcd_set_https_cert(dl_handle, \"%s\") failed", "cacert.pem");
        curl_easy_cleanup(dl_handle);
        return -2;
    }
    curl_easy_setopt(dl_handle, CURLOPT_VERBOSE, 0L);

    CURLcode curl_err_code;
    long http_code;

    // target_base_url + / + book_id + / + chap_count + / + NULL
    char* chap_url;
    HMCD_MALLOC_W_CHK(chap_url, strlen(target_base_url) + 1 + 4 + 1 + 2 + 1 + 1)

    // book_dirname + / + "/Chapter" + chap_count + NULL
    char* chap_dirname;
    HMCD_MALLOC_W_CHK(chap_dirname, strlen(book_dirname) + 8 + 2 + 1)

    // chap_url + page_count + ".jpg" + NULL
    char* page_url;
    HMCD_MALLOC_W_CHK(page_url, (strlen(target_base_url) + 1 + 4 + 1 + 2 + 1) + 8 + 1)

    // chap_dirname + / + chap_count + page_count + ".jpg" + NULL
    char* page_filename;
    HMCD_MALLOC_W_CHK(page_filename, (strlen(book_dirname) + 8 + 2) + 1 + 2 + 2 + 4 + 1);

    if (one_big_dir) { strcpy(chap_dirname, book_dirname); }

    // For all chapter in the range
    for (unsigned int chap_count = first_chap; chap_count <= last_chap; chap_count++)
    {
        sprintf(chap_url, "%s/%i/%i/", target_base_url, book_id, chap_count);

        if (!one_big_dir) { sprintf(chap_dirname, "%s/Chapter%02i", book_dirname, chap_count); }
#ifdef _WIN32
        mkdir(chap_dirname);
#else
        mkdir(chap_dirname, 0777);
#endif

        HMCD_LOG("Downloading chapter %i...", chap_count);
        unsigned int page_count = 0;

        // For all pages
        // Get http code if 404 or 403 assume it means end of chapter
        // If 200 go download it
        for (page_count = 1; ; page_count++)
        {
            sprintf(page_url, "%s%04i.jpg", chap_url, page_count);

            sprintf(page_filename, "%s/%02i%02i.jpg", chap_dirname, chap_count, page_count);

            // Get http_code
            curl_easy_setopt(dl_handle, CURLOPT_URL, page_url);
            curl_easy_setopt(dl_handle, CURLOPT_NOBODY, 1L);

            HMCD_CURL_PERF_W_ERR_CHK(dl_handle, curl_err_code,
                free(book_dirname);
                free(chap_url);
                free(chap_dirname);
                free(page_url);
                free(page_filename);
                curl_easy_cleanup(dl_handle);,
                -3)

            curl_easy_getinfo(dl_handle, CURLINFO_HTTP_CODE, &http_code);
            // If page exists
            if (http_code == 200)
            {
                FILE* page_file;
                HMCD_FOPEN_W_ERR_CHK(page_file, page_filename, "wb+",
                    free(book_dirname);
                    free(chap_url);
                    free(chap_dirname);
                    free(page_url);
                    free(page_filename);
                    curl_easy_cleanup(dl_handle);,
                    -5)

                // Download it and write the data
                curl_easy_setopt(dl_handle, CURLOPT_URL, page_url);
                curl_easy_setopt(dl_handle, CURLOPT_HEADER, 0L);
                curl_easy_setopt(dl_handle, CURLOPT_NOBODY, 0L);
                curl_easy_setopt(dl_handle, CURLOPT_WRITEFUNCTION, fwrite);
                curl_easy_setopt(dl_handle, CURLOPT_NOPROGRESS, 0L);
                curl_easy_setopt(dl_handle, CURLOPT_XFERINFOFUNCTION, _hmcd_curl_progress_callback);
	            curl_easy_setopt(dl_handle, CURLOPT_WRITEDATA, page_file);

	            HMCD_CURL_PERF_W_ERR_CHK(dl_handle, curl_err_code,
                    free(book_dirname);
                    free(chap_url);
                    free(chap_dirname);
                    free(page_url);
                    free(page_filename);
                    curl_easy_cleanup(dl_handle);,
                    -6)

                if (g_hmcd_enable_logs)
                    putchar('\n');

                curl_easy_getinfo(dl_handle, CURLINFO_HTTP_CODE, &http_code);
                // If somehow we can't get it
                if (http_code != 200)
                {
                    HMCD_LOG_ERR("curl_easy_perform(dl_handle) failed, page_url = %s; http = %li", page_url, http_code)
                    free(book_dirname);
                    free(chap_url);
                    free(chap_dirname);
                    free(page_url);
                    free(page_filename);
                    curl_easy_cleanup(dl_handle);
                    return -7;
                }

	            fclose(page_file);
                HMCD_LOG("%s => %s OK", page_url, page_filename);
                curl_easy_setopt(dl_handle, CURLOPT_NOPROGRESS, 1L);
            }
            // Assume error 404(for GLOBAL server) or 403(for CN server) means end of chapter
            else if (http_code == 404 || http_code == 403)
            {
                HMCD_LOG("%s => %s XX", page_url, page_filename);
                HMCD_LOG("Downloaded chapter %i (%i pages)", chap_count, page_count);
                break;
            }
            // Other errors will fallback here
            else
            {
                HMCD_LOG_ERR("curl_easy_perform(dl_handle) failed, page_url = %s; http = %li", page_url, http_code)
                free(book_dirname);
                free(chap_url);
                free(chap_dirname);
                free(page_url);
                free(page_filename);
                curl_easy_cleanup(dl_handle);
                return -4;
            }
        }
    }
    clock_t clock_end = clock();
    float elapsed_sec = ((float)(clock_end - clock_start)) / CLOCKS_PER_SEC;
    HMCD_LOG("Downloaded %s (%i %s chapter %i to %i) in %f (seconds)",
        book_name,
        book_id,
        hmcd_get_server_name(target_server->server_id),
        first_chap,
        last_chap,
        elapsed_sec);

    free(book_dirname);
    free(chap_url);
    free(chap_dirname);
    free(page_url);
    free(page_filename);
    curl_easy_cleanup(dl_handle);
    return 0;
}

#undef HMCD_LOG
#undef HMCD_LOG_ERR
#undef HMCD_ASSERT_W_ERR_LOG
#undef HMCD_FOPEN_W_ERR_CHK
#undef HMCD_CURL_INIT_W_ERR_CHK
#undef HMCD_CURL_PERF_W_ERR_CHK
#undef HMCD_DO_SILENT
#undef HMCD_MALLOC_W_CHK