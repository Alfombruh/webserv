#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <iostream>
# include <string>
# include <vector>
# include <stack>
# include <fcntl.h>

# include "../srcs/classes/Server.hpp"
# include "servconf.hpp"

class ServConf;

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
			public:
				configException(std::string msg, int err_num = 0, std::string arg = "") : errno_(err_num)
				{
					error_msg_ = msg;
					if (error_msg_.find('%') != std::string::npos)
						error_msg_.replace(error_msg_.find('%'), 1, arg);
					if (errno_)
					{
						error_msg_ = error_msg_ + " (" + std::to_string(errno_) + ": " + strerror(errno_) + ")";
					}
				}
				virtual ~configException() throw () {}
				const char* what() const throw () {
					return error_msg_.c_str();
				}
			private:
				int errno_;
				std::string error_msg_;
		};
};

#endif
