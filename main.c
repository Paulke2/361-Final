#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "queue_functions.c"

int getInternalEventTime(struct process *ready_queue,int quantum,int time_passed)
{
    if(ready_queue==NULL){
        //if ready_queue mt, we need to read a new instruction so internal_event_time needs to be > next_instruction_time so we set internal_event_time to be arbitrary large number
        return 9999999;
    }
    else{
        if(ready_queue->burstTime>=quantum){
            return time_passed+quantum;
        }else{
            //process will not finish quantum and we need to return curr time +remaining burstTime
            return time_passed+ready_queue->burstTime;
        }
    }
}

void schedule()
{
    struct process *hold_queue1 = NULL;
    struct process *hold_queue2 = NULL;
    struct process *ready_queue = NULL;
    struct process *wait_queue = NULL;
    struct process *finished_queue = NULL;
    // if there are no processes on ready or wait, we will increment time_passed by 1
    int time_passed = 0;
    // system config. options
    int sys_memory = 0;
    int used_memory = 0;
    int sys_serial_devices = 0;
    int quantum = 0;
    FILE *file = fopen("input.txt", "r");
    char buffer[100];
    const char *empty = "empty";
    strcpy(buffer, empty);
    int next_instruction_time = 0;
    int internal_event_time = 99;
    while (true)
    {
        if ((strcmp(buffer, "empty") == 0))
        {
            // if the instruction read time has passed, and there is no outstanding instruction
            fgets(buffer, 100, file);
        }
        if (buffer == NULL)
        {
            next_instruction_time = 99999999;
            printf("%s", "No more instructions to read");
        }
        char temp[100];
        strcpy(temp, buffer);
        char *token = strtok(temp, " ");
        char instruction_type[100];
        strcpy(instruction_type, token);
        token = strtok(NULL, " ");
        next_instruction_time = atoi(token);

        internal_event_time=getInternalEventTime(ready_queue,quantum,time_passed);
        if (next_instruction_time < internal_event_time)
        {
            // read new instruction
            if (strcmp(instruction_type, "C") == 0)
            {
                printf("-----Configuring----\n");
                token = strtok(NULL, " ");
                sys_memory = getNumber(token);
                printf("this is mem %d\n", sys_memory);
                token = strtok(NULL, " ");
                sys_serial_devices = getNumber(token);
                printf("serial devices: %d\n", sys_serial_devices);
                token = strtok(NULL, " ");
                quantum = getNumber(token);
                printf("quantum: %d\n", sys_serial_devices);
                printf("------------------\n");
                strcpy(buffer, empty);
            }
            else if (strcmp(instruction_type, "A") == 0)
            {
                struct process *newJob = createNewProcess(token);
                strcpy(buffer, empty);
                // determine if enough total mem.
                if (newJob->memoryRequested > sys_memory)
                {
                    // discard job
                }
                else if (newJob->memoryRequested <= sys_memory - used_memory)
                {
                    // if their is enough mem. add job to ready_queue
                    ready_queue=addToQueue(newJob, ready_queue);
                    used_memory = used_memory + newJob->memoryRequested;
                }
                else
                {
                    // if not enough mem. add to hold queue
                    if(newJob->priority==2){
                        hold_queue2=addToQueue(newJob, hold_queue2);
                    }
                    else{
                        //instead of adding to queue1 with addToQueue, create a method that accepts the same parameters as addToQueue, loop through hold_queue1, 
                        //once curr->next->bursttime > newJob->burst_time, do something like newJob->next = curr->next. then curr->next=newJob. or some variation of inserting in the
                        //middle of a linked list
                        hold_queue1=addToQueue(newJob, hold_queue1);
                    }
                }
            }
            else if (strcmp(instruction_type, "Q") == 0)
            {
                //device request
                strcpy(buffer, empty);
            }
            else if (strcmp(instruction_type, "L") == 0)
            {
                //device release request
                strcpy(buffer, empty);
            }
            else
            {
                strcpy(buffer, empty);
            }
        }
        else
        {
            // else process internal event. there are no more instructions that need to be read.
            if (ready_queue != NULL)
            {
                if (ready_queue->burstTime - quantum > 0)
                {
                    time_passed = time_passed + quantum;
                    ready_queue->burstTime = ready_queue->burstTime - quantum;
                    // now we need to bring head to tail
                    struct process *temp = duplicateProcess(ready_queue);
                    //printf("jobNumber %d has %d run time left\n",ready_queue->processID,ready_queue->burstTime);
                    ready_queue=addToQueue(temp, ready_queue);
                    ready_queue = ready_queue->next;
                    printQueue(ready_queue);
                }
                else
                {
                    // job complete. job scheduler should check if holdqueue 1 process has enough mem. then check hold queue 2 if enough resources
                    time_passed = time_passed + ready_queue->burstTime;
                    ready_queue->burstTime = 0;
                    struct process *completed_job = duplicateProcess(ready_queue);
                    // add ready_queue head to finished queue and remove process from ready_queue. release devices/mem. and check waitqueue for device requests
                    used_memory = used_memory - ready_queue->memoryRequested;
                    printf("finished Job number: %d\n",ready_queue->processID);
                    finished_queue=addToQueue(completed_job, finished_queue);
                    ready_queue = ready_queue->next;
                }
            }
        }

        if (time_passed >=30&&ready_queue==NULL)
        {
            printf("%s","ready Queue should be mt");
            // this is temporary. we need to figure out what stops the program. cannot stop when no more instructions b/c there will still be internal events to handle
            break;
        }
    }
}
int main()
{

    schedule();

    return 0;
}