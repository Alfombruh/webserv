#include "routes.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sstream>
#include <cstdlib>

static void getPython(Request &req, Response &res)
{
	std::vector<char*> cstrings;
	cstrings.reserve(req.env.env.size());
	for(size_t i = 0; i < req.env.env.size(); ++i) {
		cstrings.push_back(const_cast<char*>(req.env.env[i].c_str()));
	}
	cstrings.push_back(NULL);
	res.setBody(req.getBody());
	res.status(STATUS_300).textPython(req.env.PATH_INFO, &cstrings[0]).send();
};

void parseEnv(Request &req, Response &res) //https://datatracker.ietf.org/doc/html/rfc3875#section-4.1.5
{
	(void )res;
	req.env.PATH_INFO = req.getAbsoluteRoute();
	req.env.REMOTE_ADDR = inet_ntoa(req.getClientAddr().sin_addr);
	std::stringstream ss;
	ss << htons(req.getClientAddr().sin_port);
	req.env.REMOTE_PORT = ss.str();
	req.env.REQUEST_METHOD =  req.getMethodStr();
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
	for(size_t i = 0; i < req.env.env.size(); ++i) {
		cout << req.env.env[i] << "\n";
	}
	cout << "\n";
	//req.printReqAtributes();
};


bool cgi(Router &router)
{
	if(router.get("/cgi.py", &getPython))
		return true;
	if(router.get("/cgi.php", &getPython))
		return true;
	if(router.get("/cgi.sh", &getPython))
		return true;
	if(router.get("/ubuntu_cgi_tester", &getPython))
		return true;
	if(router.get("/cgi_tester", &getPython))
		return true;
	return router.notFound();
};