#include "routes.h"

static void get(Request &req, Response &res)
{
	(void)req;
	// res.status(STATUS_200).text("has llegado a index.get").send();
	res.status(STATUS_300).html("./public/html/index.html").send();
};

static void post(Request &req, Response &res)
{
	(void)req;
	res.status(STATUS_200).text("has llegado a index.post").send();
};

static void delet(Request &req, Response &res)
{
	(void)req;
	res.status(STATUS_200).text("has llegado a index.delete").send();
};

bool index(Router &router)
{
	if (router.use("/public", &publicFolder))
		return true;
	if (router.use("/upload", &upload))
		return true;
	if (router.use("/cgi-bin", &cgi))
		return true;
	if (router.get("/", &get))
		return true;
	if (router.post("/", &post))
		return true;
	if (router.delet("/", &delet))
		return true;
	return router.notFound();
};