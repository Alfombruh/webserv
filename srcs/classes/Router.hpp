#ifndef ROUTER_HPP
#define ROUTER_HPP

// #include "webserv.h"
#include "config.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include <arpa/inet.h>


// #include "Server.hpp"

class Response;
class Config;

class Router
{
	// static std::vector<string> routes;
	Request &req;
	Response &res;

public:
	Router(Request &req, Response &res);
	// Router(string configPath);
	~Router(){};

	bool methodAllowed(std::vector<METHOD> methods) const;
	string routeFile(const std::string &route) const;
	bool useLocations(std::vector<Location> &locations, bool (*func)(Router &, Location &));
	bool apiUse(const string route, bool (*callback)(Router &));
	bool use(const string route, bool (*func)(Router &));

	bool accepExtension(const string extension) const;
	bool fileExists(const string &filePath) const;

	bool get(const string route, void (*get)(Request &, Response &)) const;
	bool get(void (*get)(Request &, Response &, string)) const;
	bool get(const Location &, void (*get)(Request &, Response &, string)) const;

	bool post(const string route, void (*post)(Request &, Response &)) const;
	bool post(void (*post)(Request &, Response &, string, bool)) const;
	bool post(const Location &, void (*post)(Request &, Response &, string, bool)) const;

	bool delet(const string route, void (*delet)(Request &, Response &)) const;
	bool delet(void (*delet)(Request &, Response &, string, bool)) const;
	bool delet(const Location &, void (*delet)(Request &, Response &, string, bool)) const;

	bool notFound() const;
	const string getReqRoute() const;

	void parseEnv();
};

#endif
