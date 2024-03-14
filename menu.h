#include <stdio.h>
#include <stdlib.h>

char* start(){
    printf("Enter the name of input file\n");
    char* file_name=(char*)malloc(20*sizeof(char));
    scanf("%s",file_name);
    return file_name;
}

void afterReadingFile(){
    printf("Choose the option :\n");
    printf("1)Add new process to the CPU.\n");
    printf("2)Delete a process from the CPU.\n");
    printf("3)Run the processes according to scheduler for N Quantum time(user input) and show the remaining time of processes.\n");
    printf("4)Change priorty of a process\n");
    printf("5)Exit.\n");
}