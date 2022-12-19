#ifndef API_ROUTER_H
#define API_ROUTER_H

#include "webserv.h"
#include "../../srcs/classes/Router.hpp"
#include "../../srcs/classes/Request.hpp"
#include "../../srcs/classes/Response.hpp"

class Router;
class Request;

bool apiIndex(Router &router);
bool login(Router &router);

#endif