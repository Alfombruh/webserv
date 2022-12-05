#include "routes.h"

static void get(Request &req, Response &res)
{
	(void)req;
	res.status(STATUS_300).html("./public/html/upload.html").send();
};

static void post(Request &req, Response &res)
{
	cout << "Body: " << req.getBody() << "\n";
	StrStrMap headers = req.getHeaders();
	if (headers.find("content-type") == headers.end() ||
		(headers.at("content-type") != "image/png" && headers.at("content-type") != "image/jpg"))
	{
		cout << "headers: " << headers.at("content-type") << "\n";
		res.status(STATUS_451).text("content-type must be image/png or image/jpg").send();
		return;
	}
	const char *value = req.getUrlVar("filename");
	if (value == NULL)
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
	filename += headers.at("content-type") == "image/png" ? ".png" : ".jpg";
	if (access(("image_galery/" + filename).c_str(), F_OK) != -1)
	{
		res.status(STATUS_409).text("filename: " + filename + " allready exists").send();
		return;
	}
	std::ofstream file("image_galery/" + filename, std::ofstream::binary | std::ofstream::out);
	file.write(req.getBody().c_str(), req.getBody().size());
	file.close();
	res.status(STATUS_201).text("filename: " + filename + " uploaded").send();
};

static void delet(Request &req, Response &res)
{
	const char *value = req.getUrlVar("filename");
	string filename = value ? value : "";
	if (value == NULL || (filename.find(".png") == string::npos && filename.find(".jpg") == string::npos))
	{
		res.status(STATUS_206).text("specify a filename: url?filename=exaple-filename.png").send();
		return;
	}
	if (access(("image_galery/" + filename).c_str(), F_OK) != -1)
	{
		remove(("image_galery/" + filename).c_str());
		res.status(STATUS_200).text("filename: " + filename + " deleted").send();
		return;
	}
	res.status(STATUS_200).text("filename: " + filename + " does not exist").send();
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