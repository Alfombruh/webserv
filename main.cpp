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
	std::signal(SIGINT, signalHandler);
	try
	{
		ConfigParser parser(argv[1]);
		for (size_t i = 0; i < parser.getServerAmmount(); i++)
		{
			Server server(parser.getConfigAt(i));
			server.setup();
			server.run();
		}
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
	}
	return (0);
}