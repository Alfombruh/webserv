#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "Server.hpp"
#include "SerConf.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <fcntl.h>

class Config
{
private:
	int fd;
	int workers;
	std::string path;
	std::string content;
	std::vector<std::string> tokens;
	std::vector<ServConf> servers;

public:
	Config(std::string &path);
	~Config();
	void tokenize();
	void parse();
	void clear();
	int getWorkers();
	std::vector<ServConf> &getServers();
	std::string &getPath();
	std::string &getFileContent();
	bool isDirectiveVal(std::string const &str);

	class configException : virtual public std::exception
	{
	private:
		int errno;
		std::string error_msg;

	public:
		configException(std::string msg, int err_num = 0, std::string arg = "") : errno(err_num)
		{
			error_msg = msg;
			if (error_msg.find('%') != std::string::npos)
				error_msg.replace(error_msg.find('%'), 1, arg);
			if (errno)
			{
				error_msg = error_msg + " (" + ft::to_string(errno) + ": " + strerror(errno) + ")";
			}
		}
		virtual ~configException() throw() {}
		const char *what() const throw()
		{
			return error_msg.c_str();
		}
	};
};

#endif
