
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "queue_functions.h"

struct process *duplicateProcess(struct process *queue)
{
    struct process *temp = (struct process *)malloc(sizeof(struct process));
    temp->processID = queue->processID;
    temp->priority = queue->priority;
    temp->burstTime = queue->burstTime;
    temp->memoryRequested = queue->memoryRequested;
    temp->maxDevices = queue->maxDevices;
    temp->next = NULL;
    return temp;
}


void printQueue(struct process *queue)
{
    printf("%s\n","---------printing queue------");
    while(queue!=NULL){
        printf("JobNumber:%d\n",queue->processID);
        queue=queue->next;
    }
    printf("%s\n","----------------------------");
}
int getNumber(char command_instruction[50])
{
    // this function cuts of first 2 chars, then returns the int
    char temp[50];
    int length = strlen(command_instruction);
    for (int i = 2; i < length; i++)
    {
        temp[i - 2] = command_instruction[i];
    }
    return atoi(temp);
}
struct process *createNewProcess(char *token)
{
    int job_number = 0;
    int process_mem = 0;
    int process_devices = 0;
    int burst_time = 0;
    int priority = 0;
    printf("----New job-------\n");
    token = strtok(NULL, " ");
    job_number = getNumber(token);
    printf("job_number %d\n", job_number);
    token = strtok(NULL, " ");
    process_mem = getNumber(token);
    printf("process_mem: %d\n", process_mem);
    token = strtok(NULL, " ");
    process_devices = getNumber(token);
    printf("process_devices: %d\n", process_devices);
    token = strtok(NULL, " ");
    burst_time = getNumber(token);
    printf("burst_time: %d\n", burst_time);
    token = strtok(NULL, " ");
    priority = getNumber(token);
    printf("priority: %d\n", priority);
    printf("-----------------\n");
    struct process *newJob = (struct process *)malloc(sizeof(struct process));
    newJob->processID = job_number;
    newJob->priority = priority;
    newJob->burstTime = burst_time;
    newJob->memoryRequested = process_mem;
    newJob->maxDevices = process_devices;
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
//This queue is for Shortest Job First
{
    struct process *temp = queue;
    if (temp == NULL || queue->burstTime >= newJob->burstTime)
    {
        //If the burst time of the new job is less than the burst time of the current head, new job
        //becomes the new head of the queue
        newJob->next = queue;
        queue = newJob;
        return queue;
    }
    else
    {
        while (temp->next != NULL && temp->next->burstTime < newJob->burstTime)
        {
            //Loops through queue until it finds where the new job burst time is less than the next
            //process in the queue. 
            temp = temp->next;
        }
        //Inserts new job where the burst time is less than the next process in the queue
        newJob->next = temp->next;
        temp->next = newJob;
        return queue;
    }
}


