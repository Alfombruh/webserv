#include "routes.h"

static bool fileExists(const string &filePath)
{
	std::ifstream found(filePath.c_str());
	return found.good();
}

void get(Request &req, Response &res, const string filePath)
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

void post(Request &req, Response &res, const string filePath)
{
	string contentType = req.getHeader("content-type");
	string extension = filePath.substr(filePath.rfind('.'));
	string filename = filePath.substr(filePath.rfind('/') + 1);

	if (fileExists(filePath))
	{
		res.status(STATUS_409).text("filename: " + filename + " allready exists").send();
		return;
	};
	if (contentType.empty())
	{
		res.status(STATUS_451).text("content-type must be image/png or image/jpg").send();
		return;
	}
	if (extension == ".png" && contentType != "image/png")
	{
		res.status(STATUS_451).text("content-type must be image/png").send();
		return;
	}
	if (extension == ".jpg" && contentType != "image/jpg")
	{
		res.status(STATUS_451).text("content-type must be image/jpg").send();
		return;
	}
	if (req.getBody().empty())
	{
		res.status(STATUS_204).text("body is empty").send();
		return;
	}
	std::ofstream file(filePath, std::ofstream::binary | std::ofstream::out);
	file.write(req.getBody().c_str(), req.getBody().size());
	file.close();
	res.status(STATUS_201).text("filename: " + filename + " uploaded").send();
};

void delet(Request &req, Response &res, const string filePath)
{
	(void) req;
	string extension = filePath.substr(filePath.rfind('.'));
	string filename = filePath.substr(filePath.rfind('/') + 1);
	cout << "filePath:"<< filePath << "\n";
	if (!fileExists(filePath))
	{
		res.status(STATUS_409).text("filename: " + filename + " does not exist").send();
		return;
	};
	std::remove(filePath.c_str());
	res.status(STATUS_200).text("filename: " + filename + " deleted").send();
};
