#include "routes.h"

static void get(Request &rec, Response &res)
{
	(void)rec;
	res.status(STATUS_200).text("has llegado a about.get").send();
};

static void post(Request &rec, Response &res)
{
	(void)rec;
	res.status(STATUS_200).text("has llegado a about.post").send();
};

static void delet(Request &rec, Response &res)
{
	(void)rec;
	res.status(STATUS_200).text("has llegado a about.delete").send();
};

bool about(Router &router)
{
	if (router.get("/", &get))
		return true;
	if (router.post("/", &post))
		return true;
	if (router.delet("/", &delet))
		return true;
	return false;
};