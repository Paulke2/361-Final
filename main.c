#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "bankers.c"
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
        if (ready_queue->accrued + quantum <= ready_queue->burstTime)
        {
            return time_passed + quantum;
        }
        else
        {
            // process will not finish quantum and we need to return curr time +remaining burstTime
            return time_passed + (ready_queue->burstTime - ready_queue->accrued);
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
    int used_sys_serial_devices = 0;
    int quantum = 0;
    int quantum_temp = 0;
    int requestedNum = 0;
    int processID;
    FILE *file = fopen("input.txt", "r");
    char buffer[100];
    const char *empty = "empty";
    strcpy(buffer, empty);
    int next_instruction_time = 0;
    bool updateFlag = true;
    int internal_event_time = 99;
    while (true)
    {
        internal_event_time = getInternalEventTime(ready_queue, quantum, time_passed);
        quantum = quantum_temp;
        while (next_instruction_time < internal_event_time)

        {
            if ((strcmp(buffer, "empty") == 0))
            {
                // if the buffer is 'empty', indicating that an instruction has been read and a new one is
                // abel to be read, buffer gets the next line from the file.
                fgets(buffer, 100, file);
            }
            char temp[100];
            strcpy(temp, buffer);
            char *token = strtok(temp, " ");
            char instruction_type[100];
            strcpy(instruction_type, token);
            token = strtok(NULL, " ");
            next_instruction_time = atoi(token);
            if (!(next_instruction_time < internal_event_time))
            {
                break;
            }

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
                quantum_temp = quantum;
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
                    if (updateFlag)
                    {
                        internal_event_time = getInternalEventTime(ready_queue, quantum, next_instruction_time);
                        updateFlag = false;
                    }
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
                //if its a request, it sets temp->requestedDevices to the requested amount. then once that process is on the CPU, and the temp->requestedDevices>0, bankers is called
                token = strtok(NULL, " ");
                processID = getNumber(token);
                token = strtok(NULL, " ");
                requestedNum = getNumber(token);
                // find process ID
                struct process *temp = ready_queue;

                if (temp != NULL)
                {
                    if (temp->processID == processID)
                    {
                        quantum = next_instruction_time - time_passed;
                    }
                }
                while (temp != NULL)
                {
                    if (temp->processID == processID)
                    {
                        temp->requestedDevices = requestedNum;
                        break;
                    }
                    temp = temp->next;
                }

                // else not safe to give resources. we need to find it, then add it to wait queue.

                // device request
                strcpy(buffer, empty);
            }
            else if (strcmp(instruction_type, "L") == 0)
            {
                //if its a release, re requested amount is set to a negative number. once its on the CPU it will release those devices from the used amount.
                token = strtok(NULL, " ");
                processID = getNumber(token);
                token = strtok(NULL, " ");
                requestedNum = getNumber(token);
                struct process *temp = ready_queue;
                if (temp != NULL)
                {
                    if (temp->processID == processID)
                    {
                        quantum = next_instruction_time - time_passed;
                        temp->requestedDevices = -requestedNum;
                        printf("\nrequested: %d\n",temp->requestedDevices);
                    }
                    else
                    {
                        while (temp != NULL)
                        {
                            if (temp->processID == processID)
                            {
                                temp->requestedDevices = -requestedNum;
                                printf("\nrequested: %d\n",temp->requestedDevices);

                            }
                            temp = temp->next;
                        }
                    }
                }
                // temp->requestedDevices=-requestedNum;
                //  device release request
                strcpy(buffer, empty);
            }
            else
            {

                if (next_instruction_time < 9999)
                {
                    // print
                    printAtTime(used_memory, next_instruction_time, time_passed, sys_memory, sys_serial_devices, hold_queue1, hold_queue2, ready_queue, wait_queue, finished_queue, ready_queue);
                    strcpy(buffer, empty);
                }
                else
                {
                    if (ready_queue == NULL)
                    {
                        break;
                    }
                }
            }
        }

        // else process internal event. there are no more instructions that need to be read.
        if (ready_queue != NULL)
        {
            bool waited=false;
            if (ready_queue->accrued + quantum < ready_queue->burstTime)
            {
                time_passed = internal_event_time;
                ready_queue->accrued = ready_queue->accrued + quantum;
                if (ready_queue->requestedDevices < 0)
                {
                    //this will run if its a release request.

                    //it is granted and all of waitqueue is checked

                    //ready_queue->requestedDevices should be a negative number
                    used_sys_serial_devices = used_sys_serial_devices + ready_queue->requestedDevices;
                    struct process *wait_temp = wait_queue;
                    //checking wait queues now that more devices freed
                
                    while (wait_temp != NULL)
                    {
                        struct process *temp = duplicateProcess(wait_temp);
                        ready_queue = addToQueue(temp, ready_queue);
                        if (bankers(ready_queue, wait_temp->processID, ready_queue->requestedDevices, sys_serial_devices) == 0)
                        {
                            wait_queue = removeProcess(wait_queue, temp->processID);
                            ready_queue = addToQueue(temp, ready_queue);
                            used_sys_serial_devices = used_sys_serial_devices + temp->requestedDevices;
                            temp->allocatedDevices = temp->allocatedDevices + temp->requestedDevices;
                            temp->requestedDevices = 0;
                        }else{
                            ready_queue=removeProcess(ready_queue, temp->processID);
                        }
                        wait_temp = wait_temp->next;
                    }
                }
                else if (ready_queue->requestedDevices > 0)
                {
                    //if its a device use request, this runs
                    if (bankers(ready_queue, ready_queue->processID, ready_queue->requestedDevices, sys_serial_devices) == 0)
                    {
                        struct process *temp = duplicateProcess(ready_queue);
                        
                        used_sys_serial_devices = used_sys_serial_devices + temp->requestedDevices;
                        temp->allocatedDevices = temp->allocatedDevices + temp->requestedDevices;
                        temp->requestedDevices = 0;
                    }
                    else
                    {
                        //if requesting process does not leave system in safe state, put it on wait_queue
                        struct process *deep_copy = duplicateProcess(ready_queue);
                        wait_queue = addToQueue(deep_copy, wait_queue);
                        ready_queue = removeProcess(ready_queue, deep_copy->processID);
                    }
                }
                // now we need to bring head to tail
                if(!(waited)){
                    //if the head of the ready queue was not put on the wait queue, put it on back of ready
                struct process *temp = duplicateProcess(ready_queue);
                ready_queue = addToQueue(temp, ready_queue);
                ready_queue = ready_queue->next;
                }
            }
            else
            {
                // job complete. job scheduler should check if holdqueue 1 process has enough mem. then check hold queue 2 if enough resources
                time_passed = internal_event_time;
                ready_queue->accrued = ready_queue->accrued + (ready_queue->burstTime - ready_queue->accrued);
                ready_queue->finish = time_passed;
                struct process *completed_job = duplicateProcess(ready_queue);
                // add ready_queue head to finished queue and remove process from ready_queue. release devices/mem. and check waitqueue for device requests
                used_sys_serial_devices = used_sys_serial_devices - ready_queue->allocatedDevices;
                used_memory = used_memory - ready_queue->memoryRequested;
                finished_queue = addToQueue(completed_job, finished_queue);
                struct process *temp = ready_queue;
                ready_queue = ready_queue->next;
                free(temp);
                if (wait_queue != NULL)
                {
                    //now we check wait_queue with our new device resources
                    struct process *wait_temp = wait_queue;
                    //checking wait queues now that more devices freed
                
                    while (wait_temp != NULL)
                    {
                        printf("ckecking||||||||||||");
                        struct process *temp = duplicateProcess(wait_temp);
                        ready_queue = addToQueue(temp, ready_queue);
                        if (bankers(ready_queue, wait_temp->processID, ready_queue->requestedDevices, sys_serial_devices) == 0)
                        {
                            wait_queue = removeProcess(wait_queue, temp->processID);
                            used_sys_serial_devices = used_sys_serial_devices + temp->requestedDevices;
                            temp->allocatedDevices = temp->allocatedDevices + temp->requestedDevices;
                            temp->requestedDevices = 0;
                        }else{
                            ready_queue=removeProcess(ready_queue, temp->processID);
                        }
                        wait_temp = wait_temp->next;
                    }
                }
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
                    }
                    hold_queue2_temp = hold_queue2_temp->next;
                }
            }
        }
        else
        {

            time_passed = time_passed + 1;
        }

        if (time_passed >= 1000 && ready_queue == NULL)
        {
            // this is temporary. once ready_queue is NULL, we will let the program read the final d instruction.
            // it is possible to have multiple simulations, so after final D, figure out how to read next lines
            FILE *temp_file = file;
            
            printAtTime(used_memory, next_instruction_time, time_passed, sys_memory, sys_serial_devices, hold_queue1, hold_queue2, ready_queue, wait_queue, finished_queue, ready_queue);
            break;
        }
    }
}
int main()
{

    schedule();

    return 0;
}
