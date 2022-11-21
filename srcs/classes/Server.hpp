#ifndef SERVER_HPP
#define SERVER_HPP

#include <fstream>
#include <exception>
#include <netinet/in.h>

class Server
{
private:
    int sd;
    int new_socket;
    int addr_len;
    int reader;
    struct sockaddr_in addr;
    char *buffer;
    // Config config;
public:
    Server(std::string is);
    int setup(void);
    int run(void);

    class serverException : public std::exception
    {
        char *msg;
    public:
        serverException(const char *msg);
        virtual const char *what() const throw();
    };
};

#endif