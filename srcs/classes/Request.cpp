#include "webserv.h"
#include "Request.hpp"

Request::Request(int clientId) : clientId((size_t)clientId) {}

Request::~Request() {}

/* raw request tiene caracteres no printeables */
bool Request::parseRequest(string rawReq)
{
	size_t endStatusLine = rawReq.find("\n");
	size_t endHeaders = rawReq.find("\n\n");

	// cout << rawReq << "\n";
	// STATUS LINE
	if (parseStatusLine(rawReq.substr(0, endStatusLine)) == FAILED)
		return false;
	// HEADERS
	if (parseHeaders(rawReq.substr(endStatusLine + 1, endHeaders - (endStatusLine + 1))) == FAILED)
		return false;
	// BODY
	if (parseBody(rawReq.substr(endHeaders + 2)) == FAILED)
		return false;

	printReqAtributes();
	return true;
};

string Request::getHeader(string header)
{
	if(headers.find(header) != headers.end())
		return NULL;
	return headers.at(header);
};

size_t Request::getClientId() const { return clientId; };

// PRIVATE

void Request::printReqAtributes()
{
	// STATUS LINE
	cout << "status line:\n";
	cout << "method:" << method << "$\n";
	cout << "route:" << route << "$\n";
	cout << "protocol version:" << protocolVersion << "$\n";
	// REQUEST HEADERS
	cout << "\nheaders:\n";
	for (StrStrMap::iterator it = headers.begin(); it != headers.end(); ++it)
		cout << it->first << ":" << it->second << "$\n";
	// BODY
	cout << "\nbody headers:\n";
};

bool Request::parseStatusLine(string rawStatusLine)
{
	size_t i = 0;
	string method;

	// METHOD
	while (rawStatusLine.at(i) != ' ')
		method.push_back(rawStatusLine.at(i++));
	if (method == "GET")
		method = GET;
	else if (method == "POST")
		method = POST;
	else if (method == "DELETE")
		method = DELETE;
	else
		return false;
	// ROUTE
	i++;
	while (rawStatusLine.at(i) != ' ')
		route.push_back(rawStatusLine.at(i++));
	// PROTOCOL VERSION
	i++;
	while (i < rawStatusLine.length())
		protocolVersion.push_back(rawStatusLine.at(i++));
	if (protocolVersion != "HTTP/1.1")
		return false;
	return true;
};

bool Request::parseHeaders(string rawHeaders)
{
	size_t i = 0;
	size_t length = rawHeaders.length();
	string key;
	string value;

	while (i < length)
	{
		while (i < length && rawHeaders[i] != ':')
			key.push_back(tolower(rawHeaders[i++]));
		i += 2;
		while (i < length && rawHeaders[i] != '\n')
			value.push_back(rawHeaders[i++]);
		i++;
		headers.insert(make_pair(key, value));
		key.clear();
		value.clear();
	}
	return true;
};
bool Request::parseBody(string rawBody)
{
	body = rawBody;
	return true;
};
