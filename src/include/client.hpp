#pragma once

#include <mutex>

#include <arpa/inet.h>

#include "constants.hpp"

struct Client final
{
    char username[MAXLEN_USER_NAME];
    char hostname[MAXLEN_HOST_NAME];
    char server_name[MAXLEN_SERVER_NAME];
    char real_name[MAXLEN_REAL_NAME];
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