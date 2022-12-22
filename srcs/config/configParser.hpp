#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#define KYS "at this point kill yourself. In your next life make sure you close your brackets correctly"

#include "../classes/config.hpp"
#include <iostream>
#include <exception>

using std::cout;
using std::string;

class ConfigParser
{
private:
	Config              configuration;
	string              rawServer;
	void                parseServer();
	string              parseVar(const string line, const string key);
	void                parseLocation(const string line);
	void                parseLine(const string line);
    void                errorChecking(void);
    bool                missingElements(void);
    void                locationElements(void);
	string              trimSpaces(string rawConfig) const;
	std::vector<METHOD> parseMethods(const string line);

public:
	ConfigParser(const string configPath);
	~ConfigParser();

	// GETTERS
	const Config &getConfiguration(void) const;
	class ConfigParseException : public std::exception
	{
		char *msg;

	public:
		ConfigParseException(const char *msg);
		virtual const char *what() const throw();
	};
};

#endif