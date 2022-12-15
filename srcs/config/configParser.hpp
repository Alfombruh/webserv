#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include <iostream>
#include <exception>
#include "../classes/config.hpp"

using std::cout;
using std::string;

class Config;

class ConfigParser
{
private:
    std::vector<Config> configurations;
    StrVec rawServers;
    size_t totalServers;
    void parseServers();
    void parseLocation(const string line, Config &config);
    void parseLine(const string line, Config &config);

public:
    ConfigParser(const string configPath);
    ~ConfigParser();

    // GETTERS
    Config &parser(size_t num);
    size_t getServerAmmount() const;
    const Config &getConfigAt(const size_t index) const;
    class configParseException : public std::exception
    {
        char *msg;

    public:
        configParseException(const char *msg);
        virtual const char *what() const throw();
    };
};

#endif