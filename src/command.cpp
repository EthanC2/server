#include <string.h>

#include <unistd.h>

#include "include/constants.hpp"
#include "include/command.hpp"

static const char *WHITESPACE = " \t\v\f\r\n";

CommandError Command::execute(Channel *channel, std::unordered_set<std::string> *usernames, std::mutex *usernames_mutex, Client *client, char *buffer)
{
    const char *command = strsep(&buffer, WHITESPACE) ?: "<null>";
    char *args = buffer;

    printf("[COMMAND] user \"%s\" on channel \"%s\" executed \"%s\" with args \"%s\"\n", client->username, channel->name, command, args ?: "<null>");

    if (strcmp(command, "quit") == 0)
    {
        return Command::quit(channel, client, args);
    }
    else if (strcmp(command, "user") == 0)
    {
        return Command::user(channel, usernames, usernames_mutex, client, args);
    }

    return CommandError::Unknown;
}

CommandError Command::user(Channel *channel, std::unordered_set<std::string> *usernames, std::mutex *usernames_mutex, Client *client, char *buffer)
{
    std::lock_guard<std::mutex> lock(*usernames_mutex);
    const char *_ = strsep(&buffer, WHITESPACE);
    const char *username = strsep(&buffer, WHITESPACE);
    const char *hostname = strsep(&buffer, WHITESPACE);
    const char *server_name = strsep(&buffer, WHITESPACE);
    const char *real_name = strsep(&buffer, WHITESPACE);
    char response[2*MAX_USER_NAME];

    printf("[COMMAND | USER] username: %s, hostname: %s, server_name: %s, real_name: %s", username, hostname, server_name, real_name);

    if (username == nullptr or hostname == nullptr or server_name == nullptr or real_name == nullptr)
    {
        const char *errmsg = Command::error_message(CommandError::MissingParameters);
        channel->message("SERVER", errmsg, client);
        return CommandError::MissingParameters;
    }

    if (usernames->find(std::string(username)) != usernames->end())
    {
        const char *errmsg = Command::error_message(CommandError::UsernameAlreadyExists);
        channel->message("SERVER", errmsg, client);
        return CommandError::UsernameAlreadyExists;
    }

    printf("[COMMAND | USER] \"%s\" set their username to \"%s\"\n", client->username, username);
    snprintf(response, 2*MAX_USER_NAME, "successfully changed username to \"%s\"", username);
    channel->message("SERVER", response, client);

    usernames->erase(std::string(username));
    strncpy(client->username, username, MAX_USER_NAME);
    usernames->insert(std::string(username));

    return CommandError::Success;
}

CommandError Command::quit(Channel *channel, Client *client, const char *args)
{
    const char *message = args ?: "a user left the server";

    channel->message("SERVER", message, nullptr);
    client->exit = true;

    return CommandError::Success;
}

const char *Command::error_message(CommandError error)
{
    static const char *error_messages[] = {
        [static_cast<int>(CommandError::Success)] = "error: success",
        [static_cast<int>(CommandError::MissingParameters)] = "error: missing parameters",
        [static_cast<int>(CommandError::UsernameAlreadyExists)] = "error: username already exists",
        [static_cast<int>(CommandError::Unknown)] = "error: unknown error"
    };

    return error_messages[static_cast<int>(error)];
}