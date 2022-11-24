#include "webserv.h"
#include "Server.hpp"
#include <fstream>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>

Server::Server(string path)
{
    (void)path;
    // config.parse(path);
}

int Server::setupSelect(fd_set sd_set, int total[MAX_CONNECTIONS])
{
    struct timeval tv = {.tv_sec = TIMEOUT, .tv_usec = 0};

    FD_ZERO(&sd_set); // sets the fd set to 0 b4 starting to work with it, it get initialized
    for (int i = 0; i < MAX_CONNECTIONS; i++)
    {
        if (total[i] >= 0)
            FD_SET(total[i], &sd_set);
    }
    return (select(FD_SETSIZE, &sd_set, NULL, NULL, &tv)); // last parameter is timeout, have to change it
}

int Server::setup(void)
{
    addr_len = sizeof(addr);
    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
        throw serverException("couldn't create socket");
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);
    if (bind(sd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
        throw serverException("not binding");
    if (listen(sd, BACKLOG) < 0) // the second parameter is the amount of clients that can request connections
        throw serverException("not listening");
    return (0);
}

int Server::run()
{
    fd_set sd_set;
    int totalConnections[MAX_CONNECTIONS];
    int reader;
    char *buffer = NULL;
    char msg[] = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";

    for (int i = 0; i < MAX_CONNECTIONS; i++)
        totalConnections[i] = -1;
    FD_ZERO(&sd_set);
    FD_SET(sd, &sd_set);
    for (int i=0; i < MAX_CONNECTIONS; i++)
        FD_SET(totalConnections[i], &sd_set); // it BREAKS THE CODE HERE
    cout << "reaches this \n";
    exit (1);
    totalConnections[0] = this->sd;
    while (1)
    {
        if (setupSelect(sd_set, totalConnections) < 0)
            throw serverException("select couldnt be setup'd correctly");
        if ((new_socket = accept(sd, (struct sockaddr *)&addr, (socklen_t *)&addr_len)) < 0)
            throw serverException("couldn't accept package");
        if ((reader = read(new_socket, buffer, BUFFER_SIZE) < 0))
            throw serverException("cannot read");
        write(new_socket, msg, strlen(msg));
        write(1, buffer, strlen(buffer));
        close(new_socket);
    }
}

Server::serverException::serverException(const char *msg) : msg((char *)msg){};
const char *Server::serverException::what() const throw()
{
    // return (strcat("Server Error: ", msg));
    return (msg);
};