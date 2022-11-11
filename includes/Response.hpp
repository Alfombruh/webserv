#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include <iostream>

class Response{
    private:
		std::string response;
		std::string request;
		Response();
		~Response();
    public:
		Response(std::string msg);

		void	readSocket(int new_fd);
		char	*getResponse(void);

};

Response::Response(std::string msg){
	response = msg;
}

Response::~Response(void){
}

void	Response::readSocket(__unused int new_fd){
	return;
}

#endif
