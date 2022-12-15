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

void ConfigParser::parseLocation(const string line, Config &config)
{
    Location location;

    cout << "Line is:\t" << line << "\n"; 
    location.location = line.substr(0, line.find(" "));
    cout << "location:" << location.location << "$\n";
    config.setLocations(location);
    cout << "Firsts location: " << config.getLocations()[0].location << "\n";
};

void ConfigParser::parseLine(const string line, Config &config)
{
    if (9 < line.size() && line.substr(0, 9) == "location ")
    {
        parseLocation(line.substr(9), config);
        return;
    }
    // cout << "line: $" << line << "$\n";
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
const char *ConfigParser::configParseException::what() const throw()
{
    // return (strcat("Server Error: ", msg));
    return (msg);
};