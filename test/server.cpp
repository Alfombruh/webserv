#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define BUFFER_SIZE 1024
#define BACKLOG 10
#define PORT 8080

int main(int argc, char **argv){
	int server_fd, new_socket, reader;
	struct sockaddr_in addr;
	char buffer[BUFFER_SIZE];
	int addr_len = sizeof(addr);
	(void) argc;
	(void) argv;
	(void) reader;
	char msg[] = "Yo, Im the server\n";
	
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
		write(2, "error socket creation\n", sizeof("error socket creation\n"));
		return (1);
	}
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons( PORT );
	memset(addr.sin_zero, 0, sizeof(addr.sin_zero));
	if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0){
		write(2, "error at binding\n", sizeof("error at binding\n"));
		return (2);
	}
	if (listen(server_fd, BACKLOG) < 0){
		write(2, "error at listening\n", sizeof("error at listening\n"));
		return (3);
	}
	while(1){
		if ((new_socket = accept(server_fd, (struct sockaddr *)&addr, (socklen_t *)&addr_len)) < 0){
			write(2, "error at accepting\n", sizeof("error at accepting\n"));
			return (4);
		}
		if ((reader = read(new_socket, buffer, BUFFER_SIZE) < 0)){
			write(2, "error at reading\n", sizeof("error at reading\n"));
			return (5);
		}
		write(new_socket, msg, strlen(msg));
		write(1, buffer, strlen(buffer));
		close(new_socket);
	}
	return (0);
}
