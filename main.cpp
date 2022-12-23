#include "webserv.h"
#include <csignal>
#include "srcs/config/configParser.hpp"
#include "srcs/classes/config.hpp"

void signalHandler(int signal)
{
    system("leaks webserv");
	exit(signal);
}

Config configuration;

int main(int argc, char **argv)
{
	if (argc != 2)
		return (errorMessage("./webserv configFile.conf"));
    std::signal(SIGINT, signalHandler);
	try
	{
		ConfigParser parser(argv[1]);
		configuration = parser.getConfiguration();
		// configuration.printConfig();

		Server server;
		server.setup();
		server.run();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
	}
	return (0);
}