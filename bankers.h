int bankers(struct process *processes, int requestingID, int requestedNum, int sys_memory){
    int numProcesses = 0;
    struct process *temp = processes;
    while(temp != NULL){
        numProcesses++;
        temp = temp->next;
    }
    int allocated[numProcesses], max[numProcesses], need[numProcesses];
    int available = sys_memory;
    temp = processes;
    for(int i = 0; i < numProcesses; i++){
        allocated[i] = temp->memoryRequested;
        max[i] = temp->maxDevices;
        need[i] = max[i] - allocated[i];
        available -= allocated[i];
        temp = temp->next;
    }
    int requesting = 0, requestedNum = 0;
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
    return 0;
}