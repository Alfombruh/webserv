#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "webserv.h"

using std::cout;
using std::string;

class Config
{
private:
    IntVec Ports;
    string serverName;
    Redirection redirection;
    std::vector<METHOD> alowedMethods;
    string root;
    string destination;
    StrStrMap errorPages;
    std::vector<Location> locations;

public:
    Config(){};
    ~Config(){};

    const IntVec &ports() const { return Ports; }
    void setPort(const int port) { Ports.push_back(port); };

    const string &getServerName() const { return serverName; }
    void setServerName(const string &serverName) { this->serverName = serverName; }

    const Redirection &getRedirection() const { return redirection; }
    void setRedirection(const Redirection redirection) { this->redirection = redirection; }

    const std::vector<METHOD> &getAlowedMethods() const { return alowedMethods; }
    void setAlowedMethod(const METHOD method) { alowedMethods.push_back(method); }

    const string &getRoot() const { return root; }
    void setRoot(const string &root) { this->root = root; }

    const string &getDestination() const { return destination; }
    void setDestination(const string &destination) { this->destination = destination; }

    const StrStrMap &getErrorPages() const { return errorPages; }
    void setErrorPage(const string status, const string pageRoute) { errorPages[status] = pageRoute; }

    const std::vector<Location> &getLocations() const { return locations; }
    void setLocations(const Location location) { locations.push_back(location); }
};

#endif