#pragma once

#include "channel.hpp"
#include "client.hpp"

class Command
{
    public:
        static void execute(Channel *channel, Client *client, char *buffer);
};