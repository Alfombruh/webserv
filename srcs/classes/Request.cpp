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

size_t Request::getClientId() const { return clientId; };

// PRIVATE

void Request::printReqAtributes()
{
	// STATUS LINE
	cout << "status line:\n";
	cout << "method:" << statusLine.method << "$\n";
	cout << "route:" << statusLine.route << "$\n";
	cout << "protocol version:" << statusLine.protocolVersion << "$\n";
	// REQUEST HEADERS
	cout << "\nrequest headers:\n";
	cout << "host:" << requestHeaders.host << "$\n";
	cout << "userAgent:" << requestHeaders.userAgent << "$\n";
	cout << "accept:" << requestHeaders.accept << "$\n";
	cout << "acceptLanguage:" << requestHeaders.acceptLanguage << "$\n";
	cout << "acceptEncoding:" << requestHeaders.acceptEncoding << "$\n";
	// GENERAL HEADERS
	cout << "\rgeneral headers:\n";
	// REPRESENTATION HEADERS
	cout << "\rrepresentation headers:\n";
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
		statusLine.method = GET;
	else if (method == "POST")
		statusLine.method = POST;
	else if (method == "DELETE")
		statusLine.method = DELETE;
	else
		return false;
	// ROUTE
	i++;
	while (rawStatusLine.at(i) != ' ')
		statusLine.route.push_back(rawStatusLine.at(i++));
	// PROTOCOL VERSION
	i++;
	while (i < rawStatusLine.length())
		statusLine.protocolVersion.push_back(rawStatusLine.at(i++));
	if (statusLine.protocolVersion != "HTTP/1.1")
		return false;
	return true;
};

bool Request::parseHeaders(string rawHeaders)
{

	requestHeaders.host = getValue(rawHeaders, "Host", 4);
	requestHeaders.userAgent = getValue(rawHeaders, "User-Agent", sizeof("User-Agent") - 1);
	requestHeaders.accept = getValue(rawHeaders, "User-Agent", sizeof("User-Agent") - 1);

	return true;
};
bool Request::parseBody(string rawBody)
{
	body = rawBody;
	return true;
};

string Request::getValue(string &str, string key, size_t keyLength)
{
	size_t keyPos = str.find(key);
	if (keyPos == string::npos)
		return NULL;
	keyPos += keyLength + 2;
	string tmp;
	while (str[keyPos] && str[keyPos] != '\n')
	{
		tmp.push_back(str[keyPos]);
		keyPos++;
	}
	return tmp;
};