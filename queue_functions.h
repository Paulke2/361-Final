#ifndef QUEUE_FUNCTIONS_H_
#define QUEUE_FUNCTIONS_H_
struct process
{
    int processID;
    int priority;
    int burstTime;
    int memoryRequested;
    int maxDevices;
    struct process *next;
};
struct process *createNewProcess(char *token);
struct process *duplicateProcess(struct process *queue);
struct process *addToQueue(struct process *newJob, struct process *queue);
struct process *addToQueueSJF(struct process *newJob, struct process *queue);
void printQueue(struct process *queue);
#endif /* QUEUE_FUNCTIONS_H_ */