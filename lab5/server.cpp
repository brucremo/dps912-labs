// server.cpp - A simple socket server
//
// 22-Jul-20  Bruno Alexander   Lab implemented.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <sys/un.h>
#include <iostream>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    char * path = "/tmp/lab5";
    int sockfd, newsockfd;
    socklen_t clilen;
    char buffer[256];
    char doneStr[5] = "Done";
    struct sockaddr_un serv_addr, cli_addr;
    int n;

    serv_addr.sun_family = AF_UNIX;
    strncpy(serv_addr.sun_path, (char*)path, sizeof(serv_addr.sun_path));

    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd < 0) 
    error("ERROR: Opening socket failed");
    
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
            sizeof(serv_addr)) < 0) 
            error("ERROR: Binding failed");
    listen(sockfd,1);
    std::cout << "Listening..." << std::endl;
    clilen = sizeof(cli_addr);

    newsockfd = accept(sockfd, 
                (struct sockaddr *) &cli_addr, 
                &clilen);

    if (newsockfd < 0) 
        error("ERROR: Accept failed");

    n = write(newsockfd,"PID",4);
    if (n < 0) error("ERROR: PID Request failed");

    bzero(buffer,256);
    n = read(newsockfd,buffer,255);

    if (n < 0) {
        error("ERROR: Reading failed");
        return -1;
    }else{
        std::cout << "Received: " << buffer << std::endl;
        std::cout << "This client has pid: " << buffer << std::endl;
    }
    
    n = write(newsockfd,"Sleep",6);

    bzero(buffer,256);
    n = read(newsockfd,buffer,255);

    if (n < 0) {
        error("ERROR: Reading failed");
        return -1;
    }else{
        std::cout << "Received: " << buffer << std::endl;
        if(strcmp(buffer, doneStr) == 0){
            n = write(newsockfd,"Quit",6);
        }else{
            error("ERROR: Invalid command");
        }
        if (n < 0) error("ERROR: Writing failed");
    }
    close(newsockfd);
    close(sockfd);
    return 0; 
}