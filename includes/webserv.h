#ifndef WEBSERV_H
#define WEBSERV_H

#include <iostream>
#include <map>

typedef std::map<std::string, std::string> StrStrMap;

#include "../srcs/classes/Server.hpp"
#include "../srcs/classes/Request.hpp"
#include "../srcs/classes/Response.hpp"

#define MAX_CONNECTIONS 1000
#define TIMEOUT         60
#define BUFFER_SIZE     1024
#define BACKLOG         10
#define PORT            8080
#define FAILED          false

using std::string;
using std::cout;


typedef struct s_webserv{
    int server_fd;
    int new_socket;
    int addr_len;
    int reader;
    struct sockaddr_in addr;
} t_webserv;

int init_server(t_webserv &webserv);
int errorMessage(std::string msg);

#endif
