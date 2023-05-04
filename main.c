#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
//below is the structure for a job/process.
struct process {
   int processID;
   int priority;
   int burstTime;
   int memoryRequested;
   int maxDevices;
   struct process *next;
};
int getNumber(char command_instruction[50]){
    //this function cuts of first 2 chars, then returns the int
    char temp[50];
    int length=strlen(command_instruction);
    for(int i =2;i<length ;i++){
        temp[i-2]=command_instruction[i];
    }
    return atoi(temp);
}
void schedule(char (*instructions)[100],int number_of_strings){
    //this function handles instructions it will keep reading the first line until there are no more instructions to read.
    int instruction_cout=0;
    //if there are no processes on ready or wait, we will increment time_passed by 1
    int time_passed=0;
    //system config. options
    int sys_memory=0;
    int sys_serial_devices=0;
    int quantum=0;
    while(time_passed<50) {
        char temp[50];
        //copy original strigns as strtok modifies original string. if time passed<arrival, reset string
        strcpy(temp,instructions[instruction_cout]);
        if(instructions[instruction_cout][0]=='C'){
            //sys. config
            char *token = strtok(instructions[instruction_cout], " ");
            token=strtok(NULL," ");
            int arrival = atoi(token);
            printf("arrival time:%d\n",arrival);
            printf("current time:%d\n",time_passed);
            if(arrival<=time_passed) {
               printf("-----Configuring----\n");
               token=strtok(NULL," ");
               sys_memory=getNumber(token);
               printf("this is mem %d\n",sys_memory);
               token=strtok(NULL," ");
               sys_serial_devices=getNumber(token);
               printf("serial devices: %d\n",sys_serial_devices);
               token=strtok(NULL," ");
               quantum=getNumber(token);
               printf("quantum: %d\n",sys_serial_devices);
               //if success, increase instruction count
               instruction_cout++;

            }else{
            strcpy(instructions[instruction_cout],temp);
            }
        }
        else if(instructions[instruction_cout][0]=='A'){
            //job arrival

        }
         else if(instructions[instruction_cout][0]=='Q'){
            //device request

        }
        else if(instructions[instruction_cout][0]=='L'){
            //release device request

        }else{
            //else we want to print the current system status
        }

        time_passed++;
        //if no processes, break loop
    }
}




int main(){
    //main will read in input and get the list of input.
    //lines allocates room for 50 strings of length 100. our while condition keeps
    //reading lines from the input file until the line is NULL(i.e. last line of file) andkeep adding
    //the lines to the lines matrix which when the while condition false, holds all input lines
    FILE *file = fopen("input.txt","r");
    char lines[50][100];
    char buffer[100];
    int number_of_strings =0;
    while(fgets(buffer,100,file)!=NULL){
        strcpy(lines[number_of_strings],buffer);
        number_of_strings++;
    }
    fclose(file);

    schedule(lines,number_of_strings);

    return 0;
}