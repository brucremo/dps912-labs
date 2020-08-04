// DPS912 - Lab6.cpp
// Created: Bruno Alexander Cremonese de Morais   Aug 03, 2020

#include <iostream>
#include <cstring>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <error.h>

using namespace std;

const int ARGUMENT_LENGTH = 64;

int main (int argc, char *argv[]) {

    char argument1[ARGUMENT_LENGTH];
    char argument2[ARGUMENT_LENGTH];
    char arg1[3][ARGUMENT_LENGTH];
    char arg2[3][ARGUMENT_LENGTH];

    int len1 = 0, len2 = 0, status = -1;
    int pfd[2];
    pid_t pid = 0, pid1, pid2;

    strcpy(argument1, argv[1]);
    strcpy(argument2, argv[2]);

    char *token = strtok(argument1, " ");

    while (token != NULL) {

        strcpy(arg1[len1], token);
        token = strtok(NULL, " ");
        ++len1;
    }

    token = strtok(argument2, " ");

    while(token != NULL) {
        
        strcpy(arg2[len2], token);
        token = strtok(NULL, " ");
        ++len2;
    }

    char *args1[len1 + 1];
    char *args2[len2 + 1];

    for (int i = 0; i < len1 + 1; i++) {

        if (i == len1) {
            args1[i] = (char *)NULL;
        } else {
            args1[i] = arg1[i];
        }
    }
    for (int i = 0; i < len2 + 1; i++) {

        if (i == len2) {
            args2[i] = (char *)NULL;
        } else {
            args2[i] = arg2[i];
        }
    }

    if (pipe(pfd) == -1) { 
        perror("pipe"); 
    }

    pid1 = fork();
    if (pid1 == 0) {
        if (pfd[1] != STDOUT_FILENO) {
            if (dup2(pfd[1], STDOUT_FILENO) == -1) { 
                perror ("ERROR: DUP2 failed"); 
            }

            if (close(pfd[0]) == -1) { 
                perror ("ERROR: Closing FD 0 failed"); 
            }

            if (close(pfd[1]) == -1) { 
                perror ("ERROR: Closing FD 1 failed"); 
            }
        }

        if (execvp(args1[0], args1) == -1) { 
            perror ("ERROR:  run execlp ps."); 
        }
    }

    pid2 = fork();
    if (pid2 == 0) {
        if (pfd[0] != STDIN_FILENO) {
            if (dup2(pfd[0], STDIN_FILENO) == -1) { 
                perror ("ERROR: DUP2 failed"); 
            }

            if (close(pfd[0]) == -1) { 
                perror ("ERROR: Closing FD 1 failed"); 
            }

            if (close(pfd[1]) == -1) { 
                perror ("ERROR: Closing FD 0 failed"); 
            }
        }

        if (execvp(args2[0], args2) == -1) { 
            perror ("ERROR: execvp failed"); 
        }
        cout << "ERROR: Second argument not run" << endl;
    }

    if (close(pfd[0]) == -1) { 
        perror("ERROR: Closing FD0"); 
    }

    if (close(pfd[1]) == -1) { 
        perror("ERROR: Closing FD1"); 
    }

    while (pid > 0) {
        pid = wait(&status);
        cout << "PID:" << pid << " finished" << endl;
    }
}