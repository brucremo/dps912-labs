// DPS912 - server.cpp
// Created: Bruno Alexander Cremonese de Morais   Aug 03, 2020

#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <mutex>
#include <thread>

using namespace std;

// Global variables
const int MAX_BUFFER_SIZE = 256;
struct sockaddr_in server_addr, client_addr; 
bool keep_running = true;
int sock_fd; 
char buffer[MAX_BUFFER_SIZE]; 
socklen_t socket_len;

int main (int argc, char *argv[]) {

    string port = argv[1];
    sock_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_TCP);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(stoi(port));
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket with the server address 
    if (bind(sock_fd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0 ) 
    { 
        perror("ERROR: Cannot bind socket"); 
        exit(EXIT_FAILURE); 
    } 

    listen(sock_fd,1);
    cout << "Listening..." << endl;

    socklen_t client_len = sizeof(client_addr);

    int client_fd = accept(sock_fd, (struct sockaddr *) &client_addr, &client_len);

}