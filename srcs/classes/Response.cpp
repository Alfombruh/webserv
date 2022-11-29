#include "webserv.h"
#include "Response.hpp"
#include "unistd.h"

Response::Response(int clientId) : clientId((size_t)clientId) {}

Response::~Response() {}

void Response::clearReq()
{
	protocolVersion.clear();
	statusCode.clear();
	statusText.clear();
	headers.clear();
	body.clear();
}

// GETTERS - SETTERS
string Response::getHeader(string header)
{
	if (headers.find(header) != headers.end())
		return NULL;
	return headers.at(header);
};

size_t Response::getClientId() const { return clientId; };

// RESPONSES
void Response::text(const string msg)
{
	string headers = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: " + std::to_string(msg.length()) + "\n\n";
	
	headers += msg;
	cout << msg << "\n";
	write(clientId, headers.c_str(), headers.length());
};
