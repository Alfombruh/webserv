#include "webserv.h"
#include "Response.hpp"

Response::Response(int clientId) : clientId((size_t)clientId)
{
	headers.push_back("Host: " + (string)HOST);
	// HERE WE INITIALIZE ALL STATIC HEADERS HOST, PORT, LOCATION...
}

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
	return *this;
};

Response &Response::html(const string filename)
{
	std::ifstream file(filename);
	string tmp((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();
	body = tmp;
	// cout << body << "\n";
	headers.push_back("Content-Type: text/html");
	headers.push_back("Content-Length: " + std::to_string(body.length()));
	return *this;
};

Response &Response::img(const string filename)
{
	std::ifstream file(filename);
	string tmp((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();
	body = tmp;
	headers.push_back("Content-Type: image/png");
	headers.push_back("Content-Length: " + std::to_string(body.length()));
	return *this;
};

void Response::send()
{
	string response = stringifyResponse();
	// cout << "-------------res-------------\n";
	// cout << response;
	// cout << "-----------------------------\n";
	write(clientId, response.c_str(), response.length());
	clearResponse();
};
