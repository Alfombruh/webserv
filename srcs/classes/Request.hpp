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
	string protocolVersion;

 	//HEADERS
	StrStrMap headers;
	
	//BODY
	string body;

	void printReqAtributes();
	bool parseStatusLine(string rawStatusLine);
	bool parseHeaders(string rawHeaders);
	bool parseBody(string rawBody);

public:
	Request(int clientId);
	~Request();

	void clearReq();
	// PARSING
	bool parseRequest(string rawReq);
	// GETTERS - SETTERS
	string getHeader(string header);
	size_t getClientId() const;
};

#endif

// https://developer.mozilla.org/en-US/docs/Web/HTTP/Messages