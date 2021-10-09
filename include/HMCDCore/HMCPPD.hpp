#ifndef HMCDPPD_HPP
#define HMCDPPD_HPP

/**
 * @file include/HMCDCore/HMCPPD.hpp
 * @brief C++ namespace and class wrapper for HMCD.h
*/
#include "HMCD.h"

#include <cstddef>
#include <cstdint>
#include <cstdio>

#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>

/**
 * @brief C++ wrapper of HMCD
*/
namespace hmcppd
{
    /**
     * @brief Server designation
    */
    typedef HMCD_SERVER_ID SERVER_ID;

    /**
     * @brief Structure representing a book, used for book lists
    */
    typedef struct _Book
    {
        std::uint32_t book_id;   //!< ID of the book starting at 1001
        std::string book_name;  //!< Name of the book, may be unicode
    }Book;

    /**
     * @brief Wrapper around HmcdServer
    */
    class DlServer
    {
        // PUBLIC FUNCTIONS

        /**
         * @brief Basic constructor, throws std::runtime_error, if unrecognized
        */
        public: DlServer(SERVER_ID server_id)
        {
            if (server_id == SERVER_ID::HMCD_CHINA)
                dl_server = &HMCD_CN_SERVER;
            else if (server_id == SERVER_ID::HMCD_GLOBAL)
                dl_server = &HMCD_GLB_SERVER;
            else throw std::runtime_error("Unrecognized server_id = " + std::to_string(server_id) + "\n");
        }

        public: DlServer() = delete;

        /**
         * @brief Does nothing, only here because of smart pointers needing a destructor definition
        */
        public: ~DlServer()
        {
            ;
        }

        /**
         * @brief Enable logs to stdout
         * @param enable True to enable
        */
        public: void enable_logs(bool enable) { hmcd_enable_logs(enable); }

        /**
         * @brief Check if logs are enabled
         * @return True if enabled
        */
        public: bool enabled_logs() { return hmcd_enabled_logs(); }

        /**
         * @brief Get server designation, throws std::runtime_error, if member variable is somehow uninitialized
         * @return Designation string
        */
        public: std::string get_server_name()
        {
            const char* server_name = hmcd_get_server_name(dl_server->server_id);
            if (!server_name)
                throw std::runtime_error(
                    "get_server_name() failed, unrecognized server_id = " + std::to_string(dl_server->server_id) + "\n");
            return std::string(server_name);
        }

        /**
         * @brief Returns directory size
         * @param dir_name Directory path
         * @return Total size
        */
        public: std::uint64_t get_dir_size(const std::string& dir_name)
            { return static_cast<std::uint64_t>(hmcd_get_dir_size(dir_name.c_str())); }

        /**
         * @brief Get number of chapter, throws std::runtime_error on fail
         * @param book_index index of books array, throws std::invalid_argument if >= get_book_count()
         * @return Number of chapters
        */
        public: unsigned int get_chap_cnt(unsigned int book_index)
        {
            if (book_index >= dl_server->book_count)
                throw std::invalid_argument("book_index >= get_book_count()\n");
            unsigned int chap_count = hmcd_get_chap_cnt(dl_server, book_index);
            if (chap_count == 0)
                throw std::runtime_error("get_chap_cnt() failed\n");
            return chap_count;
        }

        /**
         * @brief Downloads manhua
         * @param book_index index of target_server->books, throws std::invalid_argument if >= get_book_count()
         * @param first_chap First chapter to download
         * @param last_chap Last chapter to download
         * @param one_big_dir Download every page in one directory
        */
        public: void dl_book(unsigned int book_index, unsigned int first_chap, unsigned int last_chap, bool one_big_dir)
        {
            if (book_index >= dl_server->book_count)
                throw std::invalid_argument("book_index >= get_book_count()\n");

            bool previous_state = enabled_logs();
            enable_logs(false);
            unsigned int chapter_count = get_chap_cnt(book_index);
            enable_logs(previous_state);

            if (last_chap > chapter_count)
                throw std::invalid_argument("last_chap >= get_chap_count(book_index)\n");

            int result = hmcd_dl_book(dl_server, book_index, first_chap, last_chap, one_big_dir);
            if (result != 0)
                throw std::runtime_error("dl_book() failed\n");
        }

        /**
         * @brief Change from china to global or global to china, @n
         * throws std::runtime_error if current server_id is neither SERVER_ID::HMCD_CHINA or SERVER_ID::HMCD_GLOBAL
        */
        public: void switch_server()
        {
            switch (dl_server->server_id)
            {
                case SERVER_ID::HMCD_CHINA:
                    dl_server = &HMCD_GLB_SERVER;
                    break;
                case SERVER_ID::HMCD_GLOBAL:
                    dl_server = &HMCD_CN_SERVER;
                    break;
                default:
                    throw std::runtime_error("Unrecognized server_id = " + std::to_string(dl_server->server_id) + "\n");
                    break;
            }
        }

        // GETTERS - SETTERS

        /**
         * @brief Number of books on the current server
        */
        public: std::uint32_t get_book_count() { return dl_server->book_count; }

        /**
         * @brief Current server designation
        */
        public: SERVER_ID get_server_id() { return dl_server->server_id; }

        /**
         * @brief Get output directory of downloads
        */
        public: std::string get_out_dir() { return std::string(dl_server->out_dir); }

        /**
         * @brief Get vector of Books
        */
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
        private: const HmcdServer* dl_server = nullptr;
    };
}

#endif /* HMCDPPD_HPP */