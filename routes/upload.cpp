#include "routes.h"

static void get(Request &req, Response &res)
{
	(void)req;
	res.status(STATUS_300).html("./public/html/upload.html").send();
};

static void post(Request &req, Response &res)
{
	StrStrMap headers = req.getHeaders();
	for (StrStrMap::iterator it = headers.begin(); it != headers.end(); it++)
	{
		if (it->first == "content-type" && it->second != "image/png" && it->second != "image/jpg")
		{
			res.status(STATUS_451).text("only image/png or image/jpg").send();
			return;
		}
	}
	std::ofstream file("picture.png", std::ofstream::binary | std::ofstream::out);
	file.write(req.getBody().c_str(), req.getBody().size());
	file.close();
	res.status(STATUS_200).text("has llegado a upload.post").send();
};

static void delet(Request &req, Response &res)
{
	(void)req;
	res.status(STATUS_200).text("has llegado a upload.delete").send();
};

bool upload(Router &router)
{
	if (router.get("/", &get))
		return true;
	if (router.post("/", &post))
		return true;
	if (router.delet("/", &delet))
		return true;
	return router.notFound();
};