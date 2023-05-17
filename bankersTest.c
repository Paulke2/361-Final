#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "queue_functions.h"
#include "bankers.c"
//below is the structure for a job/process.

int main(){
    struct process p1 = {1, 1, 10, 5, 3, 0,0,0,0, NULL};
    struct process p2 = {2, 2, 10, 5, 3, 2,0,0,0, NULL};
    struct process p3 = {3, 3, 10, 5, 7, 2,0,0,0, NULL};
    struct process p4 = {4, 4, 10, 5, 3, 1,0,0,0, NULL};
    p1.next = &p2;
    p2.next = &p3;
    p3.next = &p4;
    struct process *processes = &p1;
    struct process *testProcess;
    testProcess = processes;
    printf("%d",testProcess->processID);
    bankers(processes, 2, 1, 2);
    return 0;
}
