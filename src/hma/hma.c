/**
 * Honkai Manhua Archiver (HMA)
 * Copyright (C) 2022 JeSuis
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

#include <hma/hma.h>

const char* HMA_CURL_HTTPS_CERT_URL = "https://curl.se/ca/cacert.pem";
const char* HMA_CERTIFICATE_PATH = "cacert.pem";

const hma_srv HMA_GLB_SERVER =
{
    HMA_GLOBAL,
    "global",
    "https://act-webstatic.hoyoverse.com/manga/static/comic/book",
    22,
    {
        {1001, "Ai-Chan Facts"         },
        {1002, "Gratitude Arc"         },
        {1003, "AE Invasion"           },
        {1004, "Azure Waters"          },
        {1005, "Second Eruption"       },
        {1006, "Gemina: Tales"         },
        {1007, "World Serpent"         },
        {1008, "Kiana Plays Honkai"    },
        {1009, "London Holiday"        },
        {1010, "Moon Shadow"           },
        {1011, "Elan Palatinus"        },
        {1012, "SpringFest"            },
        {1013, "ELF"                   },
        {1014, "Second Key"            },
        {1015, "Escape From Nagazora"  },
        {1016, "St. Freya High"        },
        {1017, "Gemina: Invasion"      },
        {1018, "Divine Key"            },
        {1019, "Cooking With Valkyries"},
        {1020, "Empyrean Blade"        },
        {1021, "Alien Space"           },
        {1022, "Spring Festival Trip"  }
    }
};

const hma_srv HMA_CN_SERVER =
{
    HMA_CHINA,
    "china",
    "https://act-webstatic.mihoyo.com/new_static_v2/comic/book",
    23,
    {
        {1001, "逃离长空篇"},
        {1002, "樱花追忆篇"},
        {1004, "绀海篇"},
        {1005, "绯樱篇"},
        {1006, "逆熵入侵篇"},
        {1007, "恩返篇"},
        {1008, "月影篇"},
        {1009, "紫鸢篇"},
        {1010, "神之键秘话"},
        {1011, "玩崩坏3的琪亚娜"},
        {1012, "第二次崩坏"},
        {1013, "女武神的餐桌"},
        {1014, "夏日回忆-预告篇"},
        {1015, "双子：起源"},
        {1016, "双子：入侵"},
        {1017, "蛇之篇"},
        {1018, "雾都假日"},
        {1019, "年岁"},
        {1020, "武装人偶"},
        {1021, "传承"},
        {1022, "云墨剑心"},
        {1023, "异乡"},
        {1024, "新春旅行"}
    }
};

void hma_log_(bool enable_logs, const char* str)
{
    if (enable_logs) { puts(str); }
}

void hma_logf_(bool enable_logs, char* fmt, ...)
{
    if (enable_logs)
    {
        va_list vaargs;
        va_start(vaargs, fmt);
        vprintf(fmt, vaargs);
        va_end(vaargs);
    }
}

void hma_mkdir_(const char* path, mode_t mode)
{
    int result;
    #ifdef _WIN32
        result = _mkdir(path);
    #else
        result = mkdir(path, mode);
    #endif
    if (result != 0 && errno != EEXIST)
    {
        fprintf(stderr, "hma_mkdir_(): Cannot create %s\n", path);
        abort();
    }
}

HMA_ERR hma_init_ctx(hma_ctx** pctx, bool enable_logs, const char* output_dir, const hma_srv* server)
{
    HMA_ERR error = HMA_UNDEFINED_ERROR;

    (*pctx) = malloc(sizeof(hma_ctx));
    hma_ctx* ctx = (*pctx);
    ctx->enable_logs = enable_logs;
    ctx->output_dir = strdup(output_dir);
    ctx->server = server;
    ctx->curl_handle = curl_easy_init();
    if (!ctx->curl_handle)
    {
        fprintf(stderr, "%s(): curl_easy_init() failed\n", __func__);
        error = HMA_ERR_FAILED_CURL_INIT;
        goto hma_init_ctx_exit1;
    }
    curl_easy_setopt(ctx->curl_handle, CURLOPT_VERBOSE, 0L);
    curl_easy_setopt(ctx->curl_handle, CURLOPT_WRITEFUNCTION, fwrite);
    curl_easy_setopt(ctx->curl_handle, CURLOPT_FAILONERROR, 1L);
    FILE* cert_file;
    // Check for certificate
    {
        if ((cert_file = fopen(HMA_CERTIFICATE_PATH, "r")) != NULL)
        {
            fclose(cert_file);
            curl_easy_setopt(ctx->curl_handle, CURLOPT_CAINFO, HMA_CERTIFICATE_PATH);
            hma_log_(ctx->enable_logs, "Certificate found\n");
            return HMA_SUCCESS;
        }
        else
        {
            hma_logf_(ctx->enable_logs, "Certificate not found, downloading from %s\n", HMA_CURL_HTTPS_CERT_URL);
            cert_file = fopen(HMA_CERTIFICATE_PATH, "wb+");
            if (!cert_file)
            {
                fprintf(stderr, "%s(): fopen failed\n", __func__);
                error = HMA_ERR_FAILED_FOPEN;
                goto hma_init_ctx_exit2;
            }
            curl_easy_setopt(ctx->curl_handle, CURLOPT_SSL_VERIFYPEER, 0L);
            curl_easy_setopt(ctx->curl_handle, CURLOPT_URL, HMA_CURL_HTTPS_CERT_URL);
            curl_easy_setopt(ctx->curl_handle, CURLOPT_WRITEDATA, cert_file);
            curl_easy_setopt(ctx->curl_handle, CURLOPT_NOBODY, 0L);
            curl_easy_setopt(ctx->curl_handle, CURLOPT_NOPROGRESS, 1L);

            CURLcode err_code = 0;
            if ((err_code = curl_easy_perform(ctx->curl_handle)) != CURLE_OK)
            {
                fprintf(stderr, "%s(): curl_easy_perform failed (%s)\n", __func__, curl_easy_strerror(err_code));
                error = HMA_ERR_FAILED_CURL_PERFORM;
                goto hma_init_ctx_exit3;
            }
            fclose(cert_file);
            hma_logf_(ctx->enable_logs, "Successfully downloaded %s from %s\n", HMA_CERTIFICATE_PATH, HMA_CURL_HTTPS_CERT_URL);
            curl_easy_setopt(ctx->curl_handle, CURLOPT_CAINFO, HMA_CERTIFICATE_PATH);
            curl_easy_setopt(ctx->curl_handle, CURLOPT_SSL_VERIFYPEER, 1L);
            return HMA_SUCCESS;
        }
    }
    hma_init_ctx_exit3:
        fclose(cert_file);
    hma_init_ctx_exit2:
        curl_easy_cleanup(ctx->curl_handle);
    hma_init_ctx_exit1:
        free(ctx->output_dir);
        free(ctx);
    return error;
}

void hma_free_ctx(hma_ctx* ctx)
{
    curl_easy_cleanup(ctx->curl_handle);
    free(ctx->output_dir);
    free(ctx);
}

HMA_ERR hma_get_chap_cnt(hma_ctx* ctx, hma_u32* ret_chap_count, hma_u32 book_index)
{
    if (book_index > ctx->server->book_count)
    {
        return HMA_ERR_FAILED_BOOK_OUT_OF_RANGE;
    }
    HMA_ERR error;

    hma_u32 book_id = ctx->server->books[book_index].book_id;
    const char* book_name = ctx->server->books[book_index].book_name;
    const char* server_name = ctx->server->name;
    const char* base_url = ctx->server->base_url;
    CURL* curl_handle = ctx->curl_handle;

    hma_logf_(ctx->enable_logs, "Getting chapter count of %s (%u %s)\n", book_name, book_id, server_name);
    curl_easy_setopt(ctx->curl_handle, CURLOPT_WRITEFUNCTION, NULL);
    curl_easy_setopt(ctx->curl_handle, CURLOPT_NOBODY, 1L);

    // base_url + / + book_id + / + chap_count + / + "0001.jpg" + NULL
    char* first_page_url = malloc(strlen(base_url) + 1 + 4 + 1 + 2 + 1 + 8 + 1);

    // Check if first page (0001.jpg) exist for every chapter
    for (hma_u32 chap_count = 1; ; chap_count++)
    {
        sprintf(first_page_url, "%s/%04u/%u/0001.jpg", base_url, book_id, chap_count);
        curl_easy_setopt(curl_handle, CURLOPT_URL, first_page_url);
        CURLcode err_code = curl_easy_perform(curl_handle);
        if (err_code == CURLE_HTTP_RETURNED_ERROR)
        {
            *ret_chap_count = chap_count;
            hma_logf_(ctx->enable_logs, "Chapter %u XX\n", chap_count);
            hma_logf_(ctx->enable_logs, "%s (%u %s) has %u chapters\n", book_name, book_id, server_name, (*ret_chap_count) - 1);
            free(first_page_url);
            return HMA_SUCCESS;
        }
        else if (err_code == CURLE_OK) { hma_logf_(ctx->enable_logs, "Chapter %u OK\n", chap_count); }
        else
        {
            fprintf(stderr, "%s(): curl_easy_perform failed (%s)\n", __func__, curl_easy_strerror(err_code));
            error = HMA_ERR_FAILED_CURL_PERFORM;
            goto hma_get_chap_cnt_exit1;
        }
    }
    hma_get_chap_cnt_exit1:
        free(first_page_url);
    return error;
}


int hma_curl_progress_callback_(
    void *clientp,
    curl_off_t dltotal,
    curl_off_t dlnow,
    curl_off_t,
    curl_off_t)
{
    hma_ctx* ctx = (hma_ctx*)clientp;
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

HMA_ERR hma_dl_book(
    hma_ctx* ctx,
    hma_u32 book_index,
    hma_u32 first_chap,
    hma_u32 last_chap,
    bool one_directory)
{
    if (book_index > ctx->server->book_count)
    {
        return HMA_ERR_FAILED_BOOK_OUT_OF_RANGE;
    }
    HMA_ERR error = HMA_UNDEFINED_ERROR;

    const char* output_dir = ctx->output_dir;
    const char* server_name = ctx->server->name;
    const char* base_url = ctx->server->base_url;
    hma_u32 book_id = ctx->server->books[book_index].book_id;
    const char* book_name = ctx->server->books[book_index].book_name;
    CURL* curl_handle = ctx->curl_handle;

    hma_logf_(ctx->enable_logs, "Downloading %s (%u %s chapter %u to %u)...\n",
        book_name,
        book_id,
        server_name,
        first_chap,
        last_chap);

    char* book_dirpath;
    if (one_directory)
    {
        book_dirpath = strdup(output_dir);
        hma_mkdir_(book_dirpath, 0755);
    }
    else
    {
        // output_dir + / + server_name + / + book_id + NULL
        book_dirpath = malloc(strlen(output_dir) + 1 + strlen(server_name) + 1 + 4 + 1);
        sprintf(book_dirpath, "%s", output_dir);
        hma_mkdir_(book_dirpath, 0755);
        sprintf(book_dirpath, "%s/%s", output_dir, server_name);
        hma_mkdir_(book_dirpath, 0755);
        sprintf(book_dirpath, "%s/%s/%04u", output_dir, server_name, book_id);
        hma_mkdir_(book_dirpath, 0755);
    }

    char* chap_dirpath;
    // base_url + / + book_id + / chap_count + / + page_count + ".jpg" + NULL
    char* page_url = malloc(strlen(base_url) + 1 + 4 + 1 + 2 + 1 + 4 + 4 + 1);
    char* page_filepath;
    if (one_directory)
    {
        chap_dirpath = strdup(book_dirpath);
        // chap_dirpath + / + server_name + _ + book_id + _ + chap_count + page_count + ".jpg" + NULL
        page_filepath = malloc(strlen(chap_dirpath) + 1 + strlen(server_name) + 1 + 4 + 1 + 2 + 2 + 4 + 1);
    }
    else
    {
        // book_dirpath + / + chap_count + NULL
        chap_dirpath = malloc(strlen(book_dirpath) + 1 + 2 + 1);
        // chap_dirpath + / + chap_count + page_count + ".jpg" + NULL
        page_filepath = malloc((strlen(book_dirpath) + 1 + 2) + 1 + 2 + 2 + 4 + 1);
    }

    for (hma_u32 chap_count = first_chap; chap_count <= last_chap; chap_count++)
    {
        if (!one_directory)
        {
            sprintf(chap_dirpath, "%s/%02u", book_dirpath, chap_count);
            hma_mkdir_(chap_dirpath, 0755);
        }
        hma_logf_(ctx->enable_logs, "Downloading chapter %u...\n", chap_count);
        for (hma_u32 page_count = 1; ; page_count++)
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
                hma_logf_(ctx->enable_logs, "%s => %s XX\n", page_url, page_filepath);
                hma_logf_(ctx->enable_logs, "Downloaded chapter %u (%u pages)\n", chap_count, page_count);
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
                    error = HMA_ERR_FAILED_FOPEN;
                    goto hma_dl_book_exit1;
                }
                curl_easy_setopt(curl_handle, CURLOPT_URL, page_url);
                curl_easy_setopt(curl_handle, CURLOPT_HEADER, 0L);
                curl_easy_setopt(curl_handle, CURLOPT_NOBODY, 0L);
                curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, fwrite);
                curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 0L);
                curl_easy_setopt(curl_handle, CURLOPT_XFERINFOFUNCTION, hma_curl_progress_callback_);
                curl_easy_setopt(curl_handle, CURLOPT_XFERINFODATA, ctx);
	            curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, page_file);

                err_code = curl_easy_perform(curl_handle);
                hma_log_(ctx->enable_logs, "\n");
                if (err_code != CURLE_OK)
                {
                    fprintf(stderr, "%s(): curl_easy_perform failed (%s)\n", __func__, curl_easy_strerror(err_code));
                    fclose(page_file);
                    error = HMA_ERR_FAILED_CURL_PERFORM;
                    goto hma_dl_book_exit1;
                }
                fclose(page_file);
                hma_logf_(ctx->enable_logs, "%s => %s OK\n", page_url, page_filepath);
            }
            else
            {
                fprintf(stderr, "%s(): curl_easy_perform failed (%s)\n", __func__, curl_easy_strerror(err_code));
                error = HMA_ERR_FAILED_CURL_PERFORM;
                goto hma_dl_book_exit1;
            }
        }
    }
    hma_logf_(ctx->enable_logs, "Downloaded %s (%u %s chapter %u to %u)\n",
        book_name,
        book_id,
        server_name,
        first_chap,
        last_chap);
    error = HMA_SUCCESS;
    hma_dl_book_exit1:
        free(book_dirpath);
        free(chap_dirpath);
        free(page_url);
        free(page_filepath);
    return error;
}