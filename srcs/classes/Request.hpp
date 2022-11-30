#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "webserv.h"
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

 	//STATUS LINE
	METHOD method;
	string route;
	string absolutRoute;
	string protocolVersion;

 	//HEADERS
	StrStrMap headers;
	
	//BODY
	string body;

	bool parseStatusLine(string rawStatusLine);
	bool parseHeaders(string rawHeaders);
	bool parseBody(string rawBody);
	void printReqAtributes();

public:
	Request(int clientId);
	~Request();

	void clearReq();
	// PARSING
	bool parseRequest(string rawReq);
	// GETTERS - SETTERS
	const string getHeader(const string header) const;
	size_t getClientId() const;
	const string &getRoute() const;
	const string &getAbsoluteRoute() const;
	const METHOD &getMethod() const;
	bool isInRoute(const string route) const;
	void updateRoute(const string route);
};

#endif

// https://developer.mozilla.org/en-US/docs/Web/HTTP/Messages