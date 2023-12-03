#pragma once

using FileDescriptor = int;

#define PORT 25565
#define CONNECTION_BACKLOG 3
#define DEFAULT_CHANNEL_NAME "#default"
#define SERVER_USERNAME "SERVER"
#define WHITESPACE " \t\v\f\r\n"

#define MAXLEN_MESSAGE 512
#define MAXLEN_CHANNEL_NAME 200
#define MAXLEN_USER_NAME 9
#define MAXLEN_HOST_NAME 256
#define MAXLEN_SERVER_NAME 256
#define MAXLEN_REAL_NAME 256