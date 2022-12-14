#ifndef SERVCONF_HPP
# define SERVCONF_HPP

# include "Response.hpp"
# include "Server.hpp"
# include "Config.hpp"

# include <stdint.h>
# include <sys/socket.h>
# include <map>
# include <vector>

struct Listen
{
	std::string	ip;
	uint32_t	port;

	Listen() : ip(""), port (0) {};
	Listen(std::string ip, uint32_t port) : ip(ip), port(port) {};
};

inline bool operator==(const Listen &src1, const Listen &src2)
{
	return (src1.ip == src2.ip && sr1c.port == src2.port);
}

class ServConf 
{
	private:
		bool						autindex;
		size_t						client_MBS; //client_max_body_size but shorter written eventho im gonna write it like 4 times(hope so)
		std::vector<Listen>			listens;
		std::vector<std::string>	server_name;
		std::vector<std::string>	locations;
		std::vector<std::string>	methods;
		std::vector<std::string>	index;
		std::string					credent;
		std::string					uploads;
		std::string					cgi_bin;
		std::string					root;
		std::string					uri;
		L_FLAGS						modifier;
		std::map<std::string, std::string>	cgi;
		std::map<int, std::string>			error_codes;

	public:
		ServConf();
		~ServConf(){};
		std::vector<Listen>			getListens();
		std::vector<std::string>	getServer_Name();
		std::vector<ServConf>		getLocations();
		std::string					getUri()
		void					server_name(std::vector<std::string>::iterator &it);
		void					listen(std::vector<std::string>::iterator &it);
		void					Server(std::vector<std::string>iterator &it)
		void					client_MBS(std::vector<std::string>::iterator &it);
		void					error_pages(std::vector<std:string>::iterator &it);
		void					auth(std::vector<std:string>::iterator &it);
		void					root(std::vector<std:string>::iterator &it);
		void					cgi(std::vector<std:string>::iterator &it);
		void					cgi_bin(std::vector<std:string>::iterator &it);
		void					limit_except(std::vector<std:string>::iterator &it);
		void					index(std::vector<std:string>::iterator &it);
		void					autoindex(std::vector<std:string>::iterator &it);
		void					upload(std::vector<std:string>::iterator &it);
		void					location(std::vector<std:string>::iterator &it);
		void					location_loop(std::vector<std:string>::iterator &it, std::vector<ServConf> &locations);

		int	id;
		enum L_FLAGS //LOCATION DIRECTIVE FLAGS (their flags cause i say so🤠)
		{
			NONE,
			EXACT,
			SENSITIVE_REG,
			INSENSITIVE_REG,
			LONGEST
		};
};

#endif
