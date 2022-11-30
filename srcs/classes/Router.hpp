#ifndef ROUTER_HPP
#define ROUTER_HPP

// #include "webserv.h"
#include "Request.hpp"
#include "Response.hpp"
// #include "Server.hpp"

class Router
{
	// static std::vector<string> routes;
	Request &req;
	Response &res;

public:
	Router(Request &req, Response &res);
	// Router(string configPath);
	~Router(){};

	bool use(const string route, bool (*func)(Router &));
	bool get(const string route, void (*get)(Request &, Response &)) const;
	bool post(const string route, void (*post)(Request &, Response &)) const;
	bool delet(const string route, void (*delet)(Request &, Response &)) const;
	bool notFound() const;
	const string getReqRoute() const;
};

#endif
