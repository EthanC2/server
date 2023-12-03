#pragma once

using FileDescriptor = int;

const int PORT {25565};
const int CONNECTION_BACKLOG {3};
const int MAXLEN_MESSAGE{512};
const int MAXLEN_CHANNEL_NAME {200};

const int MAXLEN_USER_NAME {9};
const int MAXLEN_HOST_NAME {256};
const int MAXLEN_SERVER_NAME {256};
const int MAXLEN_REAL_NAME {256};

//const char *WHITESPACE = " \t\v\f\r\n";