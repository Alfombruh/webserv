#include "webserv.h"
#include "Request.hpp"

Request::Request(int clientId) : clientId((size_t)clientId) {}

Request::~Request() {}

void Request::clearReq()
{
	route.clear();
	protocolVersion.clear();
	headers.clear();
	body.clear();
}

/* raw request tiene caracteres no printeables */
bool Request::parseRequest(string rawReq, Response &res)
{
	size_t endStatusLine = rawReq.find("\n");
	size_t endHeaders = rawReq.find("\n\n");

	// cout << rawReq << "\n";
	// STATUS LINE
	if (parseStatusLine(rawReq.substr(0, endStatusLine), res) == FAILED)
		return false;
	// HEADERS
	if (parseHeaders(rawReq.substr(endStatusLine + 1, endHeaders - (endStatusLine + 1))) == FAILED)
		return false;
	// BODY
	if (parseBody(rawReq.substr(endHeaders + 2)) == FAILED)
		return false;
	return true;
};

const string Request::getHeader(const string header) const
{
	if (headers.find(header) != headers.end())
		return NULL;
	return headers.at(header);
};

size_t Request::getClientId() const { return clientId; };

const string &Request::getRoute() const { return route; };
const string &Request::getAbsoluteRoute() const { return absolutRoute; };

const METHOD &Request::getMethod() const { return method; };

const string Request::getMethodStr() const
{
	switch (method)
	{
	case 0:
		return "GET";
	case 1:
		return "POST";
	case 2:
		return "DELETE";
	default:
		break;
	}
}


bool Request::isInRoute(const string route) const
{
	size_t i = 0;
	if (route == this->route)
		return true;
	while (i < route.length() && i < this->route.length())
	{
		if (route[i] != this->route[i])
			return false;
		i++;
	}
	if (route == "/" || (i < this->route.length() && this->route[i] == '/'))
		return true;
	return false;
}

void Request::updateRoute(const string route)
{
	string tmp;
	size_t i = route.length();

	if(route == "/")
		return;
	while (i < this->route.length())
		tmp += this->route[i++];
	if (tmp.empty())
		tmp = "/";
	this->route = (const string)tmp;
}
// PRIVATE
void Request::printReqAtributes()
{
	// STATUS LINE
	cout << "STATUS LINE:\n";
	cout << "method:" << method << "$\n";
	cout << "route:" << route << "$\n";
	cout << "protocol version:" << protocolVersion << "$\n";
	// HEADERS
	cout << "\nHEADERS:\n";
	for (StrStrMap::iterator it = headers.begin(); it != headers.end(); ++it)
		cout << it->first << ":" << it->second << "$\n";
	// BODY
	cout << "\nBODY:\n";
};

bool Request::parseStatusLine(string rawStatusLine, Response &res)
{
	size_t i = 0;
	string method;

	// METHOD
	while (rawStatusLine.at(i) != ' ')
		method.push_back(rawStatusLine.at(i++));
	if (method == "GET")
		this->method = GET;
	else if (method == "POST")
		this->method = POST;
	else if (method == "DELETE")
		this->method = DELETE;
	else
	{
		res.status(STATUS_405).send();
		return false;
	}
	// ROUTE
	i++;
	while (rawStatusLine.at(i) != ' ')
		route.push_back(rawStatusLine.at(i++));
	absolutRoute = route;
	// PROTOCOL VERSION
	i++;
	while (i < rawStatusLine.length())
		protocolVersion.push_back(rawStatusLine.at(i++));
	if (protocolVersion != "HTTP/1.1")
	{
		res.status(STATUS_505).send();
		return false;
	}
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
