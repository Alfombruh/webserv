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
Server::~Server()
{
	for (ClientMap::iterator it = clients.begin(); it != clients.end(); it++)
	{
		delete it->second.first;
		delete it->second.second;
	}
}

void Server::acceptConnection(void)
{
	int client;
	struct sockaddr_in client_addr;
	int clientAddrLen = sizeof(client_addr);
	if ((client = accept(server, (struct sockaddr *)&client_addr, (socklen_t *)&clientAddrLen)) <= 0)
		throw(serverException("couldn't accept connection"));
	FD_SET(client, &current_set);
	clients.insert(make_pair(client, make_pair(new Request(client), new Response(client))));
	if (client > max_socket)
		max_socket = client;
}

void Server::handleConnection(int newClient)
{
	long valread;
	string rawReq;
	char buffer[30000] = {0};

	valread = read(newClient, buffer, 30000);
	for (size_t i = 0; buffer[i]; i++)
	{
		if (isprint(buffer[i]) || buffer[i] == '\n')
			rawReq.push_back(buffer[i]);
	}
	Router router(*clients.at(newClient).first, *clients.at(newClient).second);
	if (clients.at(newClient).first->parseRequest(rawReq) == FAILED)
	{
		string badReq = "HTTP/1.1 400 Bad Request\n\n";
		write(newClient, badReq.c_str(), badReq.length());
	}
	else
		handleRouting(router, newClient);
	// printf("------------------Hello message sent-------------------\n");
	// CLOSE-CLEAR CLIENT FROM SET AND MAP
	close(newClient);
	delete clients.at(newClient).first;
	delete clients.at(newClient).second;
	clients.erase(newClient);
	FD_CLR(newClient, &current_set);
}

void Server::handleRouting(Router &router, int client)
{
	string hello;
	if (router.use("/", &index))
		return;
	hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 13\n\n404 not found";
	write(client, hello.c_str(), hello.length());
}

int Server::setup(void)
{
	int opt = 1;

	timeout.tv_sec = TIMEOUT;
	timeout.tv_usec = 0;
	addr_len = sizeof(addr);
	if ((server = socket(AF_INET, SOCK_STREAM, 0)) <= 0)
		throw serverException("couldn't create socket");
	if (setsockopt(server, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) != 0) // works without it, just a good pratice
		throw serverException("couldn't change socket options");
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(PORT);
	if (bind(server, (struct sockaddr *)&addr, sizeof(addr)) < 0)
		throw serverException("not binding");
	if (listen(server, BACKLOG) < 0) // the second parameter is the amount of clients that can request connections
		throw serverException("not listening");
	return (0);
}

int Server::run()
{
	FD_ZERO(&current_set);
	FD_SET(server, &current_set);
	max_socket = server;
	while (1)
	{
		ready_set = current_set;
		if (select(max_socket + 1, &ready_set, NULL, NULL, &timeout) < 0)
			throw serverException("select couldnt be setup'd correctly");
		for (int i = 0; i <= max_socket; i++)
		{
			if (FD_ISSET(i, &ready_set) == 0)
				continue; // ionmi hace cosas raras
			if (i == server)
			{
				acceptConnection();
				continue; // las sigue haciendo
			}
			handleConnection(i);
		}
	}
}

Server::serverException::serverException(const char *msg) : msg((char *)msg){};
const char *Server::serverException::what() const throw()
{
	// return (strcat("Server Error: ", msg));
	return (msg);
};