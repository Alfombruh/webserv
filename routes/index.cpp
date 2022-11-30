#include "routes.h"

static void get(Request &rec, Response &res)
{
	(void)rec;
	res.status(STATUS_200).text("has llegado a index.get").send();
};

static void post(Request &rec, Response &res)
{
	(void)rec;
	res.status(STATUS_200).text("has llegado a index.post").send();
};

static void delet(Request &rec, Response &res)
{
	(void)rec;
	res.status(STATUS_200).text("has llegado a index.delete").send();
};

bool index(Router &router)
{
	if (router.use("/about", &about))
		return true;

	if (router.get("/", &get))
		return true;
	if (router.post("/", &post))
		return true;
	if (router.delet("/", &delet))
		return true;
	return router.notFound();
};