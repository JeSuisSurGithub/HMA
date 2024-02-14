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

#include <hmcd/hmcd.h>

void* hmcd_malloc_(size_t size)
{
    void* ptr = malloc(size);
    if (ptr == NULL)
    {
        fputs("hmcd_malloc_(): Out of memory\n", stderr);
        abort();
    }
    return ptr;
}

char* hmcd_strdup_(const char* str)
{
    char* new_str = hmcd_malloc_(strlen(str) + 1);
    strcpy(new_str, str);
    return new_str;
}

void hmcd_log_(bool enable_logs, const char* str)
{
    if (enable_logs) { puts(str); }
}

void hmcd_logf_(bool enable_logs, char* fmt, ...)
{
    if (enable_logs)
    {
        va_list vaargs;
        va_start(vaargs, fmt);
        vprintf(fmt, vaargs);
        va_end(vaargs);
    }
}

void hmcd_mkdir_(const char* path, mode_t mode)
{
    int result;
    #ifdef _WIN32
        result = _mkdir(path);
    #else
        result = mkdir(path, mode);
    #endif
    if (result != 0 && errno != EEXIST)
    {
        fprintf(stderr, "hmcd_mkdir_(): Cannot create %s\n", path);
        abort();
    }
}

HMCD_ERR hmcd_init_ctx(hmcd_ctx** pctx, bool enable_logs, const char* output_dir, const hmcd_srv* server)
{
    HMCD_ERR error = HMCD_UNDEFINED_ERROR;

    (*pctx) = hmcd_malloc_(sizeof(hmcd_ctx));
    hmcd_ctx* ctx = (*pctx);
    ctx->enable_logs = enable_logs;
    ctx->output_dir = hmcd_strdup_(output_dir);
    ctx->server = server;
    ctx->curl_handle = curl_easy_init();
    if (!ctx->curl_handle)
    {
        fprintf(stderr, "%s(): curl_easy_init() failed\n", __func__);
        error = HMCD_ERR_FAILED_CURL_INIT;
        goto hmcd_init_ctx_exit1;
    }
    curl_easy_setopt(ctx->curl_handle, CURLOPT_VERBOSE, 0L);
    curl_easy_setopt(ctx->curl_handle, CURLOPT_WRITEFUNCTION, fwrite);
    curl_easy_setopt(ctx->curl_handle, CURLOPT_FAILONERROR, 1L);
    FILE* cert_file;
    // Check for certificate
    {
        if ((cert_file = fopen(HMCD_CERTIFICATE_PATH, "r")) != NULL)
        {
            fclose(cert_file);
            curl_easy_setopt(ctx->curl_handle, CURLOPT_CAINFO, HMCD_CERTIFICATE_PATH);
            hmcd_log_(ctx->enable_logs, "Certificate found\n");
            return HMCD_SUCCESS;
        }
        else
        {
            hmcd_logf_(ctx->enable_logs, "Certificate not found, downloading from %s\n", HMCD_CURL_HTTPS_CERT_URL);
            cert_file = fopen(HMCD_CERTIFICATE_PATH, "wb+");
            if (!cert_file)
            {
                fprintf(stderr, "%s(): fopen failed\n", __func__);
                error = HMCD_ERR_FAILED_FOPEN;
                goto hmcd_init_ctx_exit2;
            }
            curl_easy_setopt(ctx->curl_handle, CURLOPT_SSL_VERIFYPEER, 0L);
            curl_easy_setopt(ctx->curl_handle, CURLOPT_URL, HMCD_CURL_HTTPS_CERT_URL);
            curl_easy_setopt(ctx->curl_handle, CURLOPT_WRITEDATA, cert_file);
            curl_easy_setopt(ctx->curl_handle, CURLOPT_NOBODY, 0L);
            curl_easy_setopt(ctx->curl_handle, CURLOPT_NOPROGRESS, 1L);

            CURLcode err_code = 0;
            if ((err_code = curl_easy_perform(ctx->curl_handle)) != CURLE_OK)
            {
                fprintf(stderr, "%s(): curl_easy_perform failed (%s)\n", __func__, curl_easy_strerror(err_code));
                error = HMCD_ERR_FAILED_CURL_PERFORM;
                goto hmcd_init_ctx_exit3;
            }
            fclose(cert_file);
            hmcd_logf_(ctx->enable_logs, "Successfully downloaded %s from %s\n", HMCD_CERTIFICATE_PATH, HMCD_CURL_HTTPS_CERT_URL);
            curl_easy_setopt(ctx->curl_handle, CURLOPT_CAINFO, HMCD_CERTIFICATE_PATH);
            curl_easy_setopt(ctx->curl_handle, CURLOPT_SSL_VERIFYPEER, 1L);
            return HMCD_SUCCESS;
        }
    }
    hmcd_init_ctx_exit3:
        fclose(cert_file);
    hmcd_init_ctx_exit2:
        curl_easy_cleanup(ctx->curl_handle);
    hmcd_init_ctx_exit1:
        free(ctx->output_dir);
        free(ctx);
    return error;
}

