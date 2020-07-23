#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netdb.h> 
#include <iostream>

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    char * path = "/tmp/lab5";
    int sockfd, n;
    struct sockaddr_un serv_addr;

    char buffer[256];
    char doneStr[5] = "Done";
    char pidStr[4] = "PID";
    char sleepStr[6] = "Sleep";
    char quitStr[5] = "Quit";

    bool loop = true;

    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path, path);

    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);

    while(loop){

        if (connect(sockfd,(struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
            error("ERROR: Unable to connect");

        bzero(buffer,256);
        n = read(sockfd,buffer,255);

        if (n < 0) {
            error("ERROR: Reading failed");
            return -1;
        }else{
            std::cout << "Received: " << buffer << std::endl;
            if(strcmp(buffer,pidStr) == 0){
                int pid = getpid();
                char * strPid;
                sprintf(strPid, "%d", pid);
                n = write(sockfd, strPid, sizeof(pid));
                std::cout << "Sent: " << *strPid << std::endl;
            }else{
                error("ERROR: Invalid command");
            }
            if (n < 0) error("ERROR: Writing failed");
        }

        bzero(buffer,256);
        n = read(sockfd,buffer,255);

        if (n < 0) {
            error("ERROR: Reading failed");
            return -1;
        }else{
            std::cout << "Received: " << buffer << std::endl;
            if(strcmp(buffer, sleepStr) == 0){
                std::cout << "Sleeping..." << std::endl;
                sleep(5);
                std::cout << "Woke up!" << std::endl;
                n = write(sockfd, "Done", 5);
            }else{
                error("ERROR: Invalid command");
            }
            if (n < 0) error("ERROR: Writing failed");
        }

        bzero(buffer,256);
        n = read(sockfd,buffer,255);
        
        if (n < 0) {
            error("ERROR: Reading failed");
            return -1;
        }else{
            std::cout << "Received: " << buffer << std::endl;
            if(strcmp(buffer, quitStr) == 0){
                loop = false;
            }else{
                error("ERROR: Invalid command");
            }
            if (n < 0) error("ERROR: Writing failed");
        }
    }


    close(sockfd);
    return 0;
}