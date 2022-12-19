#include "routes.h"
#include "../api/routes/api_routes.h"

// static void get(Request &req, Response &res)
// {
// 	(void)req;
// 	res.status(STATUS_200).html("./public/html/index.html").send();
// };
static void get(Request &req, Response &res, const string filePath)
{
	(void)req;
	string extension = filePath.substr(filePath.rfind('.'));

	if(extension == ".html")
		res.status(STATUS_200).html(filePath).send();
	else if(extension == ".css")
		res.status(STATUS_200).css(filePath).send();
	else if(extension == ".js")
		res.status(STATUS_200).js(filePath).send();
	else if(extension == ".png" || extension == ".jpg")
		res.status(STATUS_200).img(filePath).send();
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

bool index(Router &router)
{
	router.create_env(&parseEnv);
	if (router.get("/favicon.ico", &getFavicon))
		return true;
	if (router.get(&get))
		return true;
	if (router.post("/", &post))
		return true;
	if (router.delet("/", &delet))
		return true;
	return router.notFound();
	// if (router.use("/public", &publicFolder))
	// 	return true;
	// if (router.use("/gallery", &gallery))
	// 	return true;
	// if (router.use("/cgi-bin", &cgi))
	// 	return true;
	// if (router.use("/directory", &useDirect))
	// 	return true;
	// if (router.use("/YoupiBanane", &yupi))
	// 	return true;
};