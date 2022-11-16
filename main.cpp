#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "includes/webserv.h"

static int error_message(std::string str)
{
	std::cerr << str << std::endl;
	return (1);
}


int main(__unused int argc, __unused char **argv)
{
	t_webserv webserv;
    memset(&webserv);

	if (init_server(&webserv) == 1)
		return (1);
	__unused char msg[] = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
	while (1)
	{
		if ((webserv.new_socket = accept(webserv.server_fd, (struct sockaddr *)&webserv.addr, (socklen_t *)&webserv.addr_len)) < 0)
			return (error_message("Error: couldn't accept package"));
		if ((reader = read(webserv.new_socket, buffer, BUFFER_SIZE) < 0))
			return (error_message("Error: cannot read"));
		write(webserv.new_socket, msg, strlen(msg));
		write(1, buffer, strlen(buffer));
		close(webserv.new_socket);
	}
	return (0);
}
