#include <string>
#include <unordered_set>
#include <mutex>

#include <string.h>

#include "include/constants.hpp"
#include "include/database.hpp"

Database::Database()
{
    add_username(SERVER_USERNAME);
    add_channel(DEFAULT_CHANNEL_NAME);
}

bool Database::contains_username(const char *username)
{
    std::unique_lock<std::mutex> lock(usernames_mutex);
    return usernames.find(username) != usernames.end();
}

void Database::add_username(const char *username)
{
    std::unique_lock<std::mutex> lock(usernames_mutex);
    usernames.insert(username);
}

void Database::remove_username(const char *username)
{
    std::unique_lock<std::mutex> lock(usernames_mutex);
    usernames.erase(username);
}

bool Database::contains_channel(const char *channel_name)
{
    std::unique_lock<std::mutex> lock(channels_mutex);
    return channels.find(channel_name) != channels.end();
}

Channel *Database::get_channel(const char *channel_name)
{
    std::unique_lock<std::mutex> lock(channels_mutex);

    if (channels.find(channel_name) == channels.end())
    {
        channels.emplace(channel_name, channel_name);
    }

    return &channels.at(channel_name);
}

void Database::add_channel(const char *channel_name)
{
    std::unique_lock<std::mutex> lock(channels_mutex);
    channels.emplace(channel_name, channel_name);
}

void Database::remove_channel(const char *channel_name)
{
    std::unique_lock<std::mutex> lock(channels_mutex);
    channels.erase(channel_name);
}