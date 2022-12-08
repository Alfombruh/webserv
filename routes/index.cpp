#include "routes.h"

static void get(Request &req, Response &res)
{
	(void)req;
	res.status(STATUS_200).html("./public/html/index.html").send();
};

static void post(Request &req, Response &res)
{
	(void)req;
	res.status(STATUS_405).send();
	// if(req.getBody().empty())
	// {
	// 		return;
	// }
	// res.status(STATUS_200).text("has llegado a index.post").send();
};

static void delet(Request &req, Response &res)
{
	(void)req;
	res.status(STATUS_200).text("has llegado a index.delete").send();
};

static void getFavicon(Request &req, Response &res)
{
	(void)req;
	res.status(STATUS_300).img("./public/images/42-Urduliz-Logo-web.png").send();
};

static void direct(Request &req, Response &res)
{
	(void)req;
	cout << "directory\n";
	res.status(STATUS_301).redirect("/YoupiBanane").send();
};

static void yupi(Request &req, Response &res)
{
	(void)req;
	res.status(STATUS_200).send();
};

bool index(Router &router)
{
	if (router.use("/public", &publicFolder))
		return true;
	if (router.use("/galery", &galery))
		return true;
	if (router.use("/login", &login))
		return true;
	if (router.get("/favicon.ico", &getFavicon))
		return true;
	if (router.use("/cgi-bin", &cgi))
		return true;
	if (router.get("/directory", &direct))
		return true;
	if (router.get("/YoupiBanane", &yupi))
		return true;
	if (router.get("/", &get))
		return true;
	if (router.post("/", &post))
		return true;
	if (router.delet("/", &delet))
		return true;
	return router.notFound();
};