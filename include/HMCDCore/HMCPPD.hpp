#ifndef HMCDPPD_HPP
#define HMCDPPD_HPP

#include "HMCD.h"

#include <cstddef>
#include <cstdint>
#include <cstdio>

#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>

namespace hmcppd
{
    // Download server
    typedef HMCD_SERVER_ID SERVER_ID;

    /**
     * Structure representing a book
    */
    typedef struct _Book
    {
        std::uint32_t book_id;   //!< ID of the book starting at 1001
        std::string book_name;  //!< Name of the book, may be unicode
    }Book;

    class DlServer
    {
        // PUBLIC FUNCTIONS

        public: DlServer(SERVER_ID server_id)
        {
            if (server_id == SERVER_ID::HMCD_CHINA)
                dl_server = const_cast<HmcdServer*>(&HMCD_CN_SERVER);
            else if (server_id == SERVER_ID::HMCD_GLOBAL)
                dl_server = const_cast<HmcdServer*>(&HMCD_GLB_SERVER);
            else throw std::runtime_error("Unrecognized server_id = " + std::to_string(server_id) + "\n");
        }

        public: DlServer() = delete;

        public: ~DlServer()
        {
            ;
        }

        public: void enable_logs(bool enable) { hmcd_enable_logs(enable); }

        public: bool enabled_logs() { return hmcd_enabled_logs(); }

        public: std::string get_server_name()
        {
            const char* server_name = hmcd_get_server_name(dl_server->server_id);
            if (!server_name)
                throw std::runtime_error(
                    "get_server_name() failed, unrecognized server_id = " + std::to_string(dl_server->server_id) + "\n");
            return std::string(server_name);
        }

        public: std::uint64_t get_dir_size(std::string dir_name)
            { return static_cast<std::uint64_t>(hmcd_get_dir_size(dir_name.c_str())); }

        public: unsigned int get_chap_cnt(unsigned int book_index)
        {
            unsigned int chap_count = hmcd_get_chap_cnt(dl_server, book_index);
            if (chap_count == 0)
                throw std::runtime_error("get_chap_cnt() failed\n");
            return chap_count;
        }

        public: void dl_book(unsigned int book_index, unsigned int first_chap, unsigned int last_chap)
        {
            int result = hmcd_dl_book(dl_server, book_index, first_chap, last_chap);
            if (result != 0)
                throw std::runtime_error("dl_book() failed\n");
        }

        public: void switch_server()
        {
            switch (dl_server->server_id)
            {
                case SERVER_ID::HMCD_CHINA:
                    dl_server = const_cast<HmcdServer*>(&HMCD_GLB_SERVER);
                    break;
                case SERVER_ID::HMCD_GLOBAL:
                    dl_server = const_cast<HmcdServer*>(&HMCD_CN_SERVER);
                    break;
                default:
                    throw std::runtime_error("Unrecognized server_id = " + std::to_string(dl_server->server_id) + "\n");
                    break;
            }
        }

        // GETTERS - SETTERS

        public: std::uint32_t get_book_count() { return dl_server->book_count; }

        public: SERVER_ID get_server_id() { return dl_server->server_id; }

        public: std::string get_out_dir() { return std::string(dl_server->out_dir); }

        public: std::vector<Book> get_books()
        {
            std::vector<Book> Books;
            Books.resize(dl_server->book_count);
            for (size_t index = 0; index < dl_server->book_count; index++)
                Books[index] = {dl_server->books[index].book_id, std::string(dl_server->books[index].book_name)};
            return Books;
        }

        // VARIABLES

        // Server
        private: HmcdServer* dl_server = nullptr;
    };
}

#endif /* HMCDPPD_HPP */