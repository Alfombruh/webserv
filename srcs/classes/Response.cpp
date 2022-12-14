#include "webserv.h"
#include "Response.hpp"
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>

Response::Response(int clientId) : clientId((size_t)clientId)
{
	headers.insert(make_pair("host", (string)HOST));
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
	for (StrStrMap::iterator it = headers.begin(); it != headers.end(); ++it)
		response += (it->first + ": " + it->second + "\n");
	response += "\n";
	// BODY
	if (!body.empty())
		response += (body + "\n"); //OJO!
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

const string Response::getHeader(const string header) const
{
	if (headers.find(header) == headers.end())
		return "";
	return headers.at(header);
};
const StrStrMap &Response::getHeaders() const { return headers; };

// RESPONSES
Response &Response::status(const string status)
{
	stringStatus = status;
	return *this;
};

Response &Response::text(const string &msg)
{
	headers["content-type"] = "text/plain";
	body = msg;
	return *this;
};

Response &Response::textHtml(const string &msg)
{
	headers["content-type"] = "text/html";
	body = msg;
	return *this;
};

Response &Response::redirect(string path)
{
	headers["location"] = path;
	return *this;
};

void Response::setBody(const string body){ this->body = body;}

Response &Response::text_python(const string filename, char **env)
{

	string newbody;
	headers["server"] = "webvserv";
	headers["status"] = "200 OK";
	headers["content-type"] = "Content-Type: text/html; charset=utf-8";
	for (int i = 0; env[i] != NULL; ++i) {
		cout << env[i] << "\n";
	}
	int i = 0;
	while (body.size() >= i)
	{
		int fd[2];
		if (pipe(fd) == -1) {
			return *this;
		}
		string body_erase = body.substr(i, 100000);
		i += 100000;
		pid_t pid;
		pid = fork();
		if (pid == -1)
			return *this;
		if (pid == 0)
		{
			dup2(fd[1], STDOUT_FILENO);
			close(fd[0]);
			char *pythonArgs[] =  {(char *)"echo", (char *)"-n",(char *)body_erase.c_str(),NULL};
			execve("/bin/echo", pythonArgs,env);
			printf("execl returned! errno is [%d]\n", errno);
			perror("The error message is :");
			exit(0);
		}
		else
		{
			pid_t pid2;
			pid2 = fork();
			if (pid2 == 0)
			{
				int fd2 = open(".cgi.txt", O_CREAT | O_TRUNC | O_RDWR, 0777);
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
			newbody += readFileCgi(".cgi.txt");
		}
	}
	body = newbody;
	return *this;
};

Response &Response::json(const string &json)
{
	headers["content-type"] = "application/json";
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

string Response::readFileCgi(const string filename)
{
	std::ifstream f(filename);
	std::string s;
	std::getline(f, s);
	std::getline(f, s);
	std::getline(f, s);
	string tmp;
	while (!f.eof())
	{
		std::getline(f, s);
		tmp += s;
	}
	f.close();
	return tmp;
}

Response &Response::html(const string filename)
{
	body = readFile(filename);
	// cout << body << "\n";
	headers["content-type"] = "text/html";
	return *this;
};

Response &Response::img(const string filename)
{
	body = readFile(filename);
	headers["content-type"] = "image/png";
	return *this;
};

Response &Response::css(const string filename)
{
	body = readFile(filename);
	headers["content-type"] = "text/css";
	return *this;
};

Response &Response::js(const string filename)
{
	body = readFile(filename);
	headers["content-type"] = "application/javascript";
	return *this;
};

Response &Response::cookie(const string &hash)
{
	headers["set-cookie"] = "42webserv_session=" + hash + "; path=/";
	return *this;
};

Response &Response::expireCookie()
{
	headers["set-cookie"] = "42webserv_session=deleted; path=/; expires=Thu, 01 Jan 1970 00:00:00 GMT";
	return *this;
};

void Response::send()
{
	// if (!body.empty())

	headers["content-length"] = std::to_string(body.length());
	headers["connection"] = "close";
	string response = stringifyResponse();
	// cout << "-------------res-------------\n";
	// cout << response;
	// cout << "-----------------------------\n";
	write(clientId, response.c_str(), response.length());
	clearResponse();
};
