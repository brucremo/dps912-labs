// Lab 4
// Due Date: July 22, 2020
// sysmonExec.cpp - A system monitor using fork and exec
//
// 13-Jul-20  M. Watler         Created.
// 20-Jul-20  H. Kaba           Completed.

#include <fcntl.h>
#include <fstream>
#include <cstring>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;
const int NUM=2;

int systemMonitor();
bool isRunning=true;
bool isParent = true;//Distinguishes between the parent
                     //process and the child process(es)
pid_t childPid[NUM];

char *intf[]={"lo", "ens33"};

int main()
{
    cout << endl << "parent:main: pid:"<<getpid()<<endl;
    for(int i=0; i<NUM & isParent; ++i) {
        childPid[i] = fork();
        if(childPid[i]==0) {//the child
            cout << "child:main: pid:"<<getpid()<<endl;
            isParent=false;
            execlp("./intfMonitor", "./intfMonitor", intf[i], NULL);
            cout << "child:main: pid:"<<getpid()<<" I should not get here!"<<endl;
	        cout<<strerror(errno)<<endl;
        }
    }
    if(isParent) {
        sleep(10);
        systemMonitor();
    }

    cout << "parent:main("<<getpid()<<"): Finished!" << endl;

    return 0;
}

int systemMonitor()
{
    int status=-1;
    pid_t pid=0;

    for (int i = 0; i < NUM; i++) {
        if (!(kill (childPid[i], SIGUSR1)))
            cout << "SIGUSR1 sent to " << childPid[i] << endl;
        else if (errno == EPERM)
            cout << "Operation not permitted." << endl;
        else
            cout << childPid[i] << " doesn't exist." << endl;
    };
    sleep(30);
    for (int i = 0; i < NUM; i++) {
        if (!(kill (childPid[i], SIGUSR2)))
            cout << "SIGUSR2 sent to " << childPid[i] << endl;
        else if (errno == EPERM)
            cout << "Operation not permitted." << endl;
        else
            cout << childPid[i] << " doesn't exist." << endl;
    };

    //Wait for children to terminate
    while(pid>=0) {
        pid=wait(&status);//blocking. waitpid() is non-blocking and
                          //waits for a specific pid to terminate
        cout << "parent:systemMonitor: status:"<<status<<". The child pid:"<<pid<<" has finished"<< endl;
    }

    return 0;
}