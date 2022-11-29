#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "webserv.h"

using std::cout;
using std::string;

class Response
{
	size_t clientId;

	// STATUS LINE
	string protocolVersion;
	string statusCode;
	string statusText;

	// HEADERS
	StrStrMap headers;

	// BODY
	string body;

public:
	Response(int clientId);
	~Response();

	// bool parseResponse(string rawRes);
	void clearReq();

	// GETTERS - SETTERS
	string getHeader(string header);
	size_t getClientId() const;

	// RESPONSES
	void text(const string msg);
};

#endif

// https://developer.mozilla.org/en-US/docs/Web/HTTP/Messages