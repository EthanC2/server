#include <string.h>

#include <unistd.h>

#include "include/constants.hpp"
#include "include/database.hpp"
#include "include/channel.hpp"
#include "include/command.hpp"

static const char *WHITESPACE = " \t\v\f\r\n";

CommandError Command::execute(Database *database, Channel *channel, Client *client, char *buffer)
{
    const char *command = strsep(&buffer, WHITESPACE) ?: "<null>";
    char *args = buffer;

    printf("[COMMAND] user \"%s\" on channel \"%s\" executed \"%s\" with args \"%s\"\n", client->username, channel->name, command, args ?: "<null>");

    if (strcmp(command, "user") == 0)
    {
        return Command::user(database, channel, client, args);
    }
    else if (strcmp(command, "quit") == 0)
    {
        return Command::quit(database, channel, client, args);
    }

    channel->message("SERVER", Command::error_message(CommandError::Unknown), client);
    return CommandError::Unknown;
}

CommandError Command::user(Database *database, Channel *channel, Client *client, char *buffer)
{
    const char *_ = strsep(&buffer, WHITESPACE);
    const char *username = strsep(&buffer, WHITESPACE);
    const char *hostname = strsep(&buffer, WHITESPACE);
    const char *server_name = strsep(&buffer, WHITESPACE);
    const char *real_name = strsep(&buffer, WHITESPACE);
    char response[MAXLEN_MESSAGE];

    printf("[DEBUG | COMMAND | USER] username: %s, hostname: %s, server_name: %s, real_name: %s\n", username, hostname, server_name, real_name);
    
    // 1. Validate that all required arguments were provided
    if (username == nullptr or hostname == nullptr or server_name == nullptr or real_name == nullptr)
    {
        channel->message("SERVER", Command::error_message(CommandError::MissingParameters), client);
        return CommandError::MissingParameters;
    }

    // 2. Validate that username is not too long
    if (strlen(username) > MAXLEN_USER_NAME)
    {
        channel->message("SERVER", Command::error_message(CommandError::UsernameTooLong), client);
        return CommandError::UsernameTooLong;
    }

    // 3. Validate that the username does not already exist
    if (database->contains_username(username))
    {
        channel->message("SERVER", Command::error_message(CommandError::UsernameAlreadyExists), client);
        return CommandError::UsernameAlreadyExists;
    }

    // 4. Log that the client's username was updated
    printf("[COMMAND | USER] \"%s\" set their username to \"%s\"\n", client->username, username);
    snprintf(response, sizeof(response), "successfully changed username to \"%s\"", username);
    channel->message("SERVER", response, client);

    // 5. Change the client's identification info
    database->remove_username(client->username);
    strncpy(client->username, username, MAXLEN_USER_NAME);
    strncpy(client->hostname, hostname, MAXLEN_HOST_NAME);
    strncpy(client->server_name, server_name, MAXLEN_SERVER_NAME);
    strncpy(client->real_name, real_name, MAXLEN_REAL_NAME);
    database->add_username(username);

    return CommandError::Success;
}

CommandError Command::quit(Database *database, Channel *channel, Client *client, const char *args)
{
    const char *message = args ?: "a user left the server";

    channel->message("SERVER", message, nullptr);
    database->remove_username(client->username);
    client->exit = true;

    return CommandError::Success;
}

bool Command::is_command(const char *message)
{
    return message != nullptr and message[0] == '/';
}

const char *Command::error_message(CommandError error)
{
    static const char *error_messages[] = {
        [static_cast<int>(CommandError::Success)] = "error: success",
        [static_cast<int>(CommandError::MissingParameters)] = "error: missing parameters",
        [static_cast<int>(CommandError::UsernameAlreadyExists)] = "error: username already exists",
        [static_cast<int>(CommandError::UsernameTooLong)] = "error: username too long (max length: 9 characters)",
        [static_cast<int>(CommandError::Unknown)] = "error: unknown error"
    };

    return error_messages[static_cast<int>(error)];
}