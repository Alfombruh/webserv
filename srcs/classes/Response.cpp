#include "webserv.h"
#include "Response.hpp"
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>

Response::Response(int clientId) : clientId((size_t)clientId)
{
	headers.push_back("Host: " + (string)HOST);
	// HERE WE INITIALIZE ALL STATIC HEADERS HOST, PORT, LOCATION...
}

string Response::protocolVersion = "HTTP/1.1";

Response::~Response() {}

string Response::stringifyResponse()
{
	if (stringStatus.empty())
		stringStatus = STATUS_200;
	// STATUS LINE
	string response = (protocolVersion + " " + stringStatus + "\n");
	// HEADERS
	for (std::vector<string>::iterator it = headers.begin(); it != headers.end(); ++it)
		response += (*it + "\n");
	response += "\n";
	// BODY
	if (!body.empty())
		response += (body + "\n");
	return response;
};

void Response::clearResponse()
{
	stringStatus.clear();
	headers.clear();
	body.clear();
};

// GETTERS - SETTERS

size_t Response::getClientId() const { return clientId; };

// RESPONSES
Response &Response::status(const string status)
{
	stringStatus = status;
	return *this;
};

Response &Response::text(const string &msg)
{
	headers.push_back("Content-Type: text/plain");
	body = msg;
	return *this;
};

Response &Response::textHtml(const string &msg)
{
	headers.push_back("Content-Type: text/html");
	body = msg;
	return *this;
};

Response &Response::redirect(string path)
{
	headers.push_back("Location: " + path);
	return *this;
};

void Response::setBody(const string body){ this->body = body;}

Response &Response::text_python(const string filename, char **env)
{	int fd[2];
	if (pipe(fd) == -1) {
		return *this;
	}

	pid_t pid;
	pid = fork();
	headers.push_back("Server: webvserv");
	headers.push_back("Status: 200 OK");
	headers.push_back("Content-Type: text/html; charset=utf-8");
	if (pid == -1)
		return *this;
	if (pid == 0)
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
		char *pythonArgs[] =  {"echo", "-n", "holaaaaa" ,NULL};
		execve(pythonArgs[0], pythonArgs, env);
		exit(0);
	}
	else
	{
		pid_t pid2;
		pid2 = fork();
		if (pid2 == 0)
		{
			int fd2 = open("t.txt", O_CREAT | O_TRUNC | O_RDWR, 0777);
			// Child process 2 (grep)
			dup2(fd[0], STDIN_FILENO);
			dup2(fd2, STDOUT_FILENO);
			close(fd[0]);
			close(fd[1]);
			char *cstr = new char[filename.length() + 2];
			cstr[0] = '.';
			strcpy(cstr + 1, filename.c_str());
			char *pythonArgs[] = {cstr, NULL};
			execve(pythonArgs[0], pythonArgs, env);
			printf("execl returned! errno is [%d]\n", errno);
			perror("The error message is :");
			exit(EXIT_FAILURE);
		} else {
			wait(0);
		}
		close(fd[0]);
		close(fd[1]);
		waitpid(pid2, NULL, 0);
		//body += readFile("t.txt");
	}
	return *this;
};

Response &Response::json(const string &json)
{
	headers.push_back("Content-Type: application/json");
	body = json;
	return *this;
};

string Response::readFile(const string filename)
{
	std::ifstream file(filename);
	string tmp((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();
	return tmp;
}

Response &Response::html(const string filename)
{
	body = readFile(filename);
	// cout << body << "\n";
	headers.push_back("Content-Type: text/html");
	return *this;
};

Response &Response::img(const string filename)
{
	body = readFile(filename);
	headers.push_back("Content-Type: image/png");
	return *this;
};

Response &Response::css(const string filename)
{
	body = readFile(filename);
	headers.push_back("Content-Type: text/css");
	return *this;
};

Response &Response::js(const string filename)
{
	body = readFile(filename);
	headers.push_back("Content-Type: application/javascript");
	return *this;
};

Response &Response::cookie(const string &hash)
{
	headers.push_back("Set-Cookie: 42webserv_session=" + hash + "; path=/");
	return *this;
};

Response &Response::expireCookie()
{
	headers.push_back("Set-Cookie: 42webserv_session=deleted; path=/; expires=Thu, 01 Jan 1970 00:00:00 GMT");
	return *this;
};

void Response::send()
{
	// if (!body.empty())

	headers.push_back("Content-Length: " + std::to_string(body.length()));
	headers.push_back("Connection: close");
	string response = stringifyResponse();
	cout << "\n";
	for (std::vector<string>::iterator it = headers.begin(); it != headers.end(); ++it)
		cout << *it << "\n";
	// cout << "-------------res-------------\n";
	// cout << response;
	// cout << "-----------------------------\n";
	write(clientId, response.c_str(), response.length());
	clearResponse();
};
