#include <hmcdcore/hmcd.h>

void* _hmcd_malloc(size_t size)
{
    void* ptr = malloc(size);
    if (ptr == NULL)
    {
        fputs("_hmcd_malloc(): Out of memory\n", stderr);
        abort();
    }
    return ptr;
}

char* _hmcd_strdup(const char* str)
{
    char* new_str = _hmcd_malloc(strlen(str) + 1);
    strcpy(new_str, str);
    return new_str;
}

void _hmcd_log(bool enable_logs, const char* str)
{
    if (enable_logs) { puts(str); }
}

void _hmcd_logf(bool enable_logs, char* fmt, ...)
{
    if (enable_logs)
    {
        va_list vaargs;
        va_start(vaargs, fmt);
        vprintf(fmt, vaargs);
        va_end(vaargs);
    }
}

void _hmcd_mkdir(const char* path, mode_t mode)
{
    int result;
    #ifdef _WIN32
        result = _mkdir(path);
    #else
        result = mkdir(path, mode);
    #endif
    if (result != 0 && errno != EEXIST)
    {
        fprintf(stderr, "_hmcd_mkdir(): Cannot create %s\n", path);
        abort();
    }
}

HMCD_ERROR hmcd_init_context(hmcd_context** pcontext, bool enable_logs, const char* output_dir, const hmcd_server* server)
{
    HMCD_ERROR error = HMCD_UNDEFINED_ERROR;

    (*pcontext) = _hmcd_malloc(sizeof(hmcd_context));
    hmcd_context* context = (*pcontext);
    context->enable_logs = enable_logs;
    context->output_dir = _hmcd_strdup(output_dir);
    context->server = server;
    context->curl_handle = curl_easy_init();
    if (!context->curl_handle)
    {
        fprintf(stderr, "%s(): curl_easy_init() failed\n", __func__);
        error = HMCD_ERROR_FAILED_CURL_INIT;
        goto hmcd_init_context_exit1;
    }
    curl_easy_setopt(context->curl_handle, CURLOPT_VERBOSE, 0L);
    curl_easy_setopt(context->curl_handle, CURLOPT_WRITEFUNCTION, fwrite);
    curl_easy_setopt(context->curl_handle, CURLOPT_FAILONERROR, 1L);
    FILE* cert_file;
    // Check for certificate
    {
        if ((cert_file = fopen(HMCD_CERTIFICATE_PATH, "r")) != NULL)
        {
            fclose(cert_file);
            curl_easy_setopt(context->curl_handle, CURLOPT_CAINFO, HMCD_CERTIFICATE_PATH);
            _hmcd_log(context->enable_logs, "Certificate found\n");
            return HMCD_SUCCESS;
        }
        else
        {
            _hmcd_logf(context->enable_logs, "Certificate not found, downloading from %s\n", HMCD_CURL_HTTPS_CERT_URL);
            cert_file = fopen(HMCD_CERTIFICATE_PATH, "wb+");
            if (!cert_file)
            {
                fprintf(stderr, "%s(): fopen failed\n", __func__);
                error = HMCD_ERROR_FAILED_FOPEN;
                goto hmcd_init_context_exit2;
            }
            curl_easy_setopt(context->curl_handle, CURLOPT_SSL_VERIFYPEER, 0L);
            curl_easy_setopt(context->curl_handle, CURLOPT_URL, HMCD_CURL_HTTPS_CERT_URL);
            curl_easy_setopt(context->curl_handle, CURLOPT_WRITEDATA, cert_file);
            curl_easy_setopt(context->curl_handle, CURLOPT_NOBODY, 0L);
            curl_easy_setopt(context->curl_handle, CURLOPT_NOPROGRESS, 1L);

            CURLcode err_code = 0;
            if ((err_code = curl_easy_perform(context->curl_handle)) != CURLE_OK)
            {
                fprintf(stderr, "%s(): curl_easy_perform failed (%s)\n", __func__, curl_easy_strerror(err_code));
                error = HMCD_ERROR_FAILED_CURL_PERFORM;
                goto hmcd_init_context_exit3;
            }
            fclose(cert_file);
            _hmcd_logf(context->enable_logs, "Successfully downloaded %s from %s\n", HMCD_CERTIFICATE_PATH, HMCD_CURL_HTTPS_CERT_URL);
            curl_easy_setopt(context->curl_handle, CURLOPT_CAINFO, HMCD_CERTIFICATE_PATH);
            curl_easy_setopt(context->curl_handle, CURLOPT_SSL_VERIFYPEER, 1L);
            return HMCD_SUCCESS;
        }
    }
    hmcd_init_context_exit3:
        fclose(cert_file);
    hmcd_init_context_exit2:
        curl_easy_cleanup(context->curl_handle);
    hmcd_init_context_exit1:
        free(context->output_dir);
        free(context);
    return error;
}

void hmcd_destroy_context(hmcd_context* context)
{
    curl_easy_cleanup(context->curl_handle);
    free(context->output_dir);
    free(context);
}

