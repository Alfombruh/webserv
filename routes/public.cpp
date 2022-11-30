#include "routes.h"

static void getImg(Request &req, Response &res)
{
	res.status(STATUS_200).img("." + (req.getAbsoluteRoute())).send();
};


bool publicFolder(Router &router)
{
	if (router.get(router.getReqRoute(), &getImg))
		return true;
	// if (router.get("/", &get))
	// 	return true;
	return router.notFound();
};