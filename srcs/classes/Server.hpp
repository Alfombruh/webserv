#ifndef SERVER_HPP
#define SERVER_HPP

#include "webserv.h"
#include <fstream>
#include <exception>
#include <netinet/in.h>

class Server
{
private:
    int sd; //main connection socket descriptor
    int new_socket; //the socket that makes the new connection
    int addr_len; //the lennght of the sockaddr_in struct
    struct sockaddr_in addr; //structure that holds the value of the connection port and ip
    // Config config;
    int    setupSelect(fd_set sd_set, int total[]);
public:
    Server(std::string is);
    int setup(void);
    int run(void);

    class serverException : public std::exception
    {
        char *msg;
    public:
        serverException(const char *msg);
        virtual const char *what() const throw();
    };
};

#endif