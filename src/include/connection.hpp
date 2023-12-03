#pragma once

#include <string>
#include <unordered_set>
#include <mutex>

#include <arpa/inet.h>

#include "constants.hpp"
#include "database.hpp"
#include "channel.hpp"

class Connection final
{
    public:
        static void handle(Database &database, Channel &channel, FileDescriptor fd, sockaddr_in socket);
};