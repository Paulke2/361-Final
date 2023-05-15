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
    temp = processes;
    for(int i = 0; i < numProcesses; i++){
        all_processes[i] = *temp;
        allocated[i] = temp->allocatedDevices;
        max[i] = temp->maxDevices;
        need[i] = max[i] - allocated[i];
        available -= allocated[i];
        temp = temp->next;
    }
    int requesting = 0;
    for(int i = 0; i < numProcesses; i++){
        if(processes[i].processID == requestingID){
            requesting = i;
            break;
        }
    }
    int safeSequence[numProcesses], count = 0;
    if(allocated[requesting] + requestedNum > max[requesting]){
        printf("Error: Process %d is requesting more than its max resources.\n", requesting);
        return -1;
    }
    allocated[requesting] += requestedNum;
    need[requesting] -= requestedNum;
    available -= requestedNum;
    for(int j = 0; j < numProcesses; j = next){
        for(int i = 0; i < numProcesses; i++){
            if(need[i] > available){
                printf("Error: Process %d's request would deadlock.\n", requesting);
            }
            else if(need[i] <= available){
                available += allocated[i];
            }
            else{
                printf("Error: System is in an unsafe state.\n");
                return -1;
            }
        }
    }
    if(count != numProcesses){
        printf("Error: System is in an unsafe state.\n");
        return -1;
    }
    printf("\n");
    free(allocated);free(max);free(need);
    return 0;
}