#ifndef ROUTER_H
#define ROUTER_H

#include "webserv.h"
#include "../srcs/classes/Router.hpp"
#include "../srcs/classes/Request.hpp"
#include "../srcs/classes/Response.hpp"

class Router;
class Request;

bool index(Router &router);
bool login(Router &router);
bool gallery(Router &router);
bool location(Router &router, Location &location);
bool publicFolder(Router &router);
bool cgi(Router &router);

//ENDPOINTS
void get(Request &req, Response &res, const string filePath);
void post(Request &req, Response &res, const string filePath, bool);
void delet(Request &req, Response &res, const string filePath, bool);

//TESTER
bool useDirect(Router &router);
bool yupi(Router &router);

#endif