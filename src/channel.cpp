#include <algorithm>
#include <mutex>

#include <string.h>
#include <time.h>

#include <unistd.h>

#include "include/channel.hpp"
#include "include/constants.hpp"

Channel::Channel(const char *name_) 
{
    strncpy(name, name_, MAXLEN_CHANNEL_NAME);
}

void Channel::message(const char *sender, const char *message, Client *recipient)
{
    std::lock_guard<std::mutex> lock(mutex);
    char buffer[2*MAXLEN_MESSAGE];
    char time_buffer[16];
    time_t current_time;
    tm *time_struct;

    // 1. Get current time
    current_time = time(nullptr);
    time_struct = localtime(&current_time);
    strftime(time_buffer, sizeof(time_buffer), "%I:%M %p", time_struct);

    // 2. Create formatted message: "[<USERNAME> | <TIME>]: <MESSAGE>"
    snprintf(buffer, sizeof(buffer), "[%s | %s]: %s", sender, time_buffer, message);
    const size_t buflen = strlen(buffer) + 1;

    // 3. If there is a specific recipient, send the message to them. Else, send the message to everyone in the channel
    printf("[READ MESSAGE] user \"%s\" on channel \"%s\": \"%s\"\n", sender, name, buffer);
    if (recipient != nullptr)
    {
            printf("[WRITE MESSAGE] user \"%s\" => user \"%s\" on channel \"%s\": \"%s\"\n", sender, recipient->username, name, buffer);
            write(recipient->fd, buffer, buflen);
    }
    else
    {
        for (Client *channel_member : clients)
        {
            printf("[WRITE MESSAGE] user \"%s\" => user \"%s\" on channel \"%s\": \"%s\"\n", sender, channel_member->username, name, buffer);
            write(channel_member->fd, buffer, buflen);
        }
    }
}

void Channel::add_client(Client *client)
{
    std::lock_guard<std::mutex> lock(mutex);
    clients.push_back(client);

    printf("[CONNECTION] added client \"%s\" to channel \"%s\"\n", client->username, name);
}

void Channel::remove_client(Client *client)
{
    std::lock_guard<std::mutex> lock(mutex);
    std::remove(clients.begin(), clients.end(), client);

    printf("[CHANNEL] removed client \"%s\" from channel \"%s\"\n", client->username, name);
}