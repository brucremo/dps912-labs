// Lab3.cpp - process monitor parent
//
// 05-Feb-20  M. Watler         Created.
// 19-July-20 Bruno Alexander   Lab completed
#include <iostream>
#include <fstream>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <charconv>

using namespace std;

char procDir[] = "/proc";
const int MAXBUF=16;

int main(void)
{
    int numOfChildProcesses = 4;
    int begin, end;
    const int numProcesses=1000;
    int childPidPool[numOfChildProcesses];
    bool isParent=true;

    cout << endl << "Parent PID: " << getpid() << endl;

    char charBegin[MAXBUF];
    char charEnd[MAXBUF];

    string monitorName = "./procMonitor";
    for(int i=0; i<numOfChildProcesses && isParent; ++i) {
        begin=numProcesses*i/numOfChildProcesses;
        end=numProcesses*(i+1)/numOfChildProcesses-1;

        childPidPool[i] = fork();

        if(childPidPool[i] == 0){

            cout << endl <<"Child PID:" << getpid() << endl;
            isParent = false;

            sprintf(charBegin, "%d", begin);
            sprintf(charEnd, "%d", end);

            execlp(monitorName.c_str(), monitorName.c_str(), charBegin, charEnd, NULL);
            cout << "Error: " << strerror(errno) << endl;
        }
    }

    int forksRunning = numOfChildProcesses;
    if(isParent){
        while(forksRunning == numOfChildProcesses){
            for(int i = 0; i < numOfChildProcesses; i++){
                if(childPidPool[i] != -1){
                    int status;
                    pid_t result = waitpid(childPidPool[i], &status, WNOHANG);

                    if(result != 0){
                        cout << endl <<"Child PID EXITED:" << childPidPool[i] << endl;
                        childPidPool[i] = -1;
                        forksRunning -= 1;
                    }
                }
            }
        }
    }

    return 0;
}
