#pragma once

#include <string>
#include <unordered_set>
#include <mutex>

#include "database.hpp"
#include "channel.hpp"
#include "client.hpp"

enum class CommandError
{
    Success,
    MissingParameters,
    UsernameAlreadyExists,
    UsernameTooLong,
    Unknown,
};

class Command final
{
    public:
        static CommandError execute(Database *database, Channel *channel, Client *client, char *buffer);
        [[nodiscard]] static CommandError user(Database *database, Channel *channel, Client *client, char *buffer);
        [[nodiscard]] static CommandError quit(Database *database, Channel *chanel, Client *client, const char *args);
        static bool is_command(const char *message);
        static const char *error_message(CommandError error);
};