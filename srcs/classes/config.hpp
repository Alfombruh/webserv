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
	StrPair redirect;
	std::vector<METHOD> alowedMethods;
	string root;
	ssize_t maxBody;
	string destination;
	StrStrMap cgis;
	StrStrMap errorPages;
	std::vector<Location> locations;

public:
	Config() : maxBody(-1){};
	~Config(){};

	const string getMethodStr(METHOD method)
	{
		switch (method)
		{
		case 0:
			return "GET";
		case 1:
			return "POST";
		case 2:
			return "DELETE";
		default:
			return "";
		}
	};

	void printConfig(void)
	{
		for (IntVec::iterator it = Ports.begin(); it != Ports.end(); it++)
			cout << "listen " << *it << "\n";

		if (!serverName.empty())
			cout << "\nserver_name " << serverName << "\n";

		if (!redirect.first.empty())
			cout << "\nreturn " << redirect.first << " " << redirect.second << "\n";

		if (!alowedMethods.empty())
		{
			cout << "\nallow_methods ";
			for (std::vector<METHOD>::iterator it = alowedMethods.begin(); it != alowedMethods.end(); it++)
				cout << getMethodStr(*it) << " ";
			cout << "\n";
		}
		if (!root.empty())
			cout << "\nroot " << root << "\n";

		if (maxBody != -1)
			cout << "\nclient_body_limit" << maxBody << "\n";

		if (!destination.empty())
			cout << "\ndestination " << destination << "\n";

		if (!cgis.empty())
		{
			cout << "\n";
			for (StrStrMap::iterator it = cgis.begin(); it != cgis.end(); it++)
				cout << "error_page " << it->first << " " << it->second << "\n";
		}

		if (!errorPages.empty())
		{
			cout << "\n";
			for (StrStrMap::iterator it = errorPages.begin(); it != errorPages.end(); it++)
				cout << "error_page " << it->first << " " << it->second << "\n";
		}

		if (!locations.empty())
		{
			for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); it++)
			{
				cout << "\nlocation " << it->location << " {\n";
				if (!it->alowedMethods.empty())
				{
					cout << "\tallow_methods ";
					for (std::vector<METHOD>::iterator ite = it->alowedMethods.begin(); ite != it->alowedMethods.end(); ite++)
						cout << *ite << " ";
					cout << "\n";
				}
				if (!it->root.empty())
					cout << "\troot " << it->root << "\n";
				if (!it->destination.empty())
					cout << "\tdestination " << it->destination << "\n";
				if (!it->cgiInfo.first.empty())
					cout << "\tcgi_info " << it->cgiInfo.first << " " << it->cgiInfo.second << "\n";
				cout << "}\n";
			}
		}
	};

	const IntVec &getPorts() const { return Ports; }
	void setPort(const int port) { Ports.push_back(port); };

	const string &getServerName() const { return serverName; }
	void setServerName(const string &serverName) { this->serverName = serverName; }

	const StrPair &getRedirect() const { return redirect; }
	void setRedirect(const StrPair redirect) { this->redirect = redirect; }

	const std::vector<METHOD> &getAlowedMethods() const { return alowedMethods; }
	void setAlowedMethods(const std::vector<METHOD> alowedMethods) { this->alowedMethods = alowedMethods; }

	const string &getRoot() const { return root; }
	void setRoot(const string &root) { this->root = root; }

	ssize_t getMaxBody() const { return maxBody; }
	void setMaxBody(const size_t &maxBody) { this->maxBody = maxBody; }

	const string &getDestination() const { return destination; }
	void setDestination(const string &destination) { this->destination = destination; }

	const StrStrMap &getCgis() const { return cgis; }
	void setCgi(const string extension, const string binary) { errorPages[extension] = binary; }

	const StrStrMap &getErrorPages() const { return errorPages; }
	void setErrorPage(const string status, const string pageRoute) { errorPages[status] = pageRoute; }

	const std::vector<Location> &getLocations() const { return locations; }
	void setLocations(const Location location) { locations.push_back(location); }
};

#endif