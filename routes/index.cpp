#include "routes.h"
#include "../api/routes/api_routes.h"

static void getFavicon(Request &req, Response &res)
{
	(void)req;
	res.status(STATUS_300).img("./public/images/42-Urduliz-Logo-web.png").send();
};

bool index(Router &router)
{
	if (router.get("/favicon.ico", &getFavicon))
		return true;
	if (router.get(&get))
		return true;
	if (router.post(&post))
		return true;
	if (router.delet(&delet))
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