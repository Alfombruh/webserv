#include "configParser.hpp"
#include <fstream>

ConfigParser::ConfigParser(const string configPath) : totalServers(0)
{
	std::ifstream configFile(configPath);
	string rawConfig((std::istreambuf_iterator<char>(configFile)), std::istreambuf_iterator<char>());
	size_t i = 0;
	int bracket = 0;

	while (i < rawConfig.size())
	{
		if (rawConfig[i] == '#')
			i = rawConfig.find('\n', i) + 1;
		if (rawConfig[i] == '\n')
			i++;
		if (i + 8 < rawConfig.size() && rawConfig.substr(i, 8) == "server {")
		{
			if (bracket != 0)
				throw configParseException("you fucking retarded piece of shit next time you better write the server part in the configFile");
			i += 9;
			totalServers++;
			rawServers.push_back("");
			bracket++;
		}
		if (totalServers == 0)
			throw configParseException("you fucking retarded piece of shit next time you better write the server part in the configFile");
		if (i >= rawConfig.size())
			break;
		if (i < rawConfig.size() && rawConfig[i] == '{')
			bracket++;
		if (i < rawConfig.size() && rawConfig[i] == '}')
		{
			bracket--;
			if (bracket == 0)
			{
				i++;
				continue;
			}
			if (bracket < 0)
				throw configParseException("at this point kill yourself. In your next life make sure you close your brackets correctly");
		}
		if (rawConfig[i] == '\t')
		{
			if (i + 1 < rawConfig.size() && rawConfig[i + 1] == '\t')
				rawServers[totalServers - 1].push_back(rawConfig[i++]);
			i++;
			continue;
		}
		rawServers[totalServers - 1].push_back(rawConfig[i++]);
	}
	if (bracket != 0)
		throw configParseException("at this point kill yourself. In your next life make sure you close your brackets correctly");
	parseServers();
}

ConfigParser::~ConfigParser(){};

void ConfigParser::parseServers()
{
	for (StrVec::iterator it = rawServers.begin(); it != rawServers.end(); it++)
	{
		// cout << "$$$$$$$\n" << *it << "$$$$$$$\n";
		Config config;
		size_t i = 0;
		size_t copySize;
		while (i < it->size())
		{
			copySize = it->find("\n", i) == string::npos ? it->size() : it->find("\n", i) - i;
			parseLine(it->substr(i, copySize), config);
			i += (copySize + 1);
		}
		configurations.push_back(config);
	}
};

std::vector<METHOD> ConfigParser::parseMethods(const string line)
{
	std::vector<METHOD> allowedMethods;
	string method;
	size_t i = 0;
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
			throw configParseException("Method not suported");
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

void ConfigParser::parseLocation(const string line, Config &config)
{
	Location location;
	string cgiInfo;

	location.location = line.substr(0, line.find(" "));
	location.alowedMethods = parseMethods(parseVar(line, "allow_methods"));
	location.root = parseVar(line, "root");
	location.destination = parseVar(line, "destination");
	if (!(cgiInfo = parseVar(line, "cgi_info")).empty())
	{
		location.cgiInfo.first = cgiInfo.substr(0, cgiInfo.find(" "));
		location.cgiInfo.second = cgiInfo.substr(cgiInfo.find(" ") + 1, cgiInfo.find(";"));
	}
	config.setLocations(location);
};

void ConfigParser::parseLine(const string line, Config &config)
{
	if (9 < line.size() && line.substr(0, 9) == "location ")
	{
		parseLocation(line.substr(9), config);
		return;
	}

	string key = line.substr(0, line.find(" "));
	string tmpValue;

	if (key == "listen")
		config.setPort(stringToSize_t(parseVar(line, "listen")));
	else if (key == "server_name")
		config.setServerName(parseVar(line, "server_name"));
	else if (key == "allow_methods")
		config.setAlowedMethods(parseMethods(parseVar(line, "allow_methods")));
	else if (key == "destination")
		config.setAlowedMethods(parseMethods(parseVar(line, "destination")));
	else if (key == "client_body_limit")
		config.setMaxBody(stringToSize_t(parseVar(line, "client_body_limit")));
	else if (key == "root")
		config.setRoot(parseVar(line, "root"));
	else if (!(tmpValue = parseVar(line, "error_page")).empty())
		config.setErrorPage(tmpValue.substr(0, tmpValue.find(" ")),
							tmpValue.substr(tmpValue.find(" ") + 1, tmpValue.find(";")));
	else if (!(tmpValue = parseVar(line, "return")).empty())
		config.setRedirect(std::make_pair(tmpValue.substr(0, tmpValue.find(" ")),
										  tmpValue.substr(tmpValue.find(" ") + 1, tmpValue.find(";"))));
};

// GETTERS
size_t ConfigParser::getServerAmmount() const
{
	return configurations.size();
};
const Config &ConfigParser::getConfigAt(const size_t index) const
{
	return configurations.at(index);
};

ConfigParser::configParseException::configParseException(const char *msg) : msg((char *)msg){};
const char *ConfigParser::configParseException::what() const throw() { return (msg); };