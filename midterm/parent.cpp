//sysmonExec.cpp - A system monitor using fork and exec
//
// 13-Jul-20  M. Watler         Created.
// 21-Jul-20  Bruno Alexander   Lab implemented.

#include <fcntl.h>
#include <fstream>
#include <cstring>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;


int main()
{
    pid_t childPid[2];
    bool isParent = true;

    for(int i=0; i<2 && isParent; ++i) {
        childPid[i] = fork();
        if(childPid[i]==0) {
            execlp("./child", "./child", NULL);
        }
    }

    sleep(5);

    //Send start signals to the children (SIGUSR1)
    for (int i = 0; i < 2; i++) {
        if (!(kill (childPid[i], SIGUSR1)))
            cout << "SIGUSR1 sent to " << childPid[i] << endl;
        else if (errno == EPERM)
            cout << "Error: not allowed" << endl;
        else
            cout << "No Child with PID: " << childPid[i] << endl;
    };

    sleep(5);

    //Send start signals to the children (SIGUSR1)
    for (int i = 0; i < 2; i++) {
        if (!(kill (childPid[i], SIGUSR1)))
            cout << "SIGUSR1 sent to " << childPid[i] << endl;
        else if (errno == EPERM)
            cout << "Error: not allowed" << endl;
        else
            cout << "No Child with PID: " << childPid[i] << endl;
    };

    sleep(5);

    //Send stop signals to the children (SIGUSR2)
    for (int i = 0; i < 2; i++) {
        if (!(kill (childPid[i], SIGUSR2)))
            cout << "SIGUSR2 sent to " << childPid[i] << endl;
        else if (errno == EPERM)
            cout << "Error: not allowed" << endl;
        else
            cout << "No Child with PID: " << childPid[i] << endl;
    };

    for (int i = 0; i < 2; i++) {
        int status;
        pid_t result = waitpid(childPid[i], &status, WNOHANG);
        cout << "Parent: Child [" << childPid[i] << "] killed" << endl;
    }

    cout << "Parent: Terminating..." << endl;

    return 0;
}
