#include "Router.hpp"

Router::Router(Request &req, Response &res)
	: req(req), res(res) { parseEnv(); };

bool Router::methodAllowed(std::vector<METHOD> alowedMethods) const
{
	if (alowedMethods.empty())
		alowedMethods = configuration.getAlowedMethods();
	if (alowedMethods.empty())
		return true;
	for (std::vector<METHOD>::iterator it = alowedMethods.begin(); it != alowedMethods.end(); ++it)
	{
		if (*it == req.getMethod())
			return true;
	}
	return false;
}

string Router::routeFile(const std::string &route) const
{
	string filename = route.substr(route.rfind('/') + 1);
	return filename.rfind('.') != string::npos ? filename : "";
}

bool Router::apiUse(const string route, bool (*callback)(Router &))
{
	if (req.isInRoute(route) == false)
		return false;
	req.updateRoute(route);
	return callback(*this);
};

bool Router::use(const string route, bool (*callback)(Router &))
{
	if (req.isInRoute(route) == false)
		return false;
	if (methodAllowed(configuration.getAlowedMethods()) == false)
	{
		res.errorPage("405", STATUS_405).send();
		return true;
	}
	req.updateRoute(route);
	ssize_t bodySize = req.getHeader("content-length").empty() ? -1 : stringToSize_t(req.getHeader("content-length"));
	if (bodySize == -1)
		bodySize = req.getBody().size();
	if (configuration.getMaxBody() != -1 && bodySize > configuration.getMaxBody())
	{
		res.errorPage("413", STATUS_413).send();
		return true;
	}
	return callback(*this);
};

bool Router::useLocations(std::vector<Location> &locations, bool (*callback)(Router &, Location &))
{
	if (locations.empty())
		return false;
	for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); ++it)
	{
		if (req.isInRoute(it->location) == false)
			continue;
		if (methodAllowed(it->alowedMethods) == false)
		{
			res.errorPage("405", STATUS_405).send();
			return true;
		}
		ssize_t maxBody = it->maxBody != -1 ? it->maxBody : configuration.getMaxBody();
		ssize_t bodySize = req.getHeader("content-length").empty() ? -1 : stringToSize_t(req.getHeader("content-length"));
		if (bodySize == -1)
			bodySize = req.getBody().size();
		if (maxBody != -1 && bodySize > maxBody)
		{
			res.errorPage("413", STATUS_413).send();
			return true;
		}
		it->api.empty() ? req.updateRoute(it->location) : req.setRoute(it->api);
		return callback(*this, *it);
	}
	return false;
};

bool Router::accepExtension(const string extension) const
{
	if (req.getMethod() == GET)
	{
		if (extension == ".css")
			return true;
		if (extension == ".js")
			return true;
	}
	if (extension == ".py")
		return true;
	if (extension == ".sh")
		return true;
	if (extension == ".png")
		return true;
	if (extension == ".jpg")
		return true;
	return false;
}

bool Router::fileExists(const string &filePath) const
{
	std::ifstream found(filePath.c_str());
	return found.good();
}

// GET MIDDLEWARE
bool Router::get(const string route, void (*get)(Request &, Response &)) const
{
	if (req.getMethod() != GET || req.getRoute() != route)
		return false;
	get(req, res);
	return true;
};

bool Router::get(void (*get)(Request &, Response &, string)) const
{
	if (req.getMethod() != GET)
		return false;

	string filename = routeFile(req.getRoute());
	// filepath set for index
	string filePath = configuration.getRoot() + (req.getRoute() == "/" ? "" : req.getRoute()) + configuration.getIndex();

	if (filename.empty() == false &&
		accepExtension(filename.substr(filename.rfind('.'))) == false)
		return false;
	// cout << "filename: " << filename << "$\n";
	if (filename.empty() == false)
		filePath = configuration.getRoot() + (req.getRoute() == "/" ? "" : req.getRoute());
	// cout << "root:" << configuration.getRoot() << " route:" << req.getRoute() << "$\n";
	// cout << "filePath: " << filePath << "$\n";
	if (!fileExists(filePath))
		return false;
	if (configuration.getIndex().empty())
	{
		res.status(STATUS_200).lsDir(filePath.substr(2)).send();
		return true;
	}
	get(req, res, filePath);
	return true;
};

bool Router::get(const Location &location, void (*get)(Request &, Response &, string)) const
{
	if (req.getMethod() != GET)
		return false;

	string root = location.root.empty() ? configuration.getRoot() : location.root;
	string filename = routeFile(req.getRoute());
	// filepath set for index
	string filePath = root + (req.getRoute() == "/" ? "" : req.getRoute()) + location.index;

	if (filename.empty() == false &&
		accepExtension(filename.substr(filename.rfind('.'))) == false)
		return false;
	// cout << "location filename: " << filename << "$\n";
	if (filename.empty() == false)
	{
		filePath = location.destination.empty() ? root + (req.getRoute() == "/" ? "" : req.getRoute())
												: location.destination + (req.getRoute() == "/" ? "" : req.getRoute());
	}
	// cout << "location root:" << root << " route:" << req.getRoute() << "$\n";
	// cout << "location filePath: " << filePath << "$\n";
	if (!fileExists(filePath))
		return false;
	if (location.index.empty())
	{
		res.status(STATUS_200).lsDir(filePath.substr(2)).send();
		return true;
	}
	get(req, res, filePath);
	return true;
};

// POST  MIDDLEWARE
bool Router::post(const string route, void (*post)(Request &, Response &)) const
{
	if (req.getMethod() != POST || req.getRoute() != route)
		return false;
	post(req, res);
	return true;
};

