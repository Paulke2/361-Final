
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "queue_functions.h"

struct process *removeProcess(struct process *queue, int process_to_remove)
{
    struct process *temp = queue;
    if (queue->processID == process_to_remove)
    {
        temp = queue->next;
        free(queue);
        return temp;
    }
    else
    {
        while (queue->next->processID != process_to_remove)
        {
            queue = queue->next;
        }
    }
    struct process *temp_to_remove = queue->next;
    queue->next = queue->next->next;
    free(temp_to_remove);
    return temp;
}

void printQueue(struct process *queue)
{
    printf("%s\n", "---------printing queue------");
    while (queue != NULL)
    {
        printf("JobNumber:%d with remaining %d\n", queue->processID,queue->burstTime-queue->accrued);
        queue = queue->next;
    }
    printf("%s\n", "----------------------------");
}
int getNumber(char command_instruction[50])
{
    // this function cuts of first 2 chars, then returns the int
    char temp[50];
    int length = strlen(command_instruction);
    int counter=0;
    for (int i = 2; i < length; i++)
    {
        temp[i - 2] = command_instruction[i];
        counter++;
    }
    temp[counter]='\0';
    return atoi(temp);
}

struct process *duplicateProcess(struct process *queue)
{
    struct process *temp = (struct process *)malloc(2 + sizeof(struct process));
    temp->processID = queue->processID;
    temp->priority = queue->priority;
    temp->burstTime = queue->burstTime;
    temp->memoryRequested = queue->memoryRequested;
    temp->maxDevices = queue->maxDevices;
    temp->allocatedDevices = queue->allocatedDevices;
    temp->requestedDevices = queue->requestedDevices;
    temp->arrival = queue->arrival;
    temp->finish = queue->finish;
    temp->accrued = queue->accrued;
    temp->next = NULL;
    temp->allocatedDevices=queue->allocatedDevices;
    return temp;
}

struct process *createNewProcess(char *token, int next_instruction_time)
{
    int job_number = 0;
    int process_mem = 0;
    int maxDevices = 0;
    int burst_time = 0;
    int priority = 0;

    token = strtok(NULL, " ");
    job_number = getNumber(token);
    token = strtok(NULL, " ");
    process_mem = getNumber(token);
    token = strtok(NULL, " ");
    maxDevices = getNumber(token);
    maxDevices=(int)maxDevices;
    token = strtok(NULL, " ");
    burst_time = getNumber(token);
    burst_time=(int)burst_time;
    token = strtok(NULL, " ");
    priority = getNumber(token);
    //printf("priority: %d\n", priority);
    //printf("-----------------\n");
    struct process *newJob = (struct process *)malloc(2 + sizeof(struct process));
    newJob->processID = job_number;
    newJob->priority = priority;
    newJob->burstTime = burst_time;
    newJob->memoryRequested = process_mem;
    newJob->maxDevices = maxDevices;
    // new instances
    newJob->requestedDevices = 0;
    newJob->arrival = next_instruction_time;
    newJob->finish = 0;
    newJob->accrued = 0;
    newJob->allocatedDevices=0;
    newJob->next = NULL;
    return newJob;
}

struct process *addToQueue(struct process *newJob, struct process *queue)
{
    struct process *temp = queue;
    if (temp == NULL)
    {
        temp = newJob;
        return temp;
    }
    else
    {
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = newJob;
        return queue;
    }
}

struct process *addToQueueSJF(struct process *newJob, struct process *queue)
// This queue is for Shortest Job First
{
    struct process *temp = queue;
    if (temp == NULL || queue->burstTime >= newJob->burstTime)
    {
        // If the burst time of the new job is less than the burst time of the current head, new job
        // becomes the new head of the queue
        newJob->next = queue;
        queue = newJob;
        return queue;
    }
    else
    {
        while (temp->next != NULL && temp->next->burstTime < newJob->burstTime)
        {
            // Loops through queue until it finds where the new job burst time is less than the next
            // process in the queue.
            temp = temp->next;
        }
        // Inserts new job where the burst time is less than the next process in the queue
        newJob->next = temp->next;
        temp->next = newJob;
        return queue;
    }
}

float avgTurnaroundTime(struct process *finished_queue)
{
    // simple averaging function using linked list; used to calculate system turnaround time
    int sum = 0;
    int count = 0;
    if(finished_queue==NULL){
        return 0;
    }
    while (finished_queue != NULL)
    {
        sum = sum + (finished_queue->finish - finished_queue->arrival);
        count = count + 1;
        finished_queue = finished_queue->next;
    }
    return sum / count;
}

