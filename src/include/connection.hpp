#pragma once

#include <string>
#include <unordered_set>
#include <mutex>

#include <arpa/inet.h>

#include "constants.hpp"
#include "channel.hpp"

class Connection final
{
    public:
        static void handle(Channel &channel, std::unordered_set<std::string> &usernames, std::mutex &usernames_mutex, FileDescriptor fd, sockaddr_in socket);
};