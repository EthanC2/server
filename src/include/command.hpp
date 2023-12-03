#pragma once

#include <string>
#include <unordered_set>
#include <mutex>

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
        [[nodiscard]] static CommandError execute(Channel *channel, std::unordered_set<std::string> *usernames, std::mutex *usernames_mutex, Client *client, char *buffer);
        [[nodiscard]] static CommandError user(Channel *channel, std::unordered_set<std::string> *usernames, std::mutex *usernames_mutex, Client *client, char *buffer);
        [[nodiscard]] static CommandError quit(Channel *chanel, Client *client, const char *args);
        static const char *error_message(CommandError error);
};