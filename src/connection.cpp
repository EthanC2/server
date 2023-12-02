#include <algorithm>
#include <mutex>

#include <cstdio>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#include "include/constants.hpp"
#include "include/connection.hpp"
#include "include/command.hpp"
#include "include/client.hpp"
#include "include/error.hpp"

void Connection::handle(Channel &channel, FileDescriptor fd, sockaddr_in socket)
{
    Client client(fd, socket);
    char message[MAX_MESSAGE_LENGTH];
    ssize_t nread;

    // 1. Register new connection by adding them to the default room
    channel.add_client(&client);

    // 2. Prompt the client to change their username
    do
    {
        channel.message("SERVER", "please use the /user command to select a username", &client);
        channel.message("SERVER", "syntax: /user <username> <hostname> <servername> <realname>", &client);
        read(client.fd, message, MAX_MESSAGE_LENGTH);
    } while (Command::user(&channel, &client, message) != CommandError::Success);
    
    
    // 3. Listen to incoming messages from the client and write them to the channel
    while (not client.exit and (nread = read(client.fd, message, MAX_MESSAGE_LENGTH)) > 0)
    {
        if (message[0] == '/')
        {
            Command::execute(&channel, &client, message + 1);
        }
        else
        {
            channel.message(client.username, message, nullptr);
        }
    }

    // 4. Register closed connection by removing the them from the channel
    channel.remove_client(&client);

    errchk( close(client.fd), "close" );
}