bool Router::post(void (*post)(Request &, Response &, string)) const
{
	if (req.getMethod() != POST)
		return false;

	string filename = routeFile(req.getRoute());

	if (filename.empty() ||
		accepExtension(filename.substr(filename.rfind('.'))) == false)
		return false;
	// cout << "filename: " << filename << "$\n";
	string filePath = configuration.getRoot() + "/" + req.getRoute();

	// cout << "root:" << configuration.getRoot() << " route:" << req.getRoute() << "$\n";
	// cout << "filePath: " << filePath << "$\n";
	post(req, res, filePath);
	return true;
};

bool Router::post(const Location &location, void (*post)(Request &, Response &, string)) const
{
	if (req.getMethod() != POST)
		return false;

	string root = location.root.empty() ? configuration.getRoot() : location.root;
	string filename = routeFile(req.getRoute());

	if (filename.empty() ||
		accepExtension(filename.substr(filename.rfind('.'))) == false)
		return false;
	// cout << "filename: " << filename << "$\n";

	string filePath = location.destination.empty() ? root + req.getRoute()
												   : location.destination + "/" + req.getRoute();

	// cout << "root:" << configuration.getRoot() << " route:" << req.getRoute() << "$\n";
	// cout << "filePath: " << filePath << "$\n";
	post(req, res, filePath);
	return true;
};

// DELETE MIDDLEWARE
bool Router::delet(const string route, void (*delet)(Request &, Response &)) const
{
	if (req.getMethod() != DELETE || req.getRoute() != route)
		return false;
	delet(req, res);
	return true;
};

bool Router::delet(void (*delet)(Request &, Response &, string)) const
{
	if (req.getMethod() != DELETE)
		return false;

	string filename = routeFile(req.getRoute());

	if (filename.empty() ||
		accepExtension(filename.substr(filename.rfind('.'))) == false)
		return false;
	// cout << "filename: " << filename << "$\n";
	string filePath = configuration.getRoot() + req.getRoute();

	// cout << "root:" << configuration.getRoot() << " route:" << req.getRoute() << "$\n";
	// cout << "filePath: " << filePath << "$\n";
	delet(req, res, filePath);
	return true;
};

bool Router::delet(const Location &location, void (*delet)(Request &, Response &, string)) const
{
	if (req.getMethod() != DELETE)
		return false;

	string root = location.root.empty() ? configuration.getRoot() : location.root;
	string filename = routeFile(req.getRoute());

	if (filename.empty() ||
		accepExtension(filename.substr(filename.rfind('.'))) == false)
		return false;
	// cout << "filename: " << filename << "$\n";

	string filePath = location.destination.empty() ? root + req.getRoute()
												   : location.destination + req.getRoute();

	// cout << "root:" << configuration.getRoot() << " route:" << req.getRoute() << "$\n";
	// cout << "filePath: " << filePath << "$\n";
	delet(req, res, filePath);
	return true;
};

bool Router::notFound() const
{
	res.errorPage("404", STATUS_404).send();
	return false;
};

const string Router::getReqRoute() const
{
	return req.getRoute();
}

void Router::parseEnv() // https://datatracker.ietf.org/doc/html/rfc3875#section-4.1.5
{
	req.env.PATH_INFO = req.getAbsoluteRoute();
	req.env.REMOTE_ADDR = inet_ntoa(req.getClientAddr().sin_addr);
	std::stringstream ss;
	ss << htons(req.getClientAddr().sin_port);
	req.env.REMOTE_PORT = ss.str();
	req.env.REQUEST_METHOD = req.getMethodStr();
	std::string s = req.getHeader("host");
	std::string delimiter = ":";
	std::string token;
	size_t pos = 0;
	pos = s.find(delimiter);
	token = s.substr(0, pos);
	s.erase(0, pos + delimiter.length());
	req.env.SERVER_NAME = token;
	pos = s.find(delimiter);
	token = s.substr(0, pos);
	req.env.SERVER_PORT = token;
	req.env.SERVER_PROTOCOL = (string)req.getProtocolVersion();
	if (req.getHeader("referer") == "")
	{
		req.env.PATH_TRANSLATED = "http://" + req.getHeader("host") + req.getAbsoluteRoute();
		req.env.env.push_back("PATH_TRANSLATED=" + req.env.PATH_TRANSLATED);
	}
	req.env.env.push_back("PATH_INFO=" + req.env.PATH_INFO);
	req.env.env.push_back("GATEWAY_INTERFACE=CGI/1.1");
	req.env.env.push_back("REQUEST_URI=" + req.env.PATH_INFO);
	req.env.env.push_back("SCRIPT_NAME=" + req.env.PATH_INFO);
	req.env.env.push_back("REMOTE_ADDR=" + req.env.REMOTE_ADDR);
	req.env.env.push_back("REMOTE_PORT=" + req.env.REMOTE_PORT);
	req.env.env.push_back("REQUEST_METHOD=" + req.env.REQUEST_METHOD);
	req.env.env.push_back("SERVER_NAME=" + req.env.SERVER_NAME);
	req.env.env.push_back("SERVER_PORT=" + req.env.SERVER_PORT);
	req.env.env.push_back("SERVER_PROTOCOL=" + req.env.SERVER_PROTOCOL);
	// for(size_t i = 0; i < req.env.env.size(); ++i) {
	// 	cout << req.env.env[i] << "\n";
	// }
	// cout << "\n";
	// req.printReqAtributes();
};
