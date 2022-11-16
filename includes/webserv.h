#ifndef WEBSERV_H
#define WEBSERV_H

#include "Response.hpp"

#define BUFFER_SIZE 1024
#define BACKLOG     10
#define PORT        8080

typedef struct s_webserv{
    int server_fd;
    int new_socket;
    int addr_len;
    int reader;
    struct sockaddr_in addr;
} t_webserv;

int init_server(t_webserv &webserv);

#endif
