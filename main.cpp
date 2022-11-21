#include "webserv.h"

 int main(int argc, char **argv){
	if (argc != 2)
		return (errorMessage("./webserv configFile.conf"));
	Server server(argv[1]);
	server.setup();
	server.run();
	return (0);
 }