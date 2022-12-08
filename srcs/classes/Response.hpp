#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "webserv.h"
#include "unistd.h"
#include <vector>
#include <fstream>
#include <streambuf>
#include <string>
#include <sstream>

using std::cout;
using std::string;

class Response
{
	size_t clientId;

	// STATUS LINE
	static string protocolVersion;
	string stringStatus;

	// HEADERS
	std::vector<string> headers;

	// BODY
	string body;

	string readFile(const string filename);
public:
	Response(int clientId);
	~Response();

	// bool parseResponse(string rawRes);
	string stringifyResponse();
	void clearResponse();

	// GETTERS - SETTERS
	size_t getClientId() const;

	// RESPONSES
	Response &status(const string status);
	Response &text(const string &msg);
	Response &text_python(const string filename, char **env);
	Response &html(const string filename);
	Response &img(const string filename);
	Response &css(const string filename);
	Response &js(const string filename);
	void send();
};

#endif

// https://developer.mozilla.org/en-US/docs/Web/HTTP/Messages