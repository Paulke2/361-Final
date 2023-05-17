#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "queue_functions.h"
int bankers(struct process *processes, int requestingID, int requestedNum, int available){
    int numProcesses = 0;
    struct process *temp = processes;
    while(temp != NULL){
        numProcesses++;
        temp = temp->next;
    }
    int *allocated = malloc(numProcesses * sizeof(int));
    int *max = malloc(numProcesses * sizeof(int));
    int *need = malloc(numProcesses * sizeof(int));
    struct process all_processes[numProcesses];
    int *finished = malloc(numProcesses * sizeof(int));
    int work = available;
    for(int i = 0; i < numProcesses; i++){
        finished[i] = 0;
    }
    temp = processes;
    for(int i = 0; i < numProcesses; i++){
        all_processes[i] = *temp;
        allocated[i] = temp->allocatedDevices;
        max[i] = temp->maxDevices;
        need[i] = max[i] - allocated[i];
        if(temp->processID == requestingID){
            need[i] -= requestedNum;
            allocated[i] += requestedNum;
            if(need[i] < 0){
                printf("Unsafe state1\n");
                return 1;
            }
        }
        temp = temp->next;
    }
    //Here for debugging purposes
    // int *p1need = &need[0];
    // int *p2need = &need[1];
    // int *p3need = &need[2];
    // int *p4need = &need[3];
    int *avail = &work;
    for(int i = 0;i < numProcesses; i++){
        for(int j = 0;j<numProcesses;j++){
            if(finished[j] == 0 && need[j] <= work){
                work += allocated[j];
                need[j] = 0;
                finished[j] = 1;
            }
        }
    }
    for(int i = 0;i < numProcesses; i++){
        printf("need: %d\n", need[i]);
    }
    for(int i = 0;i < numProcesses; i++){
        if(finished[i] == 0){
            printf("Unsafe state2\n");
            return 1;
        }
    }
    free(allocated);
    free(max);
    free(need);
    free(finished);
    printf("Safe state\n");
    return 0;
}