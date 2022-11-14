#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include <iostream>
#include "webserv.h"

class Response{
    private:
		std::string response;
		std::string request;
    public:
		Response();
		~Response();
		Response(std::string msg);

		int		readSocket(int new_fd);
		void	defaultResponse();
		char	*getResponse(void);
		void	writeResponse(int socket);

};

Response::Response(void){
}

Response::Response(std::string msg){
	response = msg;
}

Response::~Response(void){
}

void	Response::writeResponse(int socket){
	write(socket, response.c_str(), response.size());
}

void	Response::defaultResponse(){
	std::ifstream	html("srcs/html/index.html");
	std::string		line;
	response = 	"HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: ";
	while(std::getline(html, line)){
		std::cout << "debug" << std::endl;
		continue;
	}
	response.append(std::to_string(sizeof(line)));
	response.append("\n\n");
	response.append(line);
	std::cout << response << std::endl;
}

int		Response::readSocket(int new_fd){
	int reader;
	char *buffer;
	buffer = (char *)malloc(sizeof(char) * 2);
	buffer[1] = '\0';
	while ((reader = read(new_fd, buffer, 1)) > 0){
		request.append(buffer);
	}
	if (reader == -1)
		return (1);
	request += '\0';
	return (0);
}

#endif
