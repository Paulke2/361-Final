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
    temp = processes;
    for(int i = 0; i < numProcesses; i++){
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
    for(int i = 0; i < numProcesses; i++){
        if(need[i] <= available){
            safeSequence[count++] = i;
            available += allocated[i];
        }
    }
    if(count != numProcesses){
        printf("Error: System is in an unsafe state.\n");
        return -1;
    }
    printf("Safe sequence: ");
    for(int i = 0; i < numProcesses; i++){
        printf("%d ", safeSequence[i]);
    }
    printf("\n");
    free(allocated);free(max);free(need);
    return 0;
}