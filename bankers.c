#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int bankers(int requesting, int requestedNum, int numProcesses, int available, int *max, int *allocated, int *need){
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
    return 0;
}
int main(){
    int max[4] = {4, 3, 2, 1};
    int allocated[4] = {1, 0, 1, 0};
    int need[4] = {3, 3, 1, 1};
    bankers(0, 1, 4, 5, max, allocated, need);
    return 0;
}