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
	size_t maxBody;
	string destination;
	StrStrMap errorPages;
	StrPair redirect;
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
	void setAlowedMethods(const std::vector<METHOD> alowedMethods) { this->alowedMethods = alowedMethods; }

	const string &getRoot() const { return root; }
	void setRoot(const string &root) { this->root = root; }

	const size_t getMaxBody() const { return maxBody; }
	void setMaxBody(const size_t &maxBody) { this->maxBody = maxBody; }

	const string &getDestination() const { return destination; }
	void setDestination(const string &destination) { this->destination = destination; }

	const StrStrMap &getErrorPages() const { return errorPages; }
	void setErrorPage(const string status, const string pageRoute) { errorPages[status] = pageRoute; }

	const StrPair &getRedirect() const { return redirect; }
	void setRedirect(const StrPair redirect) { this->redirect = redirect; }

	const std::vector<Location> &getLocations() const { return locations; }
	void setLocations(const Location location) { locations.push_back(location); }
};

#endif