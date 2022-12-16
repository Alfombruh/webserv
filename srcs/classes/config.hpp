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

    const void printConfig(void)
    {
        for (IntVec::iterator it = Ports.begin(); it != Ports.end(); it++)
            cout << "Ports are: " << *it << "\n";

        cout << "Server Name: " << serverName << "\n";

        cout << "Redirections part {\n" << "redi.status: " << redirection.status << "\n" << "redi.route: " << redirection.route << "\n" << "redi.status: " << redirection.redirect << "\n}\n";

        cout << "The allowed methods are: ";
        for (std::vector<METHOD>::iterator it = alowedMethods.begin(); it != alowedMethods.end(); it++)
            cout << *it << " ";
        cout << "\n";

        cout << "Root is: " << root << "\n";

        cout << "The Limit of bodySize is: " << maxBody << "\n";

        cout << "Destination is: " << destination << "\n";

        cout << "Error Pages are:\n";
        for (StrStrMap::iterator it = errorPages.begin(); it != errorPages.end(); it++)
            cout << it->first << " " << it->second << "\n";
        
        cout << "Redirects are: " << redirect.first << " && " << redirect.second << "\n";

        for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); it++)
        {
            cout << "Location is: " << it->location << "\n";
            cout << "\tMethods are:";
            for (std::vector<METHOD>::iterator ite = it->alowedMethods.begin(); ite !=  it->alowedMethods.end(); ite++)
                cout << *ite << " ";
            cout << "\n";
            cout << "Root is: " << it->root << "\n";
            cout << "Destination is: " << it->destination << "\n";
            cout << "CGI info: " << it->cgiInfo.first << " && " << it->cgiInfo.second << "\n";
        }
        
    };

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