void hmcd_free_ctx(hmcd_ctx* ctx)
{
    curl_easy_cleanup(ctx->curl_handle);
    free(ctx->output_dir);
    free(ctx);
}

HMCD_ERR hmcd_get_chap_cnt(hmcd_ctx* ctx, hmcd_u32* ret_chap_count, hmcd_u32 book_index)
{
    if (book_index > ctx->server->book_count)
    {
        return HMCD_ERR_FAILED_BOOK_OUT_OF_RANGE;
    }
    HMCD_ERR error;

    hmcd_u32 book_id = ctx->server->books[book_index].book_id;
    const char* book_name = ctx->server->books[book_index].book_name;
    const char* server_name = ctx->server->name;
    const char* base_url = ctx->server->base_url;
    CURL* curl_handle = ctx->curl_handle;

    hmcd_logf_(ctx->enable_logs, "Getting chapter count of %s (%u %s)\n", book_name, book_id, server_name);
    curl_easy_setopt(ctx->curl_handle, CURLOPT_WRITEFUNCTION, NULL);
    curl_easy_setopt(ctx->curl_handle, CURLOPT_NOBODY, 1L);

    // base_url + / + book_id + / + chap_count + / + "0001.jpg" + NULL
    char* first_page_url = hmcd_malloc_(strlen(base_url) + 1 + 4 + 1 + 2 + 1 + 8 + 1);

    // Check if first page (0001.jpg) exist for every chapter
    for (hmcd_u32 chap_count = 1; ; chap_count++)
    {
        sprintf(first_page_url, "%s/%04u/%u/0001.jpg", base_url, book_id, chap_count);
        curl_easy_setopt(curl_handle, CURLOPT_URL, first_page_url);
        CURLcode err_code = curl_easy_perform(curl_handle);
        if (err_code == CURLE_HTTP_RETURNED_ERROR)
        {
            *ret_chap_count = chap_count;
            hmcd_logf_(ctx->enable_logs, "Chapter %u XX\n", chap_count);
            hmcd_logf_(ctx->enable_logs, "%s (%u %s) has %u chapters\n", book_name, book_id, server_name, (*ret_chap_count) - 1);
            free(first_page_url);
            return HMCD_SUCCESS;
        }
        else if (err_code == CURLE_OK) { hmcd_logf_(ctx->enable_logs, "Chapter %u OK\n", chap_count); }
        else
        {
            fprintf(stderr, "%s(): curl_easy_perform failed (%s)\n", __func__, curl_easy_strerror(err_code));
            error = HMCD_ERR_FAILED_CURL_PERFORM;
            goto hmcd_get_chap_cnt_exit1;
        }
    }
    hmcd_get_chap_cnt_exit1:
        free(first_page_url);
    return error;
}


