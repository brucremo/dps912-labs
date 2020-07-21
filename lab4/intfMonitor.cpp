// ntfMonitor_solution.cpp - An interface monitor
//
// 13-Jul-20  M. Watler         Created.
// 21-Jul-20  Bruno Alexander   Lab implemented.

#include <fcntl.h>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;

const int MAXBUF=128;
bool isRunning=false;
bool isWaiting = true;

//Declare your signal handler function prototype
static void handler(int signalId);

int main(int argc, char *argv[])
{
    //Declare a variable of type struct sigaction
    struct sigaction sig;
    sig.sa_handler = handler;
    sigemptyset(&sig.sa_mask);
    sig.sa_flags = 0;

    char interface[MAXBUF];
    char statPath[MAXBUF*2];
    const char logfile[]="Network.log";//store network data in Network.log
    int retVal=0;

    //Register signal handlers for SIGUSR1, SIGUSR2, ctrl-C and ctrl-Z
    int action1 = sigaction(SIGUSR1, &sig, NULL);
    int action2 = sigaction(SIGUSR2, &sig, NULL);
    int action3 = sigaction(SIGINT, &sig, NULL);
    int action4 = sigaction(SIGTSTP, &sig, NULL);

    //Ensure there are no errors in registering the handlers
    if (action1 < 0 || action2 < 0 || action3 < 0 || action4 < 0 ) {
       cout << "Error registering handlers, exiting..." << endl;
       return -1;
    }

    strncpy(interface, argv[1], MAXBUF);//The interface has been passed as an argument to intfMonitor
    int fd=open(logfile, O_RDWR | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
    cout<<"intfMonitor:main: interface:"<<interface<<":  pid:"<<getpid()<<endl;

    //Wait for SIGUSR1 - the start signal from the parent
    pause();

    while(isRunning) {
        //gather some stats
        int tx_bytes=0;
        int rx_bytes=0;
        int tx_packets=0;
        int rx_packets=0;
	    ifstream infile;
        sprintf(statPath, "/sys/class/net/%s/statistics/tx_bytes", interface);
	    infile.open(statPath);
        if(infile.is_open()) {
            infile>>tx_bytes;
            infile.close();
        }
        sprintf(statPath, "/sys/class/net/%s/statistics/rx_bytes", interface);
        infile.open(statPath);
        if(infile.is_open()) {
            infile>>rx_bytes;
            infile.close();
        }
        sprintf(statPath, "/sys/class/net/%s/statistics/tx_packets", interface);
        infile.open(statPath);
        if(infile.is_open()) {
            infile>>tx_packets;
            infile.close();
        }
        sprintf(statPath, "/sys/class/net/%s/statistics/rx_packets", interface);
        infile.open(statPath);
        if(infile.is_open()) {
            infile>>rx_packets;
            infile.close();
        }
	    char data[3*MAXBUF];
        //write the stats into Network.log
        int len=sprintf(data, "%s: tx_bytes:%d rx_bytes:%d tx_packets:%d rx_packets: %d\n", interface, tx_bytes, rx_bytes, tx_packets, rx_packets);
        write(fd, data, len);
        sleep(1);
    }
    close(fd);

    return 0;
}

static void handler(int signalId)
{
    switch(signalId) {
        case SIGUSR1:
            cout << "intfMonitor: starting up" <<endl;
            isRunning = true;
	        break;
        case SIGUSR2:
            cout << "intfMonitor: shutting down" << endl;
            isRunning = false;
	        break;
        case SIGINT:
            cout << "intfMonitor: ctrl-C discarded" << endl;
            break;
        case SIGTSTP:
            cout << "intfMonitor: ctrl-Z discarded" << endl;
            break;
	default:
	    cout << "intfMonitor: undefined signal" << endl;
    }
}
