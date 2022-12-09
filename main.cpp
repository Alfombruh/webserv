#include "webserv.h"
#include <csignal>

void signalHandler(int signal)
{
	exit(signal);
}

int main(int argc, char **argv)
{
	if (argc != 2)
		return (errorMessage("./webserv configFile.conf"));

	// start:
	std::signal(SIGINT, signalHandler);
	try
	{
		Server server(argv[1]);
		server.setup();
		server.run();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
		// goto start;
	}
	return (0);
}