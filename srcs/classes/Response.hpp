#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "webserv.h"

using std::cout;
using std::string;

class Response
{
	size_t clientId;
	struct s_statusLine
	{
		string protocolVersion;
		string statusCode;
		string statusText;
	};
	struct s_ResponseHeaders
	{
		string host;
		string userAgent;
		string accept;
		string acceptLanguage;
		string acceptEncoding;
	};
	struct s_generalHeaders
	{
		string connection;
		string upgradeInsecureResponses;
	};
	struct s_representationHeaders
	{
		string contentType;
		string contentLength;
	};
	string body;

public:
	Response(int clientId);
	~Response();

	// bool parseResponse(string rawRes);

	//GETTERS - SETTERS
	size_t getClientId() const;
};

#endif

// https://developer.mozilla.org/en-US/docs/Web/HTTP/Messages