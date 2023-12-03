#pragma once

#include <string>
#include <unordered_set>
#include <unordered_map>
#include <mutex>

#include "channel.hpp"

class Database final
{
    public:    
        Database();
        
        bool contains_username(const char *username);
        void add_username(const char *username);
        void remove_username(const char *username);

        bool contains_channel(const char *channel_name);
        Channel *get_channel(const char *channel_name);
        void add_channel(const char *channel_name);
        void remove_channel(const char *channel_name);

         /* Make Database objects non-copyable */
        Database(const Database&) = delete;
        Database& operator=(const Database&) = delete;

    private:
        std::unordered_set<std::string> usernames;
        std::mutex usernames_mutex;
    
        std::unordered_map<std::string,Channel> channels;
        std::mutex channels_mutex;
};