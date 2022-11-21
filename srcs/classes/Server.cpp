#include "webserv.h"
#include <fstream>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

Server::Server(string path)
{
    (void)path;
    // config.parse(path);
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
    if (listen(sd, BACKLOG) < 0)
         throw serverException("not listening");
    return (0);
}

int Server::run()
{
    char msg[] = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
    while (1)
    {
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