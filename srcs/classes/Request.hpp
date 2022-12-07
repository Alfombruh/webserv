#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "webserv.h"
#include "Response.hpp"
#include <string>

using std::cout;
using std::string;

enum METHOD
{
	GET,
	POST,
	DELETE
};

class Request
{
	size_t clientId;

	// STATUS LINE
	METHOD method;
	string route;
	string absolutRoute;
	StrStrMap routeVars;
	string protocolVersion;

	// HEADERS
	StrStrMap headers;

	// BODY
	string body;

	bool parseRequest(string statusHeader, Response &res);
	bool parseStatusLine(string rawStatusLine, Response &res);
	void parseUrlVars();
	bool parseHeaders(string rawHeaders);
	char *urlDecode(const char *str);

public:
	Request(int clientId);
	~Request();

	void clearReq();

	void printReqAtributes();
	// PARSING
	bool readRequest(int clientSd, Response &res);
	// GETTERS - SETTERS
	const string getHeader(const string header) const;
	size_t getClientId() const;
	const string &getRoute() const;
	const string &getAbsoluteRoute() const;
	const string getUrlVar(const string key) const;
	const StrStrMap &getHeaders() const;
	const string &getBody() const;
	const METHOD &getMethod() const;
	const string getMethodStr() const;
	bool isInRoute(const string route) const;
	void updateRoute(const string route);
};

#endif

// https://developer.mozilla.org/en-US/docs/Web/HTTP/Messages