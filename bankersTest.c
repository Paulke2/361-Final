#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "bankers.h"
//below is the structure for a job/process.

int main(){
    struct process p1 = {1, 1, 10, 5, 3, 0, NULL};
    struct process p2 = {2, 2, 10, 5, 3, 2, NULL};
    struct process p3 = {3, 3, 10, 5, 3, 3, NULL};
    struct process p4 = {4, 4, 10, 5, 3, 1, NULL};
    p1.next = &p2;
    p2.next = &p3;
    p3.next = &p4;
    struct process *processes = &p1;
    bankers(processes, 4, 2, 10);
    return 0;
}
