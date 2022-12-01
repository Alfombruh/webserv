#include "Router.hpp"

Router::Router(Request &req, Response &res) : req(req), res(res){};

bool Router::use(const string route, bool (*callback)(Router &))
{
	if (req.isInRoute(route) == false)
		return false;
	req.updateRoute(route);
	return callback(*this);
};
bool Router::get(const string route, void (*get)(Request &, Response &)) const
{
	if (req.getMethod() != GET || req.getRoute() != route)
		return false;
	get(req, res);
	return true;
};
bool Router::post(const string route, void (*post)(Request &, Response &)) const
{
	if (req.getMethod() != POST || req.getRoute() != route)
		return false;
	post(req, res);
	return true;
};
bool Router::delet(const string route, void (*delet)(Request &, Response &)) const
{
	if (req.getMethod() != DELETE || req.getRoute() != route)
		return false;
	delet(req, res);
	return true;
};
bool Router::notFound() const
{
	if (getReqRoute() == "/")
		res.status(STATUS_405).text(req.getMethodStr() + " method is not supported for " + req.getAbsoluteRoute()).send();
	else
		res.status(STATUS_404).text(req.getAbsoluteRoute() + " does not exist").send();
	return false;
};

const string Router::getReqRoute() const
{
	return req.getRoute();
}
