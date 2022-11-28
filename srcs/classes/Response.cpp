#include "webserv.h"
#include "Response.hpp"

Response::Response(int clientId) : clientId((size_t)clientId) {}

Response::~Response() {}

void Response::clearReq()
{
	protocolVersion.clear();
	statusCode.clear();
	statusText.clear();
	headers.clear();
	body.clear();
}

string Response::getHeader(string header)
{
	if (headers.find(header) != headers.end())
		return NULL;
	return headers.at(header);
};

size_t Response::getClientId() const { return clientId; };
