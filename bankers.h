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