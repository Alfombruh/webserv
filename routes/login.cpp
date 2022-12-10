#include "routes.h"
#include "../SessionManager/session_container.hpp"

static void get(Request &req, Response &res)
{
	string cookie = req.getHeader("cookie");
	cookie = cookie.substr(cookie.find("=") + 1);
	if (cookie.empty())
	{
		res.status(STATUS_204).send();
		return;
	}
	session::session session = session::getSession(cookie);
	if (session.name.empty())
	{
		res.status(STATUS_204).expireCookie().send();
		return;
	}
	res.status(STATUS_302).json("{\"name\":\"" + session.name + "\",\"about\":\"" + session.about + "\"}").send();
};

static void post(Request &req, Response &res)
{
	if (req.getBody().size() != 32)
	{
		res.expireCookie().status(STATUS_204).send();
		return;
	}
	session::session session = {.hash = req.getBody(), .name = req.getUrlVar("name"), .about = req.getUrlVar("about")};
	session::createSession(session);
	res.status(STATUS_201)
		.cookie(req.getBody())
		.send();
};

static void delet(Request &req, Response &res)
{
	string cookie = req.getHeader("cookie");
	cookie = cookie.substr(cookie.find("=") + 1);
	if (cookie.empty())
	{
		res.status(STATUS_200).text("session terminated").send();
		return;
	}
	session::removeSession(cookie);
	res.status(STATUS_200).text("session terminated").send();
};

bool login(Router &router)
{
	if (router.get("/", &get))
		return true;
	if (router.post("/", &post))
		return true;
	if (router.delet("/", &delet))
		return true;
	return router.notFound();
};