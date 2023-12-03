#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <unordered_set>
#include <thread>

#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <strings.h>

#include "include/error.hpp"
#include "include/constants.hpp"
#include "include/connection.hpp"
#include "include/channel.hpp"

int main()
{
    Channel channel("default channel");
    std::vector<std::thread> clients;
    std::unordered_set<std::string> usernames { "SERVER" };
    std::mutex usernames_mutex;
    sockaddr_in server_address, client_address;
    socklen_t client_length = sizeof(client_address);
    int listen_fd, connection_fd;

    // 1. Create a socket
    errchk( listen_fd = socket(AF_INET, SOCK_STREAM, 0), "socket");

    // 2. Bind server address to socket
    bzero(&server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(PORT);

    errchk( bind(listen_fd, (sockaddr*) &server_address, sizeof(server_address)), "bind");

    // 3. Listen for incoming connections
    errchk( listen(listen_fd, CONNECTION_BACKLOG), "listen");

    // 4. Accept incoming connections, blocking until one is received and spawning a handler thread per client
    for (;;)
    {
        errchk( connection_fd = accept(listen_fd, (sockaddr*) &client_address, &client_length), "accept");
        clients.push_back( std::thread(Connection::handle, std::ref(channel), std::ref(usernames), std::ref(usernames_mutex), connection_fd, client_address) );
    }

    // 5. Wait for clients to disconnect before exiting
    for (std::thread &client : clients)
    {
        client.join();
    }

    // 6. Close listening socket
    errchk( close(listen_fd), "close");

    return EXIT_SUCCESS;
}