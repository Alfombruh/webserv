#ifndef SERVER_HPP
#define SERVER_HPP

#include "webserv.h"
#include <fstream>
#include <exception>
#include <netinet/in.h>

class Server
{
private:
    //**** sockets ****
    int server; //main connection socket descriptor
    //int client; //client's new socket
    int max_socket; //maximun number of the fd_set
    //**** fd_set's ****
    fd_set  current_set;
    fd_set  ready_set;
    //**** structure that holds the informations about the socket we are using ********
    int addr_len; //the lennght of the sockaddr_in struct
    struct sockaddr_in addr; //structure that holds the value of the connection port and ip
    //****
    struct timeval timeout;
    //**** private functions ********************************
    void    acceptConnection(void);
    void    handleConnection(int newClient);
    //****
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