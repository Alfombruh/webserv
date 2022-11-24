#include "webserv.h"

int main(int argc, char **argv)
{
	if (argc != 2)
		return (errorMessage("./webserv configFile.conf"));
	try
	{
		Server server(argv[1]);
		server.setup();
		server.run();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
	}

	return (0);
}