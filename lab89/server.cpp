// DPS912 - server.cpp
//
// Created: Bruno Alexander Cremonese de Morais   Aug 09, 2020

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
#include <queue>

using namespace std;

// Global variables
const int MAX_BUFFER_SIZE = 256;
const int CLIENT_AMOUNT = 3;

struct sockaddr_in server_addr, client_addr; 
bool keep_running = true;
int master_fd, max_fd, client_fd[CLIENT_AMOUNT]; 
char buffer[MAX_BUFFER_SIZE]; 
socklen_t socket_len;
mutex server_mutex;
queue<string> message;
fd_set active_fds, read_fds;
int client_count = 0;

// Function declarations
void * receiver(void *arg);

int main (int argc, char *argv[]) {

    if(argc != 2){
        cout << "Invalid nubmer of arguments, the accepted number is 1" << endl;
        return -1;
    }

    string port = argv[1];

    FD_ZERO(&active_fds);
    FD_ZERO(&read_fds);

    master_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(stoi(port));
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket with the server address 
    if (bind(master_fd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0 ) 
    { 
        perror("ERROR: Cannot bind socket"); 
        exit(EXIT_FAILURE); 
    } 

    // Listen for connections on master_fd
    listen(master_fd,1);
    cout << "Listening..." << endl;

    // Add master fd to the set
    FD_SET(master_fd, &active_fds);
    max_fd = master_fd;

    pthread_t tid;
    int ret = pthread_create(&tid, NULL, receiver, &master_fd);
    if (ret != 0) {
        cout << "Cannot start thread" << endl;
        cout << strerror(errno) << endl;
        close(master_fd);
        return -1;
    }

    while(keep_running){

        if(message.size() == 0){
            sleep(1);
        }

        while (message.size() > 0) {

            server_mutex.lock();
            string recvMsg = message.front();
            message.pop();
            server_mutex.unlock();

            cout << "SERVER: Message received: "<< recvMsg << endl << endl;
        }
    }

    cout << "Quitting..." << endl;

    for (int i = 0; i < client_count; ++i) {
        send(client_fd[i], "Quit", 5, 0);
    }

    pthread_join(tid, NULL);

    close(master_fd);
    return 0;
}

// Thread method implementation
void * receiver(void *arg) {

    char buffer[MAX_BUFFER_SIZE];
    struct timeval timeout;
    timeout.tv_sec  = 5;
	timeout.tv_usec = 0;

    while(keep_running){

        memset(buffer, 0, MAX_BUFFER_SIZE);

        read_fds = active_fds;

        int ret = select(max_fd + 1, &read_fds, NULL, NULL, &timeout);

        if(ret < 0){
            cout << "SERVER: " << strerror(errno) << endl;
        }else{

            if(FD_ISSET(master_fd, &read_fds)){

                client_fd[client_count] = accept(master_fd, NULL, NULL);
 
                if(client_fd[client_count] < 0){
                    cout << "SERVER: " << strerror(errno) << endl;
                }else{

                    FD_SET(client_fd[client_count], &active_fds);

                    if(max_fd < client_fd[client_count]){

                        max_fd = client_fd[client_count];
                    }

                    client_count += 1;
                }
            }else if(ret == 1){

                for (int i = 0; i < client_count; ++i) {

                    if (FD_ISSET (client_fd[i], &read_fds)) {

                        ret = recv(client_fd[i], buffer, MAX_BUFFER_SIZE, 0);

                        if(ret == -1) {

                            cout<< "SERVER: Read Error" <<endl;
                            cout<< strerror(errno) <<endl;
                        }

                        string msg = buffer;

                        server_mutex.lock();
                        message.push(msg);
                        server_mutex.unlock();
                    }
                }
            }else{
                keep_running = false;
            }
        }
    }
}