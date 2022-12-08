#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "webserv.h"
#include "Response.hpp"
#include <string>
#include <netinet/in.h>

using std::cout;
using std::string;

class Enviroment
{
public:
	string DOCUMENT_ROOT;
	string GATEWAY_INTERFACE;
	string HTTP_ACCEPT;
	string HTTP_ACCEPT_LANGUAGE;
	string HTTP_FROM;
	string HTTP_HOST;
	string HTTP_REFERER;
	string HTTP_USER_AGENT;
	string IFS;
	string MAILCHECK;
	string PATH;
	string PATH_INFO;
	string PATH_TRANSLATED;
	string QUERY_STRING;
	string REMOTE_ADDR;
	string REMOTE_HOST;
	string REMOTE_PORT;
	string REQUEST_METHOD;
	string REQUEST_URI;
	string SCRIPT_FILENAME;
	string SCRIPT_NAME;
	string SERVER_ADMIN;
	string SERVER_NAME;
	string SERVER_PORT;
	string SERVER_PROTOCOL;
	string SERVER_SOFTWARE;
	std::vector<std::string> env;
};

enum METHOD
{
	GET,
	POST,
	DELETE
};

class Request
{
	size_t clientId;
	sockaddr_in client_addr;

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
	bool parseChunkedRequest(string rawRequest, Response &res);
	bool parseStatusLine(string rawStatusLine, Response &res);
	void parseUrlVars();
	bool parseHeaders(string rawHeaders);
	char *urlDecode(const char *str);
	bool readChunkedRequest(int clientSd, Response &res, string &firstChunk);

public:
	Enviroment env;
	Request(int clientId, sockaddr_in client_addr);
	~Request();

	void clearReq();

	void printReqAtributes();
	// PARSING
	bool readRequest(int clientSd, Response &res);
	// GETTERS - SETTERS
	const string getHeader(const string header) const;
	size_t getClientId() const;
	sockaddr_in getClientAddr() const;
	const string &getRoute() const;
	const string &getAbsoluteRoute() const;
	const string getUrlVar(const string key) const;
	const StrStrMap &getHeaders() const;
	const string &getBody() const;
	const METHOD &getMethod() const;
	const string getMethodStr() const;
	const string &getProtocolVersion() const;
	bool isInRoute(const string route) const;
	void updateRoute(const string route);
};

#endif

// https://developer.mozilla.org/en-US/docs/Web/HTTP/Messages