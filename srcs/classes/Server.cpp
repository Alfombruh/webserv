#include "webserv.h"

class Server{
    private:
         int sd;
         int new_socket;
         int addr_len;
         struct sockaddr_in addr;
    public:
        int launch(void);
        int wait(void);
};

int Server::launch(void){
    addr_len = sizeof(addr);
    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
        return (error_message("Error: couldn't create socket"));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);
    if (bind(sd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
        return (error_message("Error: couldn't bind"));
    if (listen(sd, BACKLOG) < 0)
        return (error_message("Error: not listening"));
    return (0);
}

int Server::wait(){
    int new_socket;

    while (1)
    {
        if ((new_socket = accept(sd, (struct sockaddr *)&addr, (socklen_t *)&addr_len)) < 0)
            return (error_message("Error: couldn't accept package"));
        if ((reader = read(new_socket, buffer, BUFFER_SIZE) < 0))
            return (error_message("Error: cannot read"));
        write(new_socket, msg, strlen(msg));
        write(1, buffer, strlen(buffer));
        close(new_socket);
    }
}