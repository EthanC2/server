#pragma once

#include <mutex>

#include <arpa/inet.h>

#include "constants.hpp"

struct Client
{
    char username[MAX_USER_NAME];
    const sockaddr_in socket;
    const FileDescriptor fd;
    bool exit;
    std::mutex mutex;

    Client(FileDescriptor fd_, sockaddr_in socket_);

    /* Make Client objects non-copyable */
    Client() = delete;
    Client(const Client&) = delete;
    Client& operator=(const Client&) = delete;
};