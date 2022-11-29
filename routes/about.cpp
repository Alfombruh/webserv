#include "routes.h"

static void get(Request &rec, Response &res)
{
	(void)rec;
	res.text("has llegado a about.get");
};

static void post(Request &rec, Response &res)
{
	(void)rec;
	res.text("has llegado a about.post");
};

static void delet(Request &rec, Response &res)
{
	(void)rec;
	res.text("has llegado a about.delete");
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