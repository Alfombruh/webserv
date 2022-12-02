#include "config.hpp"

Config::Config(std::string &path) : path(path), workers(0), fd(open(path, .c_str(), O_RDONLY))
{
	if (fd < 0)
		throw(configException("couldn't open configuration file : %", 0, path)); //thow an error open
}

Config::~Config()
{
	clear();
}

void Config::tokenize()
{
	std::string line;
	std::string temp;
	std::string::size_type first;
	std::string::size_type last;
	std::stack<bool> brackets;
	int line_count = 1;
	char *line_c;

	while (get_next_line(fd, line_c)) //change gnl will not use it
	{
		line = line_c;
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
					throw(configException("extra closing  on line  %", 0, line_count.to_string())); //thow extra bracket error
				brackets.pop();
			}	
			if (isDirectiveVal(temp) && line[line.find_last_not_of(" \t", line.length())] != ';') //isDirectiveVal() wip(havent done it yet)
				throw(configException("missing ';' line %", 0, line_count.to_string()));																			  //throw missing ; error
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
		free(line_c);
	}
	free(line_c);
}

void Config::parse()
{
	tokenize();
	//do the rest of de parsing
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

std::vector<ServerConfig> &Config::getServers()
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
  return (str == "listen" ||
          str == "server_name" ||
          str == "root" ||
          str == "auth" ||
          str == "error_page" ||
          str == "upload" ||
          str == "autoindex" ||
          str == "index" ||
          str == "cgi" ||
          str == "cgi_bin");
}
