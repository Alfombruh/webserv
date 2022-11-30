#include "webserv.h"
#include "Response.hpp"

Response::Response(int clientId) : clientId((size_t)clientId) {}

string Response::protocolVersion = "HTTP/1.1";

Response::~Response() {}

string Response::stringifyResponse()
{
	if (stringStatus.empty())
		return NULL;
	// STATUS LINE
	string response = (protocolVersion + " " + stringStatus + "\n");
	// HEADERS
	for (std::vector<string>::iterator it = headers.begin(); it != headers.end(); ++it)
		response += (*it + "\n");
	response += "\n";
	// BODY
	if (!body.empty())
		response += (body + "\n");
	return response;
};

void Response::clearResponse()
{
	stringStatus.clear();
	headers.clear();
	body.clear();
};

// GETTERS - SETTERS

size_t Response::getClientId() const { return clientId; };

// RESPONSES
// #define ERROR(status) (!(status > 100))

Response &Response::status(const string status)
{
	stringStatus = status;
	return *this;
};

Response &Response::text(const string &msg)
{
	headers.push_back("Content-Type: text/plain");
	headers.push_back("Content-Length: " + std::to_string(msg.length()));
	body = msg;

	// string headers = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: " + std::to_string(msg.length()) + "\n\n";
	// headers += msg;
	return *this;
};

void Response::send()
{
	cout << "-------------res-------------\n";
	cout << stringifyResponse();
	cout << "-----------------------------\n";
	write(clientId, stringifyResponse().c_str(), stringifyResponse().length());
};
