#include "routes.h"


static void get(Request &req, Response &res)
{
	(void)req;
	res.status(STATUS_300).html("./public/html/gallery.html").send();
};

static void post(Request &req, Response &res)
{
	string contentType = req.getHeader("content-type");
	if (contentType.empty()|| (contentType != "image/png" && contentType != "image/jpg"))
	{
		res.status(STATUS_451).text("content-type must be image/png or image/jpg").send();
		return;
	}
	string value = req.getUrlVar("filename");
	if (value.empty())
	{
		res.status(STATUS_206).text("specify a filename: url?filename=exaple-filename").send();
		return;
	}
	if (req.getBody().empty())
	{
		res.status(STATUS_204).text("body is empty").send();
		return;
	}
	string filename = value;
	std::ifstream found(("image_gallery/" + filename).c_str());
	if (found.good())
	{
		res.status(STATUS_409).text("filename: " + filename + " allready exists").send();
		return;
	}
	std::ofstream file("image_gallery/" + filename, std::ofstream::binary | std::ofstream::out);
	file.write(req.getBody().c_str(), req.getBody().size());
	file.close();
	res.status(STATUS_201).text("filename: " + filename + " uploaded").send();
};

static void delet(Request &req, Response &res)
{
	string filename = req.getUrlVar("filename");
	if (filename.empty() || (filename.find(".png") == string::npos && filename.find(".jpg") == string::npos))
	{
		res.status(STATUS_206).text("specify a filename: url?filename=exaple-filename.png").send();
		return;
	}
	std::ifstream found(("image_gallery/" + filename).c_str());
	if (found.good())
	{
		std::remove(("image_gallery/" + filename).c_str());
		res.status(STATUS_200).text("filename: " + filename + " deleted").send();
		return;
	}
	res.status(STATUS_404).text("filename: " + filename + " does not exist").send();
};

bool gallery(Router &router)
{
	if (router.get("/", &get))
		return true;
	if (router.post("/", &post))
		return true;
	if (router.delet("/", &delet))
		return true;
	return router.notFound();
};