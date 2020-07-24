//procMonitor.cpp - process monitor child
//
// 05-Feb-20  M. Watler         Created.
// 19-July-20 Bruno Alexander   Lab completed
#include <fcntl.h>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;

static void handler(int signalId);
bool isRunning=true;
bool isPaused=false;

int main(int argc, char *argv[])
{
    //Declare a variable of type struct sigaction
    struct sigaction sig;
    sig.sa_handler = handler;
    sigemptyset(&sig.sa_mask);
    sig.sa_flags = 0;

    //Register signal handlers for SIGUSR1, SIGUSR2, ctrl-C and ctrl-Z
    int action1 = sigaction(SIGUSR1, &sig, NULL);
    int action2 = sigaction(SIGUSR2, &sig, NULL);
    int action3 = sigaction(SIGINT, &sig, NULL);
    int action4 = sigaction(SIGTSTP, &sig, NULL);

    int totalCount = 1;

    while(isRunning) {
        if(isPaused){
            cout << "Child [" << getpid() << "]: paused..." << endl;
        }else{
            cout << "Child ["<< getpid() << "]: count:" << totalCount << endl;
            totalCount++;
        }
	    sleep(1);
    }
    cout << "Child [" << getpid() << "]: Terminating..." << endl;
    return totalCount;
}

static void handler(int signalId)
{
    switch(signalId) {
        case SIGUSR1:
            isPaused = !isPaused;
	        break;
        case SIGUSR2:
            isRunning = !isRunning;
	        break;
        case SIGINT:
            cout << "Child: " << getpid() << " Ctrl-C Ignored" << endl;
            break;
        case SIGTSTP:
            cout << "Child: " << getpid() << " Ctrl-Z Ignored" << endl;
            break;
	default:
	    cout << "Undefined signal, what are you doing?" << endl;
    }
}