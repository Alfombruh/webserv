#include "config.hpp"

Config::Config(std::string &path) : path(path)
{
	workers = 0;
	fd = open(path.c_str(), O_RDONLY);
	if (fd < 0)
		throw(configException("couldn't open configuration file : %", 0, path)); //thow an error open
}

Config::~Config()
{
	clear();
}

void Config::tokenize()
{
	std::string				line;
	std::string				temp;
	std::string::size_type	first;
	std::string::size_type	last;
	std::stack<bool>		brackets;
	int						line_count = 1;
	std::ifstream			in(path.c_str());

	while (std::getline(in, line))
	{
		content += line + "\n";
		last = 0;
		while ((first = line.find_last_not_of(" \t", last)) != std::string::npos)
		{
			if (line[first] == '#')
				break;
			last = line.find_first_of(" \t", first);
			temp = line.substr(first, last - first);
			if (temp == "{")
				brackets.push(true);
			else if (temp == "}")
			{
				if (brackets.empty())
					throw(configException("extra closing  on line  %", 0, std::to_string(line_count))); //thow extra bracket error
				brackets.pop();
			}	
			if (isDirectiveVal(temp) && line[line.find_last_not_of(" \t", line.length())] != ';') //isDirectiveVal() wip(havent done it yet)
				throw(configException("missing ';' line %", 0, std::to_string(line_count)));//throw missing ; error
			if (temp.find(';', temp.length() - 1) != std::string::npos)
			{
				temp.erase(temp.length() - 1, 1);
				tokens.push_back(temp);
				tokens.push_back(";");
			}
			else
				tokens.push_back(temp);
		}
		line_count++;
	}
}

void Config::parse()
{
	int	i = 1;
	std::vector<std::string>::iterator	it;
	
	tokenize();
	it = tokens.begin();
	while (it != tokens.end())
	{
		if (*it == "server")// gonna change this and make a serverconfig file 
		{
			ServConf	serv;

			serv.id = i++;
			serv.Server(++it);
			servers.push_back(serv);
		}
		else if (*it == "workers")
		{
			workers = std::stoi(*(++it));
			if (workers > 16 || workers < 0)
				throw(Server::serverException("workers must be between a 0-16 range")); //range error
			if (*(++it) == ";")
				throw(Server::serverException("'workers' missing a ';'")); //missing ';' error
		}
		else
			throw(configException("main block not valid directive at %", 0, *it)); //wrong directive error
		if (servers.empty())
			throw(Server::serverException("missing server block")); //no server error
		++it;
	}

}

void Config::clear()
{
	if (fd > 0)
	{
		close(fd);
		fd = 0;
	}
	content.clear();
	tokens.clear();
}

int Config::getWorkers()
{
	return workers;
}

std::vector<ServConf> &Config::getServers()
{
	return servers;
}

std::string &Config::getPath()
{
	return path;
}

std::string &Config::getFileContent()
{
	return content;
}

bool isDirectiveVal(std::string const &str) {
  return (str == "listen" || str == "server_name" ||
		  str == "root" || str == "auth" ||
		  str == "error_page" || str == "upload" ||
		  str == "autoindex" || str == "index" ||
		  str == "cgi" || str == "cgi_bin");
}
