#include <string.h>

#include "include/command.hpp"

void Command::execute(Channel *channel, Client *client, char *buffer)
{
    const char *WHITESPACE = " \t\v\f\r\n";
    const char *command = strsep(&buffer, WHITESPACE) ?: "<null>";
    const char *args = strsep(&buffer, WHITESPACE) ?: "<null>";

    printf("[COMMAND] user \"%s\" on channel \"%s\" executed \"%s\" with args \"%s\"\n", client->username, channel->name, command, args);

    if (strcmp(command, "quit") == 0)
    {
        client->exit = true;
    }
}