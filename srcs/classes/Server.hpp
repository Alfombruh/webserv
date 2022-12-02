#ifndef SERVER_HPP
#define SERVER_HPP

#include "webserv.h"
#include "Request.hpp"
#include "Response.hpp"
#include "Router.hpp"
#include <fstream>
#include <exception>
#include <netinet/in.h>
#include "../../routes/routes.h"

using std::make_pair;

class Request;
class Response;
typedef std::map<int, std::pair<Request *, Response *> > ClientMap;

class Router;

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
	ClientMap clients;
public:
    Server(std::string is);
    ~Server();
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