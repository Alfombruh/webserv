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

using namespace std;

#define PORT 8080
#define SERVER_BACKLOG 10
#define SOCKETERROR -1
#define BUFSIZE 1025
#define SA_IN struct sockaddr_in
#define SA struct sockaddr

int setup_server(short port, int backlog);
int accept_new_connection(int server_socket);
int check(int exp, const char *msg);
void *handle_connection(int client_socket);

int main()
{
	int server_socket = setup_server(PORT, SERVER_BACKLOG);

	int max_socket_so_far = 0;
	fd_set current_sockets, ready_sockets;

	// initialize my current set
	FD_ZERO(&current_sockets);
	FD_SET(server_socket, &current_sockets);

	max_socket_so_far = server_socket;
	while (true)
	{
		printf("Waiting for connections...\n");
		// beacuse select is destructive
		ready_sockets = current_sockets;

		if (select(max_socket_so_far + 1, &ready_sockets, NULL, NULL, NULL) < 0)
		{
			perror("select error");
			exit(EXIT_FAILURE);
		}
		cout << "hola\n";
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

	} // while
	return 0;
}

int setup_server(short port, int backlog)
{
	SA_IN server_addr;
	int server_socket;
	check((server_socket = socket(AF_INET, SOCK_STREAM, 0)),
		  "Failed to create socket");

	// initialize the address struct
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(port);
	check(bind(server_socket, (SA*)&server_addr, sizeof(server_addr)),
		  "Bind Failed!");
	check(listen(server_socket, backlog),
		  "Listen Failed!");
	return server_socket;
}

int accept_new_connection(int server_socket)
{

	int addr_size = sizeof(SA_IN);
	int client_socket;
	SA_IN client_addr;
	check(client_socket = accept(server_socket, (SA *)&client_addr, (socklen_t *)&addr_size),
		  "accept failed");
	return client_socket;
}

int check(int exp, const char *msg)
{
	if (exp == SOCKETERROR)
	{
		perror(msg);
		exit(1);
	}
	return exp;
}

void *handle_connection(int client_socket)
{
	char buffer[BUFSIZE];
	size_t bytes_read;
	int msgsize = 0;
	char actualpath[PATH_MAX + 1];
	// read the client's messhge the name of the file-to read
	while ((bytes_read = read(client_socket, buffer + msgsize, sizeof(buffer) - msgsize - 1)) > 0)
	{
		msgsize += bytes_read;
		if (msgsize > BUFSIZE - 1 | buffer[msgsize - 1] == '\n')
			break;
	}

	check(bytes_read, "recv error");
	buffer[msgsize - 1] = 0; // null terminate  the message  and  remove  the  \n
	printf("REQUEST: %s\n", buffer);
	fflush(stdout);

	// validity check
	if (realpath(buffer, actualpath) == NULL)
	{
		printf("ERROR(bad_path): %s\n", buffer);
		close(client_socket);
		return NULL;
	}

	// read file and send its  contents  to client
	FILE *fp = fopen(actualpath, "r");
	if (fp == NULL)
	{
		printf("ERROR (open) :  %s\n", buffer);
		close(client_socket);
		return NULL;
	}

	// read file contents and send them to client
	// note this is a fine example program, but rather insecure.
	// a real- program-would probably limit the client to certain files,
	while ((bytes_read = fread(buffer, 1, BUFSIZE, fp)) > 0)
	{
		// printf("sending %zu bytes\n", bytes_read) ;
		write(client_socket, buffer, bytes_read);
	}
	close(client_socket);
	fclose(fp);
	printf("closing connection\n");
	return NULL;
}