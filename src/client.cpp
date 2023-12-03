#include <mutex>

#include <string.h>

#include <arpa/inet.h>

#include "include/constants.hpp"
#include "include/client.hpp"

Client::Client(FileDescriptor fd_, sockaddr_in socket_): socket(socket_), fd(fd_)
{
    exit = false;
    snprintf(username, MAXLEN_USER_NAME, "client-%d", fd);
}