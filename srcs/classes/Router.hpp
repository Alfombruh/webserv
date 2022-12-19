#ifndef ROUTER_HPP
#define ROUTER_HPP

// #include "webserv.h"
#include "config.hpp"
#include "Request.hpp"
#include "Response.hpp"
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

	bool create_env(void (*create_env)(Request &, Response &)) const;
	bool methodAllowed(std::vector<METHOD> methods) const;
	string routeFile(const std::string &route) const;
	bool useLocations(std::vector<Location> &locations, bool (*func)(Router &, Location &));
	bool accepExtension(const string extension) const;
	bool use(const string route, bool (*func)(Router &));
	bool get(const string route, void (*get)(Request &, Response &)) const;
	bool get(void (*get)(Request &, Response &, string)) const;
	bool get(const Location &, void (*get)(Request &, Response &, string)) const;
	bool post(const string route, void (*post)(Request &, Response &)) const;
	bool delet(const string route, void (*delet)(Request &, Response &)) const;
	bool notFound() const;
	const string getReqRoute() const;
};

#endif
