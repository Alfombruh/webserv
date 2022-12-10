#ifndef ROUTER_H
#define ROUTER_H

#include "webserv.h"
#include "../srcs/classes/Router.hpp"
#include "../srcs/classes/Request.hpp"
#include "../srcs/classes/Response.hpp"

class Router;

bool index(Router &router);
bool login(Router &router);
bool galery(Router &router);
bool publicFolder(Router &router);
bool cgi(Router &router);
void parse_env(Request &req, Response &res);

#endif