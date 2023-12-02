#pragma once

#include "channel.hpp"
#include "client.hpp"

enum class CommandError
{
    Success,
    MissingParameters,
    UsernameAlreadyExists,
    Unknown,
};

class Command
{
    public:
        [[nodiscard]] static CommandError execute(Channel *channel, Client *client, char *buffer);
        [[nodiscard]] static CommandError user(Channel *channel, Client *client, char *buffer);
        [[nodiscard]] static CommandError quit(Channel *chanel, Client *client, const char *args);
        static const char *error_message(CommandError error);
};