HMCD_ERROR hmcd_get_chap_cnt(hmcd_context* context, HMCD_u32* ret_chap_count, HMCD_u32 book_index)
{
    if (book_index > context->server->book_count)
    {
        return HMCD_ERROR_FAILED_BOOK_OUT_OF_RANGE;
    }
    HMCD_ERROR error;

    HMCD_u32 book_id = context->server->books[book_index].book_id;
    const char* book_name = context->server->books[book_index].book_name;
    const char* server_name = context->server->name;
    const char* base_url = context->server->base_url;
    CURL* curl_handle = context->curl_handle;

    _hmcd_logf(context->enable_logs, "Getting chapter count of %s (%u %s)\n", book_name, book_id, server_name);
    curl_easy_setopt(context->curl_handle, CURLOPT_WRITEFUNCTION, NULL);
    curl_easy_setopt(context->curl_handle, CURLOPT_NOBODY, 1L);

    // base_url + / + book_id + / + chap_count + / + "0001.jpg" + NULL
    char* first_page_url = _hmcd_malloc(strlen(base_url) + 1 + 4 + 1 + 2 + 1 + 8 + 1);

    // Check if first page (0001.jpg) exist for every chapter
    for (HMCD_u32 chap_count = 1; ; chap_count++)
    {
        sprintf(first_page_url, "%s/%04u/%u/0001.jpg", base_url, book_id, chap_count);
        curl_easy_setopt(curl_handle, CURLOPT_URL, first_page_url);
        CURLcode err_code = curl_easy_perform(curl_handle);
        if (err_code == CURLE_HTTP_RETURNED_ERROR)
        {
            *ret_chap_count = chap_count;
            _hmcd_logf(context->enable_logs, "Chapter %u XX\n", chap_count);
            _hmcd_logf(context->enable_logs, "%s (%u %s) has %u chapters\n", book_name, book_id, server_name, (*ret_chap_count) - 1);
            free(first_page_url);
            return HMCD_SUCCESS;
        }
        else if (err_code == CURLE_OK) { _hmcd_logf(context->enable_logs, "Chapter %u OK\n", chap_count); }
        else
        {
            fprintf(stderr, "%s(): curl_easy_perform failed (%s)\n", __func__, curl_easy_strerror(err_code));
            error = HMCD_ERROR_FAILED_CURL_PERFORM;
            goto hmcd_get_chap_cnt_exit1;
        }
    }
    hmcd_get_chap_cnt_exit1:
        free(first_page_url);
    return error;
}


int _hmcd_curl_progress_callback(
    void *clientp,
    curl_off_t dltotal,
    curl_off_t dlnow,
    curl_off_t ultotal,
    curl_off_t ulnow)
{
    hmcd_context* context = (hmcd_context*)clientp;
    if (context->enable_logs)
    {
        if (dltotal == 0 || dlnow == 0)
            return 0;
        float ratio = (float)dltotal / (float)PROGRESS_BAR_WIDTH;
        float scaled_dlnow = (float)dlnow / ratio;
        char progress[PROGRESS_BAR_WIDTH + 1] = {0};
        memset(progress, '-', PROGRESS_BAR_WIDTH);
        for (int index = 0; index < (int)scaled_dlnow; index++)
            progress[index] = '#';
        printf("\r%03i%% [%s] %li/%li (bytes)", (int)(scaled_dlnow * (100.F / (float)PROGRESS_BAR_WIDTH)), progress, dlnow, dltotal);
    }
    return 0;
}

