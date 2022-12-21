#include "webserv.h"
#include "Request.hpp"
#include "string.h"
#include <iostream>
#include <fstream>

Request::Request(int clientId, sockaddr_in client_addr)
	: clientId((size_t)clientId), client_addr(client_addr) {}

Request::~Request() {}

void *Request::ft_memset(void *str, int c, size_t len)
{
	unsigned char *aux;

	aux = (unsigned char *)str;
	while (len-- > 0)
		*aux++ = c;
	return (str);
}

void Request::clearReq()
{
	route.clear();
	protocolVersion.clear();
	headers.clear();
	body.clear();
}

/* raw request tiene caracteres no printeables */
bool Request::readChunkedRequest(int clientSd, Response &res)
{
	char c[100000];
	size_t ret;

	res.status(STATUS_100).send();
	while (body.find("0\r\n\r\n") == string::npos)
	{
		memset(c, 0, 100000);
		if ((ret = recv(clientSd, c, 100000, 0)) == (size_t)-1 || ret == (size_t)0)
		{
			res.status(STATUS_400).send();
			return false;
		}
		for (size_t i = 0; i < ret; ++i)
		{
			body.push_back(c[i]);
		}
		// cout << c;
	}
	// cout << "REQUEST BODY\n" << body;
	parseChunkedBody(body.substr(0, body.size() - 7));
	// cout << "REQUEST BODY\n" << body;
	return true;
};

bool Request::readRequest(int clientSd, Response &res)
{
	ssize_t ret;
	string request = "";
	size_t reqHeadersEnd;
	size_t bodySize;
	string statusHeader;
	bool isChunked;
	size_t i = 0;

	char c;
	// STATUS LINE AND HEADERS PARSING
	while ((reqHeadersEnd = request.find("\r\n\r\n")) == string::npos)
	{
		if ((ret = recv(clientSd, &c, 1, 0)) == -1 || ret == 0)
		{
			res.status(STATUS_400).send();
			return false;
		}
		request.push_back(c);
	}
	while (i < reqHeadersEnd)
	{
		if (std::isprint(request[i]) || request[i] == '\n')
			statusHeader.push_back(request[i]);
		i++;
	}
	body = "";
	if (parseRequest(statusHeader, res) == FAILED)
		return false;
	isChunked = getHeader("transfer-encoding").find("chunked") != string::npos ? true : false;
	bodySize = getHeader("content-length").empty() ? 0 : stringToSize_t(getHeader("content-length"));
	if (isChunked)
		return readChunkedRequest(clientSd, res);
	if (bodySize == 0)
		return true;

	// BODY PARSING
	ret = 0;
	while (true)
	{
		if ((ret += recv(clientSd, &c, 1, 0)) == -1)
		{
			res.status(STATUS_400).send();
			return false;
		}
		body.push_back(c);

		if ((size_t)ret == bodySize)
			break;
	}
	return true;
}

void Request::parseChunkedBody(string rawBody)
{
	size_t i = 0;
	body.clear();
	while (i < rawBody.size())
	{
		while (i + 1 < rawBody.size() && rawBody[i] != '\r' && rawBody[i + 1] != '\n')
			i++;
		i += 2;
		while (i + 1 < rawBody.size() && rawBody[i] != '\r' && rawBody[i + 1] != '\n')
			body.push_back(rawBody[i++]);
		i += 2;
	}
	if (rawBody.size() != 0)
		body.push_back(rawBody[rawBody.size() - 1]);
};

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

bool Request::parseStatusLine(string rawStatusLine, Response &res)
{
	size_t i = 0;
	string method;

	// METHOD
	while (i < rawStatusLine.size() && rawStatusLine.at(i) != ' ')
		method.push_back(rawStatusLine.at(i++));
	if (method == "GET")
		this->method = GET;
	else if (method == "POST")
		this->method = POST;
	else if (method == "DELETE")
		this->method = DELETE;
	// else if (method == "PUT")
	// { // harcoded
	// 	res.status(STATUS_200).send();
	// 	return false;
	// }
	else
	{
		res.errorPage("405", STATUS_405).send();
		return false;
	}
	// ROUTE
	i++;
	while (i < rawStatusLine.size() && rawStatusLine.at(i) != ' ')
		route.push_back(rawStatusLine.at(i++));
	parseUrlVars();
	absolutRoute = route;
	// PROTOCOL VERSION
	i++;
	while (i < rawStatusLine.size())
		protocolVersion.push_back(rawStatusLine.at(i++));
	if (protocolVersion != "HTTP/1.1")
	{
		res.errorPage("505", STATUS_505).send();
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
	env.env.push_back("QUERY_STRING=" + str);
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
		while (varPos < str.size() && str.at(varPos) != '&')
			routeVars[tmp].push_back(str.at(varPos) == '+' ? ' ' : str.at(varPos++));
		varPos++;
		tmp.clear();
	}
}

