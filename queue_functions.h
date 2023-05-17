#ifndef QUEUE_FUNCTIONS_H_
#define QUEUE_FUNCTIONS_H_
struct process
{
    int processID;
    int priority;
    int burstTime;
    int memoryRequested;
    int maxDevices;
    //new instances
    int arrival;
    int finish;
    int accrued;
    struct process *next;
};
struct process *createNewProcess(char *token);
struct process *duplicateProcess(struct process *queue);
struct process *addToQueue(struct process *newJob, struct process *queue);
struct process *addToQueueSJF(struct process *newJob, struct process *queue);
<<<<<<< Updated upstream
float avgTurnaroundTime(struct process *finished_queue);
int printAtTime(int time, int memory, int devices, struct process *hold_queue1, struct process *hold_queue2, struct process *ready_queue, struct process *wait_queue, struct process *finished_queue, struct process *onCPU);
=======
int printAtTime(int used_memory,int time,int time_passed, int memory, int devices, struct process *hold_queue1, struct process *hold_queue2, struct process *ready_queue, struct process *wait_queue, struct process *finished_queue, struct process *onCPU);
>>>>>>> Stashed changes
void printQueue(struct process *queue);
#endif /* QUEUE_FUNCTIONS_H_ */