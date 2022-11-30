#include "routes.h"

static void get(Request &req, Response &res)
{
	(void)req;
	res.status(STATUS_200).text("has llegado a profile.get").send();
};

static void post(Request &req, Response &res)
{
	(void)req;
	res.status(STATUS_200).text("has llegado a profile.post").send();
};

static void delet(Request &req, Response &res)
{
	(void)req;
	res.status(STATUS_200).text("has llegado a profile.delete").send();
};

bool profile(Router &router)
{
	if (router.get("/", &get))
		return true;
	if (router.post("/", &post))
		return true;
	if (router.delet("/", &delet))
		return true;
	return false;
};