int hmcd_curl_progress_callback_(
    void *clientp,
    curl_off_t dltotal,
    curl_off_t dlnow,
    curl_off_t ultotal,
    curl_off_t ulnow)
{
    hmcd_ctx* ctx = (hmcd_ctx*)clientp;
    if (ctx->enable_logs)
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

HMCD_ERR hmcd_dl_book(
    hmcd_ctx* ctx,
    hmcd_u32 book_index,
    hmcd_u32 first_chap,
    hmcd_u32 last_chap,
    bool one_directory)
{
    if (book_index > ctx->server->book_count)
    {
        return HMCD_ERR_FAILED_BOOK_OUT_OF_RANGE;
    }
    HMCD_ERR error = HMCD_UNDEFINED_ERROR;

    const char* output_dir = ctx->output_dir;
    const char* server_name = ctx->server->name;
    const char* base_url = ctx->server->base_url;
    hmcd_u32 book_id = ctx->server->books[book_index].book_id;
    const char* book_name = ctx->server->books[book_index].book_name;
    CURL* curl_handle = ctx->curl_handle;

    hmcd_logf_(ctx->enable_logs, "Downloading %s (%u %s chapter %u to %u)...\n",
        book_name,
        book_id,
        server_name,
        first_chap,
        last_chap);

    char* book_dirpath;
    if (one_directory)
    {
        book_dirpath = hmcd_strdup_(output_dir);
        hmcd_mkdir_(book_dirpath, 0755);
    }
    else
    {
        // output_dir + / + server_name + / + book_id + NULL
        book_dirpath = hmcd_malloc_(strlen(output_dir) + 1 + strlen(server_name) + 1 + 4 + 1);
        sprintf(book_dirpath, "%s", output_dir);
        hmcd_mkdir_(book_dirpath, 0755);
        sprintf(book_dirpath, "%s/%s", output_dir, server_name);
        hmcd_mkdir_(book_dirpath, 0755);
        sprintf(book_dirpath, "%s/%s/%04u", output_dir, server_name, book_id);
        hmcd_mkdir_(book_dirpath, 0755);
    }

    char* chap_dirpath;
    // base_url + / + book_id + / chap_count + / + page_count + ".jpg" + NULL
    char* page_url = hmcd_malloc_(strlen(base_url) + 1 + 4 + 1 + 2 + 1 + 4 + 4 + 1);
    char* page_filepath;
    if (one_directory)
    {
        chap_dirpath = hmcd_strdup_(book_dirpath);
        // chap_dirpath + / + server_name + _ + book_id + _ + chap_count + page_count + ".jpg" + NUL
        page_filepath = hmcd_malloc_((strlen(book_dirpath)) + strlen(server_name) + 1 + 4 + 1 + 2 + 2 + 4 + 1);
    }
    else
    {
        // book_dirpath + / + chap_count + NULL
        chap_dirpath = hmcd_malloc_(strlen(book_dirpath) + 1 + 2 + 1);
        // chap_dirpath + / + chap_count + page_count + ".jpg" + NULL
        page_filepath = hmcd_malloc_((strlen(book_dirpath) + 1 + 2) + 1 + 2 + 2 + 4 + 1);
    }

    for (hmcd_u32 chap_count = first_chap; chap_count <= last_chap; chap_count++)
    {
        if (!one_directory)
        {
            sprintf(chap_dirpath, "%s/%02u", book_dirpath, chap_count);
            hmcd_mkdir_(chap_dirpath, 0755);
        }
        hmcd_logf_(ctx->enable_logs, "Downloading chapter %u...\n", chap_count);
        for (hmcd_u32 page_count = 1; ; page_count++)
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
                hmcd_logf_(ctx->enable_logs, "%s => %s XX\n", page_url, page_filepath);
                hmcd_logf_(ctx->enable_logs, "Downloaded chapter %u (%u pages)\n", chap_count, page_count);
                break;
            }
            else if (err_code == CURLE_OK)
            {
                if (one_directory)
                {
                    sprintf(page_filepath, "%s/%s_%04u_%02u%02u.jpg", chap_dirpath, server_name, book_id, chap_count, page_count);
                }
                else
                {
                    sprintf(page_filepath, "%s/%02u%02u.jpg", chap_dirpath, chap_count, page_count);
                }
                FILE* page_file = fopen(page_filepath, "wb+");
                if (!page_file)
                {
                    fprintf(stderr, "%s(): fopen failed\n", __func__);
                    error = HMCD_ERR_FAILED_FOPEN;
                    goto hmcd_dl_book_exit1;
                }
                curl_easy_setopt(curl_handle, CURLOPT_URL, page_url);
                curl_easy_setopt(curl_handle, CURLOPT_HEADER, 0L);
                curl_easy_setopt(curl_handle, CURLOPT_NOBODY, 0L);
                curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, fwrite);
                curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 0L);
                curl_easy_setopt(curl_handle, CURLOPT_XFERINFOFUNCTION, hmcd_curl_progress_callback_);
                curl_easy_setopt(curl_handle, CURLOPT_XFERINFODATA, ctx);
	            curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, page_file);

                err_code = curl_easy_perform(curl_handle);
                hmcd_log_(ctx->enable_logs, "\n");
                if (err_code != CURLE_OK)
                {
                    fprintf(stderr, "%s(): curl_easy_perform failed (%s)\n", __func__, curl_easy_strerror(err_code));
                    fclose(page_file);
                    error = HMCD_ERR_FAILED_CURL_PERFORM;
                    goto hmcd_dl_book_exit1;
                }
                fclose(page_file);
                hmcd_logf_(ctx->enable_logs, "%s => %s OK\n", page_url, page_filepath);
            }
            else
            {
                fprintf(stderr, "%s(): curl_easy_perform failed (%s)\n", __func__, curl_easy_strerror(err_code));
                error = HMCD_ERR_FAILED_CURL_PERFORM;
                goto hmcd_dl_book_exit1;
            }
        }
    }
    hmcd_logf_(ctx->enable_logs, "Downloaded %s (%u %s chapter %u to %u)\n",
        book_name,
        book_id,
        server_name,
        first_chap,
        last_chap);
    error = HMCD_SUCCESS;
    hmcd_dl_book_exit1:
        free(book_dirpath);
        free(chap_dirpath);
        free(page_url);
        free(page_filepath);
    return error;
}