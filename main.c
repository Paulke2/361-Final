#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "bankers.h"
#include "queue_functions.c"

int getInternalEventTime(struct process *ready_queue, int quantum, int time_passed)
{
    // this function gets the time of the next internal event(i.e. running a job on the CPU). this time
    // is then compared to the next instruction read time. internal events are prioritized.
    if (ready_queue == NULL)
    {
        // if ready_queue mt, we need to read a new instruction so internal_event_time needs to be > next_instruction_time so we set internal_event_time to be arbitrary large number
        return 9999999;
    }
    else
    {
        if (ready_queue->burstTime >= quantum)
        {
            return time_passed + quantum;
        }
        else
        {
            // process will not finish quantum and we need to return curr time +remaining burstTime
            return time_passed + ready_queue->burstTime;
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
            // if the buffer is 'empty', indicating that an instruction has been read and a new one is
            // abel to be read, buffer gets the next line from the file.
            fgets(buffer, 100, file);
            printf(buffer);
        }
        char temp[100];
        strcpy(temp, buffer);
        char *token = strtok(temp, " ");
        char instruction_type[100];
        strcpy(instruction_type, token);
        token = strtok(NULL, " ");
        next_instruction_time = atoi(token);

        internal_event_time = getInternalEventTime(ready_queue, quantum, time_passed);
        if (next_instruction_time < internal_event_time)
        {
            // sys. config instruction
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
                struct process *newJob = createNewProcess(token, next_instruction_time);
                strcpy(buffer, empty);
                // determine if enough total mem.
                if (newJob->memoryRequested > sys_memory)
                {
                    // discard job
                }
                else if (newJob->memoryRequested <= sys_memory - used_memory)
                {
                    // if their is enough mem. add job to ready_queue
                    ready_queue = addToQueue(newJob, ready_queue);
                    used_memory = used_memory + newJob->memoryRequested;
                }
                else
                {
                    // if not enough mem. add to hold queue
                    if (newJob->priority == 2)
                    {
                        hold_queue2 = addToQueue(newJob, hold_queue2);
                    }
                    else
                    {
                        // If the job is priority 1, it will go on HQ1 using Shortest Job First;
                        // See documentation in function definition
                        printf("adding  %d to holdque1\n", newJob->processID);
                        printf("needed mem: %d available: %d\n", newJob->memoryRequested, sys_memory - used_memory);
                        hold_queue1 = addToQueueSJF(newJob, hold_queue1);
                    }
                }
            }
            else if (strcmp(instruction_type, "Q") == 0)
            {
                // device request
                strcpy(buffer, empty);
            }
            else if (strcmp(instruction_type, "L") == 0)
            {
                // device release request
                strcpy(buffer, empty);
            }
            else
            {
                if (time_passed >= next_instruction_time)
                {
                    printAtTime(next_instruction_time, sys_memory, sys_serial_devices, hold_queue1, hold_queue2, ready_queue, wait_queue, finished_queue, ready_queue);
                    strcpy(buffer, empty);
                    time_passed++;
                }
                else
                {

                    time_passed++;
                }
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
                    printQueue(ready_queue);
                    printf("curr time: %d\n", time_passed);
                    ready_queue->burstTime = ready_queue->burstTime - quantum;
                    ready_queue->finish = time_passed;
                    // now we need to bring head to tail
                    struct process *temp = duplicateProcess(ready_queue);
                    ready_queue = addToQueue(temp, ready_queue);
                    ready_queue = ready_queue->next;
                }
                else
                {
                    // job complete. job scheduler should check if holdqueue 1 process has enough mem. then check hold queue 2 if enough resources
                    time_passed = time_passed + ready_queue->burstTime;
                    printQueue(ready_queue);
                    printf("curr time: %d\n", time_passed);
                    ready_queue->burstTime = 0;
                    ready_queue->finish = time_passed;
                    struct process *completed_job = duplicateProcess(ready_queue);
                    // add ready_queue head to finished queue and remove process from ready_queue. release devices/mem. and check waitqueue for device requests
                    used_memory = used_memory - ready_queue->memoryRequested;
                    finished_queue = addToQueue(completed_job, finished_queue);
                    struct process *temp = ready_queue;
                    free(temp);
                    ready_queue = ready_queue->next;
                    // checking holdqueues
                    struct process *hold_queue1_temp = hold_queue1;
                    struct process *hold_queue2_temp = hold_queue2;
                    // check all of hold queue 1 if there is now enough memeory to put a process on ready queue
                    while (hold_queue1_temp != NULL)
                    {
                        if (hold_queue1_temp->memoryRequested <= sys_memory - used_memory)
                        {
                            struct process *process_to_add_to_readyQueue = duplicateProcess(hold_queue1_temp);
                            addToQueue(process_to_add_to_readyQueue, ready_queue);
                            used_memory = used_memory + process_to_add_to_readyQueue->memoryRequested;
                            hold_queue1 = removeProcess(hold_queue1, process_to_add_to_readyQueue->processID);
                            printQueue(ready_queue);
                        }
                        hold_queue1_temp = hold_queue1_temp->next;
                    }
                    // check all of hold queue 2 after hold queue 1 if there is now enough memeory to put a process
                    // on ready queue
                    while (hold_queue2_temp != NULL)
                    {
                        if (hold_queue2_temp->memoryRequested <= sys_memory - used_memory)
                        {
                            struct process *process_to_add_to_readyQueue = duplicateProcess(hold_queue2_temp);
                            addToQueue(process_to_add_to_readyQueue, ready_queue);
                            used_memory = used_memory + process_to_add_to_readyQueue->memoryRequested;
                            hold_queue2 = removeProcess(hold_queue2, process_to_add_to_readyQueue->processID);
                            printQueue(ready_queue);
                        }
                        hold_queue2_temp = hold_queue2_temp->next;
                    }
                }
            }
            else
            {
                time_passed = time_passed + 1;
            }
        }

        if (time_passed >= 10000)
        {
            // this is temporary. once ready_queue is NULL, we will let the program read the final d instruction.
            // it is possible to have multiple simulations, so after final D, figure out how to read next lines
            break;
        }
    }
}
int main()
{

    schedule();

    return 0;
}