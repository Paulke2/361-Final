#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int bankers(int numResources, int numProcesses, int *available, int *max, int *allocated, int *need){
    int f[numProcesses], safeSequence[numProcesses], work[numResources], count = 0;
    for(int i = 0; i < numResources; i++){
        work[i] = available[i];
    }
    for(int i = 0; i < numProcesses; i++){
        f[i] = 0;
    }
    int index = 0;
    while(count < numProcesses){
        int found = 0;
        for(int i = 0; i < numProcesses; i++){
            if(f[i] == 0){
                int j;
                for(j = 0; j < numResources; j++){
                    if(need[i * numResources + j] > work[j]){
                        break;
                    }
                }
                if(j == numResources){
                    for(int k = 0; k < numResources; k++){
                        work[k] += allocated[i * numResources + k];
                    }
                    safeSequence[index++] = i;
                    f[i] = 1;
                    found = 1;
                    count++;
                }
            }
        }
        if(found == 0){
            printf("System is not in safe state\n");
            return 0;
        }
    }

    return 0;
}