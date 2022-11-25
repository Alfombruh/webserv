#include "webserv.h"
#include "Response.hpp"

Response::Response(int clientId) : clientId((size_t)clientId) {}

Response::~Response() {}

size_t Response::getClientId() const { return clientId; };
