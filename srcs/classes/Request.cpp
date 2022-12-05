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
bool Request::readRequest(int clientSd, Response &res)
{
	ssize_t ret;
	char buffer[1024];
	string statusHeader;
	size_t i;

	// STATUS LINE AND HEADERS PARSING
	if ((ret = read(clientSd, buffer, 1024)) == -1 || ret == 0)
	{
		res.status(STATUS_400).send();
		return false;
	}
	for (i = 0; i < (size_t)ret; i++)
	{
		if (!isprint(buffer[i]) && buffer[i] != '\n')
			continue;
		if (statusHeader[statusHeader.size() - 1] == '\n' && buffer[i] == '\n')
		{
			statusHeader.substr(0, statusHeader.size() - 1);
			i++;
			break;
		}
		statusHeader.push_back(buffer[i]);
	}
	while (i < (size_t)ret)
		body.push_back(buffer[i++]);
	if (parseRequest(statusHeader, res) == FAILED)
		return false;

	// BODY PARSING
	if (ret != 1024)
		return true;
	while (true)
	{
		if ((ret = read(clientSd, buffer, 1024)) == -1 || ret == 0)
			return false;
		if (ret == 0)
			break;
		body.append(buffer, ret);
		if (ret < 1024)
			break;
	}
	return true;
}

bool Request::parseRequest(string statusHeader, Response &res)
{
	size_t endStatusLine = statusHeader.find("\n");

	// STATUS LINE
	if (parseStatusLine(statusHeader.substr(0, endStatusLine), res) == FAILED)
		return false;
	// HEADERS
	if (parseHeaders(statusHeader.substr(endStatusLine + 1)) == FAILED)
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
const char *Request::getUrlVar(const string key) const
{
	if (routeVars.find(key) == routeVars.end())
		return NULL;
	return routeVars.at(key).c_str();
};
const StrStrMap &Request::getHeaders() const { return headers; };
const string &Request::getBody() const { return body; };

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

	if (route == "/")
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
	cout << "\nVARS:\n";
	for (StrStrMap::iterator it = routeVars.begin(); it != routeVars.end(); ++it)
		cout << it->first << ":" << it->second << "$\n";
	// HEADERS
	cout << "\nHEADERS:\n";
	for (StrStrMap::iterator it = headers.begin(); it != headers.end(); ++it)
		cout << it->first << ":" << it->second << "$\n";
	// BODY
	cout << "\nBODY:\n" << body << "$\n";
};

bool Request::parseStatusLine(string rawStatusLine, Response &res)
{
	size_t i = 0;
	string method;

	// METHOD
	while (rawStatusLine.at(i) != ' ' && i < rawStatusLine.size())
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
	while (rawStatusLine.at(i) != ' ' && i < rawStatusLine.size())
		route.push_back(rawStatusLine.at(i++));
	parseUrlVars();
	absolutRoute = route;
	// PROTOCOL VERSION
	i++;
	while (i < rawStatusLine.size())
		protocolVersion.push_back(rawStatusLine.at(i++));
	if (protocolVersion != "HTTP/1.1")
	{
		res.status(STATUS_505).send();
		return false;
	}
	return true;
};

void Request::parseUrlVars()
{
	size_t varPos = 0;
	while (varPos < route.size() && route.at(varPos) != '?')
		varPos++;
	if (varPos == route.size())
		return;
	string str = route;
	route.clear();
	for (size_t i = 0; i < varPos; i++)
		route.push_back(str.at(i));
	varPos++;
	string tmp;
	while (varPos < str.size())
	{
		while (varPos < str.size() && str.at(varPos) != '=')
			tmp.push_back(str.at(varPos++));
		if (varPos == str.size())
			return;
		varPos++;
		routeVars[tmp];
		routeVars[tmp].clear();
		while (varPos < str.size() && str.at(varPos) != '?')
			routeVars[tmp].push_back(str.at(varPos++));
		varPos++;
		tmp.clear();
	}
}

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
