#include "webserv.h"
#include "Response.hpp"
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>

Response::Response(int clientId) : clientId((size_t)clientId)
{
	headers.insert(make_pair("Host", configuration.getServerName()));
	// HERE WE INITIALIZE ALL STATIC HEADERS HOST, PORT, LOCATION...
};

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
	headers["Content-type"] = "text/plain";
	body = msg;
	return *this;
};

Response &Response::textHtml(const string &msg)
{
	headers["Content-type"] = "text/html";
	body = "<html><body>" + msg + "</body></html>";
	return *this;
};

Response &Response::redirect(string path)
{
	headers["Location"] = path;
	return *this;
};

void Response::setBody(const string body){ this->body = body;}

Response &Response::textPython(const string filename, char **env)
{

	string newbody;
	for (int i = 0; env[i] != NULL; ++i) {
		cout << env[i] << "\n";
	}
	size_t i = 0;
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
			std::remove(".cgi.txt");
		}
	}
	body = newbody;
	return *this;
};

Response &Response::json(const string &json)
{
	headers["Content-type"] = "application/json";
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
	std::string key;
	std::string value;
	bool isheader = true;
	string tmp;
	while (!f.eof())
	{
		std::getline(f, s);
		if (s == "\r" || s.empty() || s == "\n") {
			isheader = false;
			continue;
		}
		if (isheader)
		{
			size_t length = s.length();
			size_t i = 0;
			while (i < length && s[i] != ':')
				key.push_back(s[i++]);
			i += 2;
			while (i < length && s[i] != '\n')
				value.push_back(s[i++]);
			headers.insert(make_pair(key, value));
			key.clear();
			value.clear();
		}
		else
			tmp += s;
	}
	f.close();
	return tmp;
}

Response &Response::lsDir(const string filename)
{
	int fd[2];
	if (pipe(fd) == -1) {
		return *this;
	}
	pid_t pid;
	pid = fork();
	if (pid == -1) {
		return *this;
	}
	if (pid == 0)
	{
		fd[1] = open("ls_cgi", O_CREAT | O_TRUNC | O_RDWR, 0777);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		char *pythonArgs[] =  {(char *)"./cgi-bin/ls_c_cgi", (char *)filename.c_str(), NULL};
		execve(pythonArgs[0], pythonArgs,NULL);
		std::remove("ls_cgi");
		exit(0);
	}
	else
	{
		wait(0);
		body = readFile("ls_cgi");
		std::remove("ls_cgi");
	}
	headers["Content-Type"] = "text/html";
	return *this;
};

Response &Response::html(const string filename)
{
	body = readFile(filename);
	// cout << body << "\n";
	headers["Content-Type"] = "text/html";
	return *this;
};

Response &Response::img(const string filename)
{
	body = readFile(filename);
	headers["Content-Type"] = "image/png";
	return *this;
};

Response &Response::css(const string filename)
{
	body = readFile(filename);
	headers["Content-Type"] = "text/css";
	return *this;
};

Response &Response::js(const string filename)
{
	body = readFile(filename);
	headers["Content-Type"] = "application/javascript";
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

	headers["Content-length"] = std::to_string(body.length());
	headers["Connection"] = "close";
	string response = stringifyResponse();
	// cout << "-------------res-------------\n";
	// cout << response;
	// cout << "-----------------------------\n";
	write(clientId, response.c_str(), response.length());
	clearResponse();
};
