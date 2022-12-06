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
	for (size_t i = 8080; i < 8084; i++)
	{
		ports.push_back(i);
	}
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

void Server::acceptConnection(int newClient)
{
	int client;
	struct sockaddr_in client_addr;
	int clientAddrLen = sizeof(client_addr);
	if ((client = accept(newClient, (struct sockaddr *)&client_addr, (socklen_t *)&clientAddrLen)) <= 0)
		throw(serverException("couldn't accept connection"));
	FD_SET(client, &current_set);
	clients.insert(make_pair(client, make_pair(new Request(client), new Response(client))));
	if (client > max_socket)
		max_socket = client;
}

int Server::setup(void)
{
	int opt = 1;

	timeout.tv_sec = TIMEOUT;
	timeout.tv_usec = 0;
	FD_ZERO(&current_set);
	for (IntVec::iterator it = ports.begin(); it != ports.end(); it++)
	{
		portStruct temp;
		temp.addr_len = sizeof(temp.addr);
		if ((temp.server = socket(AF_INET, SOCK_STREAM, 0)) <= 0)
			throw serverException("couldn't create socket");
		if (setsockopt(temp.server, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) != 0) // works without it, just a good pratice
			throw serverException("couldn't change socket options");
		temp.addr.sin_family = AF_INET;
		temp.addr.sin_addr.s_addr = INADDR_ANY;
		temp.addr.sin_port = htons(*it);
		if (bind(temp.server, (struct sockaddr *)&temp.addr, sizeof(temp.addr)) < 0)
			throw serverException("not binding");
		if (listen(temp.server, BACKLOG) < 0) // the second parameter is the amount of clients that can request connections
			throw serverException("not listening");
		FD_SET(temp.server, &current_set);
		portData.push_back(temp);
	}
	return (0);
}

int Server::run()
{
	max_socket = portData.back().server;
	while (1)
	{
		ready_set = current_set;
		if (select(max_socket + 1, &ready_set, NULL, NULL, &timeout) < 0)
			throw serverException("select couldnt be setup'd correctly");
		for (int i = 0; i <= max_socket; i++)
		{
		handle:
			if (FD_ISSET(i, &ready_set) == 0)
				continue;
			for (std::vector<portStruct>::iterator it = portData.begin(); it != portData.end(); *it++)
			{
				if (i == it->server)
				{
					acceptConnection(i++);
					goto handle;
				}
			}
			handleConnection(i);
		}
	}
}

void Server::handleConnection(int client)
{
	if (clients.at(client).first->readRequest(client, *clients.at(client).second) == REQ_PARSED)
	{
		Router router(*clients.at(client).first, *clients.at(client).second);
		router.use("/", &index);
	}

	// CLOSE-CLEAR CLIENT FROM SET AND MAP
	close(client);
	delete clients.at(client).first;
	delete clients.at(client).second;
	clients.erase(client);
	FD_CLR(client, &current_set);
}

Server::serverException::serverException(const char *msg) : msg((char *)msg){};
const char *Server::serverException::what() const throw()
{
	// return (strcat("Server Error: ", msg));
	return (msg);
};