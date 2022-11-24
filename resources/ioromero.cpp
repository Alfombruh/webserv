#include <stdio.h>
#include <string.h> //strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>	   //close
#include <arpa/inet.h> //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include <iostream>
​
using namespace std;
​
#define PORT 8080
#define SERVER_BACKLOG 10
#define SOCKETERROR 0
#define BUFSIZE 1025
#define SA_IN struct sockaddr_in
#define SA struct sockaddr
​
int setup_server(short port, int backlog);
int accept_new_connection(int server_socket);
int check(int exp, const char *msg);
void *handle_connection(int client_socket);
​
int main()
{
	int server_socket = setup_server(PORT, SERVER_BACKLOG);
​
	int max_socket_so_far = 0;
	fd_set current_sockets, ready_sockets;
​
	// initialize my current set
	FD_ZERO(&current_sockets);
	FD_SET(server_socket, &current_sockets);
​
	max_socket_so_far = server_socket;
	while (true)
	{
		printf("Waiting for connections...\n");
		// beacuse select is destructive
		ready_sockets = current_sockets;
​
		// if (select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL) < 0)
		if (select(max_socket_so_far + 1, &ready_sockets, NULL, NULL, NULL) < 0)
		{
			perror("select error");
			exit(EXIT_FAILURE);
		}
		// cout << "hola\n";
		// for (int i = 0; i < FD_SETSIZE; i++)
		for (int i = 0; i <= max_socket_so_far; i++)
		{
			if (FD_ISSET(i, &ready_sockets))
			{
				if (i == server_socket)
				{
					// this is a new connection
					int client_socket = accept_new_connection(server_socket);
					FD_SET(client_socket, &current_sockets);
					if (client_socket > max_socket_so_far)
						max_socket_so_far = client_socket;
				}
				else
				{
					// do whatever we do with connections.
					handle_connection(i);
					FD_CLR(i, &current_sockets);
				}
			}
		}
​
	} // while
	return 0;
}
​
int setup_server(short port, int backlog)
{
	int opt = 1;
	SA_IN server_addr;
	int server_socket;
	check((server_socket = socket(AF_INET, SOCK_STREAM, 0)),
		  "Failed to create socket");
	// set master socket to allow multiple connections ,
	// this is just a good habit, it will work without this
	check(setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)),
		  "setsockopt");
​
	// initialize the address struct
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(port);
	memset(server_addr.sin_zero, '\0', sizeof server_addr.sin_zero);
​
	check(bind(server_socket, (SA *)&server_addr, sizeof(server_addr)),
		  "Bind Failed!");
	check(listen(server_socket, backlog),
		  "Listen Failed!");
	return server_socket;
}
​
int accept_new_connection(int server_socket)
{
​
	int addr_size = sizeof(SA_IN);
	int client_socket;
	SA_IN client_addr;
	check(client_socket = accept(server_socket, (SA *)&client_addr, (socklen_t *)&addr_size),
		  "accept failed");
	return client_socket;
}
​
int check(int exp, const char *msg)
{
	if (exp < SOCKETERROR)
	{
		perror(msg);
		exit(1);
	}
	return exp;
}
​
void *handle_connection(int client_socket)
{
	long valread;
	string hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
​
	char buffer[30000] = {0};
	valread = read(client_socket, buffer, 30000);
	printf("%s\n", buffer);
	write(client_socket, hello.c_str(), hello.length());
	printf("------------------Hello message sent-------------------\n");
	close(client_socket);
	return NULL;
}