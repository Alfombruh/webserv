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
	struct s_statusLine
	{
		METHOD method;
		string route;
		string protocolVersion;
	} statusLine;
	struct s_requestHeaders
	{
		string host;
		string userAgent;
		string accept;
		string acceptLanguage;
		string acceptEncoding;
	} requestHeaders;
	struct s_generalHeaders
	{
		string connection;
		string upgradeInsecureRequests;
	} generalHeaders;
	struct s_representationHeaders
	{
		string contentType;
		string contentLength;
	} representationHeaders;
	string body;

public:
	Request(int clientId);
	~Request();

	// PARSING
	bool parseRequest(char *rawReq);
	bool parseStatusLine(string rawStatusLine);
	// GETTERS - SETTERS
	size_t getClientId() const;
};

#endif

// https://developer.mozilla.org/en-US/docs/Web/HTTP/Messages