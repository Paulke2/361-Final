#ifndef QUEUE_FUNCTIONS_H_
#define QUEUE_FUNCTIONS_H_
struct process
{
    int processID;
    int priority;
    int burstTime;
    int memoryRequested;
    int maxDevices;
    int arrival;
    int finish;
    int accrued;
    int allocatedDevices;
    struct process *next;
};
struct process *createNewProcess(char *token,int next_instruction_time);
struct process *duplicateProcess(struct process *queue);
struct process *addToQueue(struct process *newJob, struct process *queue);
struct process *addToQueueSJF(struct process *newJob, struct process *queue);
float avgTurnaroundTime(struct process *finished_queue);
int printAtTime(int used_memory,int time,int time_passed, int memory, int devices, struct process *hold_queue1, struct process *hold_queue2, struct process *ready_queue, struct process *wait_queue, struct process *finished_queue, struct process *onCPU);
void printQueue(struct process *queue);
#endif /* QUEUE_FUNCTIONS_H_ */