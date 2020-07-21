// intfMonitor_solution.cpp - An interface monitor
//
// 13-Jul-20  M. Watler         Created.
// 20-Jul-20  H. Kaba           Completed.

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

static void sigHandler(int sig);

int main(int argc, char *argv[])
{
    struct sigaction sigAct;
    sigAct.sa_handler = sigHandler;
    sigemptyset(&sigAct.sa_mask);
    sigAct.sa_flags = 0;

    char interface[MAXBUF];
    char statPath[MAXBUF*2];
    const char logfile[]="Network.log";//store network data in Network.log
    int retVal=0;

    int err1 = sigaction(SIGUSR1, &sigAct, NULL);
    int err2 = sigaction(SIGUSR2, &sigAct, NULL);
    int err3 = sigaction(SIGINT, &sigAct, NULL);
    int err4 = sigaction(SIGTSTP, &sigAct, NULL);
    if (err1 < 0 || err2 < 0 || err3 < 0 || err4 < 0 ) {
       cout << "Cannot create the signal handler" << endl;
       return -1;
    }

    strncpy(interface, argv[1], MAXBUF);//The interface has been passed as an argument to intfMonitor
    int fd=open(logfile, O_RDWR | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
    cout<<"intfMonitor:main: interface:"<<interface<<":  pid:"<<getpid()<<endl;

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

static void sigHandler(int sig)
{
    switch(sig) {
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