HMCD_ERROR hmcd_dl_book(
    hmcd_context* context,
    HMCD_u32 book_index,
    HMCD_u32 first_chap,
    HMCD_u32 last_chap,
    bool one_directory)
{
    if (book_index > context->server->book_count)
    {
        return HMCD_ERROR_FAILED_BOOK_OUT_OF_RANGE;
    }
    HMCD_ERROR error = HMCD_UNDEFINED_ERROR;

    const char* output_dir = context->output_dir;
    const char* server_name = context->server->name;
    const char* base_url = context->server->base_url;
    HMCD_u32 book_id = context->server->books[book_index].book_id;
    const char* book_name = context->server->books[book_index].book_name;
    CURL* curl_handle = context->curl_handle;

    _hmcd_logf(context->enable_logs, "Downloading %s (%u %s chapter %u to %u)...\n",
        book_name,
        book_id,
        server_name,
        first_chap,
        last_chap);

    char* book_dirname;
    if (one_directory)
    {
        book_dirname = _hmcd_strdup(output_dir);
        _hmcd_mkdir(book_dirname, 0755);
    }
    else
    {
        // output_dir + / + server_name + / + book_id + NULL
        book_dirname = _hmcd_malloc(strlen(output_dir) + 1 + strlen(server_name) + 1 + 4 + 1);
        sprintf(book_dirname, "%s", output_dir);
        _hmcd_mkdir(book_dirname, 0755);
        sprintf(book_dirname, "%s/%s", output_dir, server_name);
        _hmcd_mkdir(book_dirname, 0755);
        sprintf(book_dirname, "%s/%s/%04u", output_dir, server_name, book_id);
        _hmcd_mkdir(book_dirname, 0755);
    }

    char* chap_dirname;
    // base_url + / + book_id + / chap_count + / + page_count + ".jpg" + NULL
    char* page_url = _hmcd_malloc(strlen(base_url) + 1 + 4 + 1 + 2 + 1 + 4 + 4 + 1);
    char* page_filename;
    if (one_directory)
    {
        chap_dirname = _hmcd_strdup(book_dirname);
        // chap_dirname + / + server_name + _ + book_id + _ + chap_count + page_count + ".jpg" + NUL
        page_filename = _hmcd_malloc((strlen(book_dirname)) + strlen(server_name) + 1 + 4 + 1 + 2 + 2 + 4 + 1);
    }
    else
    {
        // book_dirname + / + chap_count + NULL
        chap_dirname = _hmcd_malloc(strlen(book_dirname) + 1 + 2 + 1);
        // chap_dirname + / + chap_count + page_count + ".jpg" + NULL
        page_filename = _hmcd_malloc((strlen(book_dirname) + 1 + 2) + 1 + 2 + 2 + 4 + 1);
    }

    for (HMCD_u32 chap_count = first_chap; chap_count <= last_chap; chap_count++)
    {
        if (!one_directory)
        {
            sprintf(chap_dirname, "%s/%02u", book_dirname, chap_count);
            _hmcd_mkdir(chap_dirname, 0755);
        }
        _hmcd_logf(context->enable_logs, "Downloading chapter %u...\n", chap_count);
        for (HMCD_u32 page_count = 1; ; page_count++)
        {
            sprintf(page_url, "%s/%04u/%u/%04u.jpg", base_url, book_id, chap_count, page_count);

            // See if it exists
            curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1L);
            curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, NULL);
            curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, NULL);
            curl_easy_setopt(curl_handle, CURLOPT_URL, page_url);
            curl_easy_setopt(curl_handle, CURLOPT_NOBODY, 1L);

            CURLcode err_code = curl_easy_perform(curl_handle);
            if (err_code == CURLE_HTTP_RETURNED_ERROR)
            {
                _hmcd_logf(context->enable_logs, "%s => %s XX\n", page_url, page_filename);
                _hmcd_logf(context->enable_logs, "Downloaded chapter %u (%u pages)\n", chap_count, page_count);
                break;
            }
            else if (err_code == CURLE_OK)
            {
                if (one_directory)
                {
                    sprintf(page_filename, "%s/%s_%04u_%02u%02u.jpg", chap_dirname, server_name, book_id, chap_count, page_count);
                }
                else
                {
                    sprintf(page_filename, "%s/%02u%02u.jpg", chap_dirname, chap_count, page_count);
                }
                FILE* page_file = fopen(page_filename, "wb+");
                if (!page_file)
                {
                    fprintf(stderr, "%s(): fopen failed\n", __func__);
                    error = HMCD_ERROR_FAILED_FOPEN;
                    goto hmcd_dl_book_exit1;
                }
                curl_easy_setopt(curl_handle, CURLOPT_URL, page_url);
                curl_easy_setopt(curl_handle, CURLOPT_HEADER, 0L);
                curl_easy_setopt(curl_handle, CURLOPT_NOBODY, 0L);
                curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, fwrite);
                curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 0L);
                curl_easy_setopt(curl_handle, CURLOPT_XFERINFOFUNCTION, _hmcd_curl_progress_callback);
                curl_easy_setopt(curl_handle, CURLOPT_XFERINFODATA, context);
	            curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, page_file);

                err_code = curl_easy_perform(curl_handle);
                _hmcd_log(context->enable_logs, "\n");
                if (err_code != CURLE_OK)
                {
                    fprintf(stderr, "%s(): curl_easy_perform failed (%s)\n", __func__, curl_easy_strerror(err_code));
                    fclose(page_file);
                    error = HMCD_ERROR_FAILED_CURL_PERFORM;
                    goto hmcd_dl_book_exit1;
                }
                fclose(page_file);
                _hmcd_logf(context->enable_logs, "%s => %s OK\n", page_url, page_filename);
            }
            else
            {
                fprintf(stderr, "%s(): curl_easy_perform failed (%s)\n", __func__, curl_easy_strerror(err_code));
                error = HMCD_ERROR_FAILED_CURL_PERFORM;
                goto hmcd_dl_book_exit1;
            }
        }
    }
    _hmcd_logf(context->enable_logs, "Downloaded %s (%u %s chapter %u to %u)\n",
        book_name,
        book_id,
        server_name,
        first_chap,
        last_chap);
    error = HMCD_SUCCESS;
    hmcd_dl_book_exit1:
        free(book_dirname);
        free(chap_dirname);
        free(page_url);
        free(page_filename);
    return error;
}