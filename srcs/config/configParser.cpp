#include "configParser.hpp"
#include <fstream>

ConfigParser::ConfigParser(const string configPath)
{
	std::ifstream configFile(configPath);
	if (!configFile.is_open())
		throw ConfigParseException("configuration file not found");
	string rawConfig((std::istreambuf_iterator<char>(configFile)), std::istreambuf_iterator<char>());
	rawConfig = trimSpaces(rawConfig);
	size_t i = 0;
	int bracket = 0;
	while (i < rawConfig.size())
	{
		if (rawConfig[i] == '#')
			i = rawConfig.find('\n', i) == string::npos ? rawConfig.size() : rawConfig.find('\n', i)  + 1;
		if (i >= rawConfig.size())
			break;
		if (i < rawConfig.size() && rawConfig[i] == '{')
			bracket++;
		if (i < rawConfig.size() && rawConfig[i] == '}')
		{
			bracket--;
			if (bracket == 0)
			{
				rawServer.push_back(rawConfig[i++]);
				continue;
			}
			if (bracket < 0)
				throw ConfigParseException(KYS);
		}
		if (bracket > 0 && rawConfig[i] == '\n')
		{
			if (i + 1 < rawConfig.size() && rawConfig[i + 1] == '\n')
				rawServer.push_back(rawConfig[i++]);
			i++;
			continue;
		}
		rawServer.push_back(rawConfig[i++]);
	}
	if (bracket != 0)
		throw ConfigParseException(KYS);
	if (checkSemiColon(rawServer) == FAILED)
		throw ConfigParseException("Cheack out semicolons");
	parseServer();
}

ConfigParser::~ConfigParser(){};

bool ConfigParser::checkSemiColon(string rawConfig) const
{
	size_t i = 0;

	while (++i < rawConfig.size())
	{
		if (rawConfig[i] != '\n')
			continue;
		if (rawConfig[i - 1] != '\n' && rawConfig[i - 1] != ';' &&
			rawConfig[i - 1] != '{' && rawConfig[i - 1] != '}' && rawConfig[i - 1] != '\t')
			return false;
	}
	return true;
};

string ConfigParser::trimSpaces(string rawConfig) const
{
	string trimmed;
	size_t i = 0;

	while (i < rawConfig.size() && (rawConfig[i] == ' ' || rawConfig[i] == '\n'))
		i++;
	while (i < rawConfig.size())
	{
		while (i + 1 < rawConfig.size() && rawConfig[i] == ' ' && rawConfig[i + 1] == ' ')
			i++;
		trimmed.push_back(rawConfig[i++]);
	}
	rawConfig = trimmed;
	trimmed.clear();
	i = 0;
	while (i < rawConfig.size())
	{
		if (i + 1 < rawConfig.size() && rawConfig[i] == '\n' && rawConfig[i + 1] == ' ')
		{
			trimmed.push_back(rawConfig[i]);
			i += 2;
			continue;
		}
		trimmed.push_back(rawConfig[i++]);
	}
	return trimmed;
};

void ConfigParser::parseServer()
{
	string tmp;
	for (size_t i = 0; i < rawServer.size(); i++)
	{
		if ((i > 0 && rawServer.at(i) == '\n' && rawServer.at(i - 1) == '\n') || rawServer.at(i) == '\t')
			continue;
		tmp.push_back(rawServer.at(i));
	}

	rawServer = tmp;
	size_t i = 0;
	size_t copySize;
	while (i < rawServer.size())
	{
		copySize = rawServer.find("\n", i) == string::npos ? rawServer.size() : rawServer.find("\n", i) - i;
		parseLine(rawServer.substr(i, copySize));
		i += (copySize + 1);
	}
};

