#include "routes.h"

static void get(Request &req, Response &res)
{
	(void)req;
	res.status(STATUS_300).text_python(req.getAbsoluteRoute());
};


bool cgi(Router &router)
{

	if (router.get(router.getReqRoute(), &get))
		return true;
	return router.notFound();
};