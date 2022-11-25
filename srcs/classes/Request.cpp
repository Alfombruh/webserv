#include "webserv.h"
#include "Request.hpp"

Request::Request(int clientId) : clientId((size_t)clientId) {}

Request::~Request() {}

/* raw request tiene caracteres no printeables */
bool Request::parseRequest(char *rawReq)
{
	size_t i = 0;
	string rawStatusLine;

	//STATUS LINE
	while (isprint(rawReq[i]))
		rawStatusLine.push_back(rawReq[i++]);
	if (parseStatusLine(rawStatusLine) == FAILED)
		return false;

	return true;
};

bool Request::parseStatusLine(string rawStatusLine)
{
	size_t i = 0;
	string method;

	while (rawStatusLine.at(i) != ' ')
		method.push_back(rawStatusLine.at(i++));
	if (method == "GET")
		statusLine.method = GET;
	else if (method == "POST")
		statusLine.method = POST;
	else if (method == "DELETE")
		statusLine.method = DELETE;
	else
		return false;
	
	i++;
	while (rawStatusLine.at(i) != ' ')
		statusLine.route.push_back(rawStatusLine.at(i++));

	i++;
	while (i < rawStatusLine.length())
		statusLine.protocolVersion.push_back(rawStatusLine.at(i++));
	if (statusLine.protocolVersion != "HTTP/1.1")
		return false;
	return true;
};

size_t Request::getClientId() const { return clientId; };
