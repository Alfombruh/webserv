#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main(__unused int argc, char **argv){
    int sock = 0;
    struct sockaddr_in servAddr;
    char msg[] = "POST /galery?filename=test.png HTTP/1.1\nContent-Type: image/jpg\nTransfer-Encoding: gzip\n\n7\r\nMozilla\r\n11\r\nDeveloper Network\r\n\0\r\n\r\n";
    char buffer[BUFFER_SIZE];
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        write(2, "error at creating socket\n", sizeof("error at creating socket\n"));
        return (1);
    }
    memset(&servAddr, '0', sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "10.13.6.6", &servAddr.sin_addr) <= 0){
        write(2, "error with IP conversion\n", sizeof("error with IP conversion\n"));
        return (2);
    }
    if (connect(sock, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0){
        write(2, "error with connecting\n", sizeof("error with connecting\n"));
        return (3);
    }
    printf("Working on socket N==%d, the HTTP request is:\n%s\n", sock, msg);
    send(sock, msg, strlen(msg), 0);
    // send(sock, "\n", sizeof(char), 0);
  //  valread = read(sock, buffer, BUFFER_SIZE);
   // printf("here I am\n");
    // printf("%s\n", buffer);
    return 0;
}
