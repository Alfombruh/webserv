#include "servconf.hpp"

ServConf::ServConf() : autoindex(false), client_MBS(0), credent("off"), cgi_bin("cgi-bin")
{
	modifier = NONE;
	initMap();//init directive map
}

std::map<std::string, ServConf::type> ServConf::dir;

void	ServConf::initMap()
{
	ServConf::dir["cgi"] = &ServConf::f_cgi;
	ServConf::dir["auth"] = &ServConf::auth;
	ServConf::dir["root"] = &ServConf::f_root;
	ServConf::dir["index"] = &ServConf::f_index;
	ServConf::dir["listen"] = &ServConf::listen;
	ServConf::dir["upload"] = &ServConf::upload;
	ServConf::dir["cgi_bin"] = &ServConf::f_cgi_bin;
	ServConf::dir["location"] = &ServConf::location;
	ServConf::dir["autoindex"] = &ServConf::f_autoindex;
	ServConf::dir["error_page"] = &ServConf::error_pages;
	ServConf::dir["server_name"] = &ServConf::f_server_name;
	ServConf::dir["limit_except"] = &ServConf::limit_except;
	ServConf::dir["client_max_body_size"] = &ServConf::f_client_MBS;
}

ServConf	&ServConf::operator=(const ServConf &rhs)
{
	client_MBS = rhs.client_MBS;
	root = rhs.root;
	error_codes = rhs.error_codes;
	index = rhs.index;
	cgi = rhs.cgi;
	cgi_bin = rhs.cgi_bin;
	autoindex = rhs.autoindex;
	credent = rhs.credent;
	return *this;
}

void	ServConf::f_server_name(std::vector<std::string>::iterator &it)
{
	while (*it != ";")
		server_name.push_back(*it++);
}

void	ServConf::Server(std::vector<std::string>::iterator &it)
{
	if (*it != "{")
		throw (Server::serverException("server block opening bracket is missing"));
	while (*(++it) != "}")
	{
		if (ServConf::dir[*it])
			(this->*(ServConf::dir[*it]))(++it);
		else
			throw (Server::serverException("invalid directive"));
	}
}

void	ServConf::listen(std::vector<std::string>::iterator &it)
{
	std::string	str = *it;
	std::string	ip = "0.0.0.0";
	uint32_t	port = 8080;

	if (str.find(':') != std::string::npos)
	{
		ip = str.substr(0, str.find(':'));
		std::string port_str = str.substr(str.find(':') + 1);
		if (port_str.find_first_not_of("0123456789") != std::string::npos)
			throw (Server::serverException(("invalid port : " + port_str).c_str()));
		port = std::stoi(port_str);
		if (port > 65535)
			throw (Server::serverException("duplicate value in 'listen'"));
	}
	else if (str.find_first_not_of("0123456789") != std::string::npos)
		ip = str;
	else
		port = std::stoi(str);
	Listen list(ip, port);
	if (std::find(listens.begin(), listens.end(), list) != listens.end())
		throw (Server::serverException("duplicate value in 'listen'"));
	listens.push_back(Listen(ip, port));
	if (*++it != ";")
		throw (Server::serverException("double value in 'listen'"));
}

std::vector<Listen>	&ServConf::getListens()
{
	return listens;
}

std::vector<std::string>	&ServConf::getServer_Name()
{
	return server_name;
}

std::vector<ServConf>	&ServConf::getLocations()
{
	return locations;
}

std::string	&ServConf::getUri()
{
	return uri;
}

void	ServConf::f_client_MBS(std::vector<std::string>::iterator &it)
{
	if (it->find_first_not_of("0123456789") != std::string::npos)
		throw (Server::serverException("invalid simbols in client_max_body_size"));
	client_MBS = std::stoi(*it);
	if (*(++it) != ";")
		throw (Server::serverException("double value in 'client_max_body_size'"));
}

void	ServConf::error_pages(std::vector<std::string>::iterator &it)
{
	std::vector<int> codes;

	while (it->find_first_not_of("0123456789") == std::string::npos)
		codes.push_back(std::stoi(*it++));
	for (std::vector<int>::iterator it2 = codes.begin(); it2 != codes.end(); it2++)
		error_codes[*it2] = *it;
	if (*(++it) != ";")
		throw (Server::serverException("double value in 'listen'"));
}

void	ServConf::auth(std::vector<std::string>::iterator &it)
{
	credent = *it;
	if (*(++it) != ";")
		throw (Server::serverException("double value in 'auth'"));
}

void	ServConf::f_root(std::vector<std::string>::iterator &it)
{
	root = *it;
	if (*(++it) != ";")
		throw (Server::serverException("double value in 'root'"));
}

void	ServConf::f_cgi(std::vector<std::string>::iterator &it)
{
	std::string &a = *it++;
	std::string &b = *it++;

	cgi[a] = b;
	if (*(++it) != ";")
		throw (Server::serverException("triple value in 'cgi'"));
}

void	ServConf::f_cgi_bin(std::vector<std::string>::iterator &it)
{
	cgi_bin = *it;
	if (*(++it) != ";")
		throw (Server::serverException("double value in 'cgi_bin'"));
}

void	ServConf::limit_except(std::vector<std::string>::iterator &it)
{
	while (*it != ";")
		methods.push_back(*it++);
}

void	ServConf::f_index(std::vector<std::string>::iterator &it)
{
	while (*it != ";")
		index.push_back(*it++);
}

void	ServConf::f_autoindex(std::vector<std::string>::iterator &it)
{
	if (*it == "on")
		autoindex = true;
	else if (*it == "off")
		autoindex = false;
	else 
		throw (Server::serverException("double value in 'autoindex'"));
	if (*(++it) != ";")
		throw (Server::serverException("double value in 'autoindex'"));
}

void	ServConf::upload(std::vector<std::string>::iterator &it)
{
	uploads = *it;
	if (*(++it) != ";")
		throw (Server::serverException("double value in 'upload'"));
}

void	ServConf::location(std::vector<std::string>::iterator &it)
{
	ServConf location2;

	location2 = *this;
	location2.location_loop(it, locations);
}

void	ServConf::location_loop(std::vector<std::string>::iterator &it, std::vector<ServConf> &locations)
{
	if (*it == "=" || *it == "~" || *it == "*~" || *it == "^~")
	{
		if (*it == "=")
			modifier = EXACT;
		else if(*it == "~")
			modifier = SENSITIVE_REG;
		else if (*it == "~*")
			modifier = INSENSITIVE_REG;
		else if (*it == "^~")
			modifier = LONGEST;
		else
			throw (Server::serverException("double value in 'upload'"));
		it++;
	}
	else
		modifier = NONE;
	uri = *it++;
	if (*it != "{")
		throw (Server::serverException("server block opening bracket is missing"));
	while (*(++it) != "}")
	{
		if (ServConf::dir[*it])
			(this->*(ServConf::dir[*it]))(++it);
		else
			throw (Server::serverException(("invalid directive " + *it + " in 'location'").c_str()));
	}
	locations.push_back(*this);
}
