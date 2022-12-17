#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "webserv.h"
#include "config.hpp"
#include <unistd.h>
#include <vector>
#include <fstream>
#include <streambuf>
#include <string>
#include <sstream>

// using std::cout;
// using std::string;
class Config;

class Response
{
	const size_t clientId;

	// STATUS LINE
	static string protocolVersion;
	string stringStatus;

	// HEADERS
	// std::vector<string> headers;
	StrStrMap headers;

	// BODY
	string body;

	string readFile(const string filename);
	string readFileCgi(const string filename);

    __unused const Config &configuration;

public:
	Response(int clientId, const Config &configuration);
	~Response();

	// bool parseResponse(string rawRes);
	string stringifyResponse();
	void clearResponse();

	// GETTERS - SETTERS
	size_t getClientId() const;
	const string getHeader(const string header) const;
	const StrStrMap &getHeaders() const;

	// RESPONSES
	Response &status(const string status);
	Response &text(const string &msg);
	Response &textHtml(const string &msg);
	Response &redirect(string path);
	Response &textPython(const string filename, char **env);
	Response &json(const string &json);
	Response &html(const string filename);
	Response &htmlCgi(const string filename);
	Response &img(const string filename);
	void setBody(const string body);
	Response &css(const string filename);
	Response &js(const string filename);
	Response &cookie(const string &hash);
	Response &expireCookie();
	void send();
};

#endif

// https://developer.mozilla.org/en-US/docs/Web/HTTP/Messages