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

static int error_message(std::string str){
	std::cerr << str << std::endl;
	return (1);
}

static int fill_webserv(t_webserv *webserv){
	webserv->addr_len = sizeof(webserv->addr);
	if ((webserv->server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
		return (error_message("Error: couldn't create socket"));
	webserv->addr.sin_family = AF_INET;
	webserv->addr.sin_addr.s_addr = INADDR_ANY;
	webserv->addr.sin_port = htons(PORT);
	if (bind(webserv->server_fd, (struct sockaddr *)&webserv->addr, sizeof(webserv->addr)) < 0)
		return (error_message("Error: couldn't bind"));
	if (listen(webserv->server_fd, BACKLOG) < 0)
		return (error_message("Error: not listening"));
	return (0);
}
/*
static int readSocket(t_webserv *webserv, Response *msg){
	ifstream is(webserv->new_socket);
	std::string hooka;

	is >> hooka;
	Response new(msg);
	return (0);
}*/

int main(__unused int argc,__unused char **argv){
	t_webserv 	webserv;
	Response	msg;
	//char		buffer[BUFFER_SIZE];
	//int 		reader;

	if (fill_webserv(&webserv) == 1)
		return (1);
	//__unused char msg[] = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
	while(1){
		if ((webserv.new_socket = accept(webserv.server_fd, (struct sockaddr *)&webserv.addr, (socklen_t *)&webserv.addr_len)) < 0)
			return(error_message("Error: couldn't accept package"));
		//if ((reader = read(webserv.new_socket, buffer, BUFFER_SIZE) < 0))
		//	return (error_message("Error: cannot read"));
		if (msg.readSocket(webserv.new_socket) == 1)
			return (error_message("Error: cannot read"));
	//	write(webserv.new_socket, msg, strlen(msg));
	//	write(1, buffer, strlen(buffer));
		msg.writeResponse(webserv.new_socket);
		close(webserv.new_socket);
	}
	return (0);
}
