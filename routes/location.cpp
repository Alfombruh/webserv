#include "routes.h"
#include "../api/routes/api_routes.h"

// IMAGES
static void getImg(Request &req, Response &res)
{
	res.status(STATUS_300).img("." + (req.getAbsoluteRoute())).send();
};
static bool images(Router &router)
{
	if (router.get(router.getReqRoute(), &getImg))
		return true;
	return router.notFound();
};

// CSS
static void getCss(Request &req, Response &res)
{
	res.status(STATUS_300).css("." + (req.getAbsoluteRoute())).send();
};
static bool css(Router &router)
{
	if (router.get(router.getReqRoute(), &getCss))
		return true;
	return router.notFound();
};

// JAVASCRIPT
static void getJs(Request &req, Response &res)
{
	res.status(STATUS_300).js("." + (req.getAbsoluteRoute())).send();
};
static bool js(Router &router)
{
	if (router.get(router.getReqRoute(), &getJs))
		return true;
	return router.notFound();
};

static void get(Request &req, Response &res, const string filePath)
{
	(void)req;
	string extension = filePath.substr(filePath.rfind('.'));
	if (extension == ".html")
		res.status(STATUS_200).html(filePath).send();
	else if (extension == ".css")
		res.status(STATUS_200).css(filePath).send();
	else if (extension == ".js")
		res.status(STATUS_200).js(filePath).send();
	else if (extension == ".png" || extension == ".jpg")
		res.status(STATUS_200).img(filePath).send();
};

bool location(Router &router, Location &location)
{
	// cout << "holaaaa:"<< router.getReqRoute() << "\n";
	if (location.api.empty() == false)
		return apiIndex(router);
	if (router.get(location, &get))
		return true;
	// if (router.post(location, &get))
	// 	return true;
	cout << "route:" << router.getReqRoute() << "$\n";
	if (router.use("/images", &images))
		return true;
	if (router.use("/css", &css))
		return true;
	if (router.use("/js", &js))
		return true;
	router.notFound();
	return true; 
};