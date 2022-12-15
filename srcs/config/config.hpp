#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <iostream>

using std::string;
using std::cout;

class Config
{
private:

public:
	Config(const string configPath);
	~Config();
};

#endif