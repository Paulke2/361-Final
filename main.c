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
    //main will read in input and get the list of input.
    //lines allocates room for 50 strings of length 100. our while condition keeps
    //reading lines from the input file until the line is NULL(i.e. last line of file) andkeep adding
    //the lines to the lines matrix which when the while condition false, holds all input lines
    FILE *file = fopen("input.txt","r");
    char lines[50][100];
    char buffer[100];
    int index =0;
    while(fgets(buffer,100,file)!=NULL){
        strcpy(lines[index],buffer);
        index++;
    }
    printf("%s",lines[0]);
    printf("%s",buffer);
    fclose(file);
    return 0;
}