std::vector<METHOD> ConfigParser::parseMethods(const string line)
{
	std::vector<METHOD> allowedMethods;
	string method;
	size_t i = 0;

	if (line.empty())
	{
		allowedMethods.push_back(GET);
		allowedMethods.push_back(POST);
		allowedMethods.push_back(DELETE);
		return allowedMethods;
	}
	while (i <= line.size())
	{
		if (line[i] != ' ' && i < line.size())
		{
			method.push_back(line[i++]);
			continue;
		}
		if (method == "GET")
			allowedMethods.push_back(GET);
		else if (method == "POST")
			allowedMethods.push_back(POST);
		else if (method == "DELETE")
			allowedMethods.push_back(DELETE);
		else
			throw ConfigParseException("Method not suported");
		i++;
		method.clear();
	}
	return allowedMethods;
}

string ConfigParser::parseVar(const string line, const string key)
{
	string tmp;
	size_t pos;

	if ((pos = line.find(key)) == string::npos)
		return "";

	pos += (key.size() + 1);
	tmp = line.substr(pos);
	return tmp.substr(0, tmp.find(";"));
}

void ConfigParser::parseLocation(const string line)
{
	Location location = {.maxBody = -1};
	string cgiInfo;

	location.location = line.substr(0, line.find(" "));
	location.alowedMethods = parseMethods(parseVar(line, "allow_methods"));
	location.index = parseVar(line, "index");
	location.root = parseVar(line, "root");
	location.maxBody = stringToSize_t(parseVar(line, "client_body_limit"));
	location.api = parseVar(line, "api");
	location.destination = parseVar(line, "destination");
	if (!(cgiInfo = parseVar(line, "cgi_info")).empty())
	{
		location.cgiInfo.first = cgiInfo.substr(0, cgiInfo.find(" "));
		location.cgiInfo.second = cgiInfo.substr(cgiInfo.find(" ") + 1, cgiInfo.find(";"));
	}
	configuration.setLocations(location);
};

void ConfigParser::parseLine(const string line)
{
	if (9 < line.size() && line.substr(0, 9) == "location ")
	{
		parseLocation(line.substr(9));
		return;
	}
	if (line.find("{") != string::npos || line.find("}") != string::npos)
		throw ConfigParseException("Only location brackets allowed");
	string key = line.substr(0, line.find(" "));
	string tmpValue;

	if (key == "listen")
		configuration.setPort(stringToSize_t(parseVar(line, "listen")));
	else if (key == "server_name")
		configuration.setServerName(parseVar(line, "server_name"));
	else if (key == "allow_methods")
		configuration.setAlowedMethods(parseMethods(parseVar(line, "allow_methods")));
	else if (key == "destination")
		configuration.setDestination(parseVar(line, "destination"));
	else if (key == "client_body_limit")
		configuration.setMaxBody(stringToSize_t(parseVar(line, "client_body_limit")));
	else if (key == "root")
		configuration.setRoot(parseVar(line, "root"));
	else if (key == "index")
		configuration.setIndex(parseVar(line, "index"));
	else if (!(tmpValue = parseVar(line, "cgi")).empty())
		configuration.setCgi(tmpValue.substr(0, tmpValue.find(" ")),
							 tmpValue.substr(tmpValue.find(" ") + 1, tmpValue.find(";")));
	else if (!(tmpValue = parseVar(line, "error_page")).empty())
		configuration.setErrorPage(tmpValue.substr(0, tmpValue.find(" ")),
								   tmpValue.substr(tmpValue.find(" ") + 1, tmpValue.find(";")));
	else if (!(tmpValue = parseVar(line, "return")).empty())
		configuration.setRedirect(std::make_pair(tmpValue.substr(0, tmpValue.find(" ")),
												 tmpValue.substr(tmpValue.find(" ") + 1, tmpValue.find(";"))));
};

// GETTERS

const Config &ConfigParser::getConfiguration(void) const
{
	return configuration;
};

ConfigParser::ConfigParseException::ConfigParseException(const char *msg) : msg((char *)msg){};
const char *ConfigParser::ConfigParseException::what() const throw() { return (msg); };