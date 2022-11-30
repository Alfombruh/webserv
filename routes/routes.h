#ifndef ROUTER_H
#define ROUTER_H

// #include "webserv.h"
#include "../srcs/classes/Router.hpp"
#include "../srcs/classes/Request.hpp"
#include "../srcs/classes/Response.hpp"
// #include "../srcs/classes/Server.hpp"

class Router;

bool about(Router &router);
bool index(Router &router);
bool profile(Router &router);
bool publicFolder(Router &router);

#endif