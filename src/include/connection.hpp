#pragma once

#include <arpa/inet.h>

#include "constants.hpp"
#include "channel.hpp"

class Connection final
{
    public:
        static void handle(Channel &channel, FileDescriptor fd, sockaddr_in socket);
};