void deleteQueue(struct process *queue_to_delete){
    if(queue_to_delete==NULL){
        return;
    }
    struct process *temp = queue_to_delete;
    while(queue_to_delete!=NULL){
        temp=queue_to_delete->next;
        free(queue_to_delete);
        queue_to_delete=temp;
    }
}

int printAtTime(int used_memory,int time,int time_passed, int memory, int devices, struct process *hold_queue1, struct process *hold_queue2, struct process *ready_queue, struct process *wait_queue, struct process *finished_queue, struct process *onCPU)
// Prints current status of scheduler at a given time.
{
    int sum = 0;
    int count = 0;
    printf("At Time %d: \nCurrent Available Main Memory=%d \nCurrent Devices=%d \n", time, memory-used_memory, devices);
    printf("---------------------------------------------------------------------------\n");
    // Prints all of the finish jobs. TODO Jobs need Arrival Time and Finish Time  to show correct values.
    printf("Completed Jobs:\n");
    while (finished_queue != NULL)
    {
        sum = sum + (finished_queue->finish - finished_queue->arrival);
        count++;
        printf("Job ID: %d Arrival Time: %d Finish Time: %d Turn Around Time: %d\n",
               finished_queue->processID, finished_queue->arrival, finished_queue->finish, (finished_queue->finish - finished_queue->arrival));

        finished_queue = finished_queue->next;
    }
    printf("---------------------------------------------------------------------------\n");
    // Prints all currently Hold Queue 1
    printf("Hold Queue 1: \n --------------------\n");
    while (hold_queue1 != NULL)
    {
        printf("Job ID: %d Run Time: %d \n", hold_queue1->processID, hold_queue1->burstTime);
        hold_queue1 = hold_queue1->next;
    }
    printf("---------------------------------------------------------------------------\n");
    // Prints all currently on Hold Queue 2
    printf("Hold Queue 2: \n --------------------\n");
    while (hold_queue2 != NULL)
    {
        printf("Job ID: %d Run Time: %d \n", hold_queue2->processID, hold_queue2->burstTime);
        hold_queue2 = hold_queue2->next;
    }

    printf("---------------------------------------------------------------------------\n");
    // Prints all  currently Ready Queue; TODO Needs Time Accrued value
    printf("Ready Queue: \n ---------------------------------\n");
    if(ready_queue!=NULL&&ready_queue->next!=NULL){
        ready_queue=ready_queue->next;
    }
    while (ready_queue != NULL)
    {
        printf("Job ID: %d Run Time: %d Time Accrued: %d\n", ready_queue->processID, ready_queue->burstTime, ready_queue->accrued);
        ready_queue = ready_queue->next;
    }
    printf("---------------------------------------------------------------------------\n");
    // Prints all  currently Wait Queue; TODO Needs Time Accrued value
    printf("Wait Queue: \n ---------------------------------\n");
    while (wait_queue != NULL)
    {
        printf("Job ID: %d Run Time: %d Time Accrued: %d\n", wait_queue->processID, wait_queue->burstTime, wait_queue->accrued);
        wait_queue = wait_queue->next;
    }
    printf("---------------------------------------------------------------------------\n");
    // Prints all process on CPU; TODO needs Time Accrued and Time Left value; currently time left
    // is burstTime-Accrued this will only work if burstTime is updated while on CPU (decreases with time on CPU)
    printf("Running on CPU: \n---------------------------------\n");
    if(onCPU!=NULL){
        if(time_passed!=0){
    printf("Job ID: %d Time Accrued: %d Time Left: %d\n", onCPU->processID, onCPU->accrued+(time-time_passed), (onCPU->burstTime - (onCPU->accrued+(time-time_passed))));
        }else{
           printf("Job ID: %d Time Accrued: %d Time Left: %d\n", onCPU->processID, onCPU->accrued+(time-onCPU->arrival), (onCPU->burstTime - (onCPU->accrued+(time-onCPU->arrival)))); 
        }
    }
    printf("---------------------------------------------------------------------------\n");
    // Calculates the average turnaround time of the jobs on the finished queue
    float turnaround=0;
    if(count>0){
        turnaround=sum/count;
    }
    printf("System Turnaround Time: %.2f",turnaround);
    return 0;
}
