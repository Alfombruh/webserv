#ifndef WEBSERV_H
#define WEBSERV_H

#include <iostream>
#include <map>
#include <vector>
#include <sstream>
#include "status_codes.h"


typedef std::map<std::string, std::string> StrStrMap;
typedef std::vector<int> IntVec;
typedef std::vector<std::string> StrVec;
typedef std::pair<std::string, std::string> StrPair;

using std::cout;
using std::string;

enum METHOD
{
	GET,
	POST,
	DELETE
};

typedef struct
{
	string location;
	std::vector<METHOD> alowedMethods;
	string index;
	string root;
	string api;
	string destination;
	StrPair cgiInfo;

} Location;

typedef struct
{
	bool redirect;
	string status;
	string route;
} Redirection;

inline size_t stringToSize_t(string str)
{
	std::stringstream sstream(str);
	size_t result;
	sstream >> result;
	return result;
}

#include "../srcs/classes/Request.hpp"
#include "../srcs/classes/Response.hpp"
#include "../srcs/classes/Server.hpp"
#include "../srcs/classes/Router.hpp"
#include "../srcs/classes/config.hpp"
extern Config configuration;

#define MAX_CONNECTIONS 1000
#define TIMEOUT 60
#define BUFFER_SIZE 1024
#define BACKLOG 10
#define PORT 8080
#define HOST "Aisha"
#define FAILED false
#define REQ_PARSED true
#define MAX_BODY 100000

typedef struct s_webserv
{
	int server_fd;
	int new_socket;
	int addr_len;
	int reader;
	struct sockaddr_in addr;
} t_webserv;

int init_server(t_webserv &webserv);
int errorMessage(std::string msg);

#endif
