#include <string.h>

#include "include/command.hpp"

void Command::execute(Channel *channel, Client *client, char *buffer)
{
    const char *WHITESPACE = " \t\v\f\r\n";
    const char *command = strsep(&buffer, WHITESPACE) ?: "<null>";
    const char *args = buffer;

    printf("[COMMAND] user \"%s\" on channel \"%s\" executed \"%s\" with args \"%s\"\n", client->username, channel->name, command, args ?: "<null>");

    if (strcmp(command, "quit") == 0)
    {
        Command::quit(channel, client, args);
    }
}

void Command::quit(Channel *channel, Client *client, const char *args)
{
    const char *message = args ?: "left the server";

    channel->message(client, message);
    client->exit = true;
}