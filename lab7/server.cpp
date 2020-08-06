// DPS912 - Lab7.cpp
// Created: Bruno Alexander Cremonese de Morais   Aug 05, 2020

#include <errno.h> 
#include <iostream> 
#include <queue> 
#include <signal.h> 
#include <string.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <unistd.h>
#include <mutex>
#include "client.h"

using namespace std;

bool is_running;
key_t key;
int msg_id;
queue<Message> msg_queue;
mutex server_mutex;

// Thread method declaration
static void handler(int sig);
void * receiver(void * arg);

int main(void)
{
    int fd, ret, len;
    char buf[BUF_LEN];
    Message recvMsg, sendMsg;

    
    struct sigaction sig;
    sig.sa_handler = handler;
    sigemptyset(&sig.sa_mask);
    sig.sa_flags = 0;

    int action3 = sigaction(SIGINT, &sig, NULL);
    int action4 = sigaction(SIGTSTP, &sig, NULL);

    if (action3 < 0 || action4 < 0 ) {
       cout << "Error registering handlers, exiting..." << endl;
       return -1;
    }

    key = ftok("serverclient", 65);
    msg_id = msgget(key, 0666 | IPC_CREAT); 
    
    is_running = true;
    pthread_t tid;
    ret = pthread_create(&tid, NULL, receiver, &fd);
    if (ret != 0) {
        cout << "Cannot start thread" << endl;
        cout << strerror(errno) << endl;
        close(fd);
        return -1;
    }

    cout << "Ready for client messaging!" << endl;

    while (is_running) {

        while (msg_queue.size() > 0) {

            server_mutex.lock();
            recvMsg = msg_queue.front();
            msg_queue.pop();
            server_mutex.unlock();

            cout << "SERVER: from client " << recvMsg.msgBuf.source << " " << recvMsg.msgBuf.buf << endl;

            sendMsg.mtype = recvMsg.msgBuf.dest;
            sendMsg.msgBuf.source = recvMsg.msgBuf.source;
            sendMsg.msgBuf.dest = recvMsg.msgBuf.dest;

            sprintf(sendMsg.msgBuf.buf, recvMsg.msgBuf.buf, getpid());
            msgsnd(msg_id, &sendMsg, sizeof(sendMsg), 0);

            sleep(1);
        }
    }

    cout << "Shutting down..." << endl;
    sendMsg.msgBuf.source = 4;

    for (int i = 0; i < 3; i++){

        sendMsg.mtype = i + 1;
        sprintf(sendMsg.msgBuf.buf, "Quit");
        msgsnd(msg_id, &sendMsg, sizeof(sendMsg), 0);
    }
    
    pthread_join(tid, NULL);
    msgctl(msg_id, IPC_RMID, NULL); 

    return 0;
}

// Thread method implementation
void * receiver(void *arg) {

    char buffer[BUF_LEN];

    while (is_running) {

        Message msg;
        msgrcv(msg_id, &msg, sizeof(msg), 4, 0);

        server_mutex.lock();
        msg_queue.push(msg);
        server_mutex.unlock();
    }

    pthread_exit(NULL);
}

static void handler(int sig) {
    switch(sig) {
        case SIGINT:
            cout << "CTRL-C Interruption..." << endl;
            is_running = false;
            break;
        case SIGTSTP:
            cout << "CTRL-Z Interruption..." << endl;
            break;
	default:
	    cout << "Undefined signal..." << endl;
    }
}
