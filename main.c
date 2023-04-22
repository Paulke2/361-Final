#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//below is the structure for a job/process.
struct process {
   int processID;
   int priority;
   int burstTime;
   int memoryRequested;
   int maxDevices;
   struct process *next;
};

int main(){
    FILE *file = fopen("input.txt","r");
    char buffer[100];
    fgets(buffer,100,file);
    printf("%s",buffer[0]);
    fclose(file);
    return 0;
}