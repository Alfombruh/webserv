#include "routes.h"

//IMAGES
static void getImg(Request &req, Response &res)
{
	res.status(STATUS_300).img("." + (req.getAbsoluteRoute())).send();
};
bool images(Router &router)
{
	if (router.get(router.getReqRoute(), &getImg))
		return true;
	return router.notFound();
};

//CSS
static void getCss(Request &req, Response &res)
{
	res.status(STATUS_300).css("." + (req.getAbsoluteRoute())).send();
};
bool css(Router &router)
{
	if (router.get(router.getReqRoute(), &getCss))
		return true;
	return router.notFound();
};

//JAVASCRIPT
static void getJs(Request &req, Response &res)
{
	res.status(STATUS_300).js("." + (req.getAbsoluteRoute())).send();
};
bool js(Router &router)
{
	if (router.get(router.getReqRoute(), &getJs))
		return true;
	return router.notFound();
};

bool publicFolder(Router &router)
{
	if (router.use("/images", &images))
		return true;
	if (router.use("/css", &css))
		return true;
	if (router.use("/js", &js))
		return true;
	return router.notFound();
};