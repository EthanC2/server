#pragma once

#include <vector>
#include <mutex>


#include "constants.hpp"
#include "client.hpp"

struct Channel final
{
    std::vector<Client*> clients;
    char name[MAX_CHANNEL_NAME];
    std::mutex mutex;

    Channel(const char *name_);
    void add_client(Client *client);
    void remove_client(Client *client);
    void message(const char *sender, const char *message, Client *recipient);

    /* Make Channel objects non-copyable */
    Channel() = delete;
    Channel(const Channel&) = delete;
    Channel& operator=(const Channel&) = delete;
};