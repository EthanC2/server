#pragma once

#include <string>
#include <unordered_set>
#include <mutex>

class Database final
{
    public:    
        Database();
        bool contains_username(const char *username);
        void add_username(const char *username);
        void remove_username(const char *username);

         /* Make Database objects non-copyable */
        Database(const Database&) = delete;
        Database& operator=(const Database&) = delete;

    private:
        std::unordered_set<std::string> usernames;
        std::mutex usernames_mutex;
};