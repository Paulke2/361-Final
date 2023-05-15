#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
struct process {
   int processID;
   int priority;
   int burstTime;
   int memoryRequested;
   int maxDevices;
   int allocatedDevices;
   struct process *next;
};
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
    int *work = malloc(numProcesses * sizeof(int));
    for(int i = 0; i < numProcesses; i++){
        finished[i] = 0;
        work[i] = available;
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
    for(int i = 0;i < numProcesses; i++){
        for(int j = 0;j<numProcesses;j++){
            if(finished[j] == 0 && need[j] <= work[j]){
                work[j] += allocated[j];
                need[j] = 0;
                finished[j] = 1;
            }
        }
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
    free(work);
    printf("Safe state\n");
    return 0;
}