string Request::encodeEnv(string keyToEncode, string value)
{
	if (keyToEncode.empty() || value.empty())
		return "";

	string env_string;
	size_t length = keyToEncode.length();
	size_t i = -1;
	if (keyToEncode == "content-type" || keyToEncode.compare(0, 2, "x-") == 0 || keyToEncode == "content-length" || keyToEncode == "user-agent")
	{
		if (keyToEncode.compare(0, 2, "x-") == 0)
		{
			env_string = "HTTP_";
		}
		while (++i < length)
		{
			if (keyToEncode[i] != '-')
				env_string.push_back((char)toupper(keyToEncode[i]));
			else
				env_string.push_back('_');
		}
	}
	else
	{
		if (keyToEncode == "referer")
			env_string = "PATH_TRANSLATED";
		else if (keyToEncode.compare(0, 2, "x-") == 0)
		{
			env_string = "HTTP_";
		}
		else
			return "";
	}
	env_string.push_back('=');
	env_string += value;
	return env_string;
}

bool Request::parseHeaders(string rawHeaders)
{
	size_t i = 0;
	size_t length = rawHeaders.length();
	string key;
	string value;
	string encode;
	while (i < length)
	{
		while (i < length && rawHeaders[i] != ':')
			key.push_back(tolower(rawHeaders[i++]));
		i += 2;
		while (i < length && rawHeaders[i] != '\n')
			value.push_back(rawHeaders[i++]);
		i++;
		encode = encodeEnv(key, value);
		if (!encode.empty())
			env.env.push_back(encode);
		headers.insert(make_pair(key, value));
		key.clear();
		value.clear();
	}
	return true;
};

const string Request::getHeader(const string header) const
{
	if (headers.find(header) == headers.end())
		return "";
	return headers.at(header);
};

size_t Request::getClientId() const { return clientId; };

sockaddr_in Request::getClientAddr() const { return client_addr; };

const string &Request::getRoute() const { return route; };
const string &Request::getAbsoluteRoute() const { return absolutRoute; };
const string Request::getUrlVar(const string key) const
{
	if (routeVars.find(key) == routeVars.end())
		return "";
	return routeVars.at(key);
};
const StrStrMap &Request::getHeaders() const { return headers; };
const string &Request::getBody() const { return body; };

const METHOD &Request::getMethod() const { return method; };
const string &Request::getProtocolVersion() const { return protocolVersion; };

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

void Request::setRoute(const string route)
{
	this->route = route;
}
// PRIVATE
void Request::printReqAtributes()
{
	// STATUS LINE
	/*
	cout << "STATUS LINE:\n";
	cout << "method:" << getMethodStr() << "$\n";
	cout << "route:" << route << "$\n";
	cout << "absolutRoute:" << absolutRoute << "$\n";
	cout << "protocol version:" << protocolVersion << "$\n";
	cout << "\nVARS:\n";
	for (StrStrMap::iterator it = routeVars.begin(); it != routeVars.end(); ++it)
		cout << it->first << ":" << it->second << "$\n";
	// HEADERS
	*/
	cout << "\nHEADERS:\n";
	for (StrStrMap::iterator it = headers.begin(); it != headers.end(); ++it)
		cout << it->first << ":" << it->second << "$\n";

	// BODY
	// cout << "\nBODY:\n"
	//    << body << "$\n";

	// cout << "SERVER_PROTOCOL:" << this->env.SERVER_PROTOCOL << "$\n";
	// cout << "REQUEST_METHOD:" << this->env.REQUEST_METHOD << "$\n";
	// cout << "PATH_INFO:" << this->env.PATH_INFO << "$\n";
	// cout << "PATH_TRANSLATED:" << this->env.PATH_TRANSLATED << "$\n";
	// cout << "REMOTE_ADDR:" << this->env.REMOTE_ADDR << "$\n";
	// cout << "REMOTE_PORT:" << this->env.REMOTE_PORT << "$\n";
	// cout << "REMOTE_ADDR:" << this->env.SERVER_NAME << "$\n";
	// cout << "REMOTE_PORT:" << this->env.SERVER_PORT << "$\n";
};
