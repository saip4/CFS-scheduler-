#include <stdio.h>
#include "menu.h"
#include "redblack.c"
#include <string.h>

void insert_node(struct list_node* TaskList,struct list_node* node){
    while(TaskList->next!=NULL){
        TaskList=TaskList->next;
    }
    TaskList->next=node;
}

//Reads the file and returns array of tasks
void readFile(){       //also updates the value of global variable No_of_Tasks
    int n;
    FILE* fptr;
    fptr=fopen("input.txt","r");
    fscanf(fptr,"%d",&n);
    No_Of_Tasks=n;
    
    for(int i=0;i<n;i++){
        task* process=(task*)malloc(sizeof(task));
        char name[15];
        float time;
        int priority;
        fscanf(fptr,"%s %f %d",&(process->name),&process->burst_time,&process->nice_value);
        struct list_node* temp=(struct list_node*)malloc(sizeof(struct list_node));
        temp->mytask=process;
        temp->next=NULL;
        if(TaskList==NULL){
            TaskList=temp;
        }
        else{
            insert_node(TaskList,temp);
        }
    }
    return;
}

void printTaskList(){
	struct list_node* temp=TaskList;
	int i=1;
	while(temp!=NULL){
		printf("%d :\nTask Name = %s\nVruntime = %f\nBurstTime = %f\nNice Value = %d \n",i,temp->mytask->name,temp->mytask->vruntime,temp->mytask->burst_time,temp->mytask->nice_value);
		i++;
		temp=temp->next;
	}
}



//takes input from the user for a new process and enter the process into the existing tree
void addNewProcess(){
    int n;
	printf("Enter the number of tasks to be entered: ");
	scanf(" %d",&n);
    No_Of_Tasks +=n;
	//TaskList= (task**)realloc(TaskList, No_Of_Tasks*sizeof(task*));

	for(int i=0;i<n;++i){
        task* process=(task*)malloc(sizeof(task));
		printf("Enter the name of the task: ");
		scanf(" %s",process->name);
		printf("Enter the Burst time of %s: ",process->name);
		scanf(" %f", &process->burst_time);
		printf("Enter the nice value of %s", process->name);
		scanf(" %d",&process->nice_value);
        process->vruntime=0;
        struct list_node* temp=(struct list_node*)malloc(sizeof(struct list_node));
        temp->mytask=process;
        temp->next=NULL;
        insert_node(TaskList,temp);
        int initialPreference =(int)(process->nice_value + process->burst_time);//change this logic *********************************
        rb_node* tree_node=create_node(initialPreference,process);
        insert(tree_node);
	}
    // printTaskList();
    // printTree(TaskTree->root);
    // for(int i =No_Of_Tasks-n-1;i<No_Of_Tasks;++i){    
    //     create_node(preference, TaskList+i);
    // }   
	
}

int deleteProcessTaskList(char* delName){
    // struct list_node* temp=List;
    // int same;
    // while(temp!=NULL){
    //     same=strcmp(temp->mytask->name, delName);
    //     if(same==0){

    //     }
    // }
    struct list_node* temp;
    struct list_node* prev;
    temp = TaskList;
    prev = TaskList;
    if(!strcmp(temp->mytask->name,delName)){
        TaskList=temp->next;
        free(temp);
        return 0;
    }
    else{
        temp=temp->next;
        while(temp!=NULL){
            if(!strcmp(temp->mytask->name,delName)){
                prev->next=temp->next;
                free(temp);
                return 0;
            }
            else{
                temp=temp->next;
                prev=prev->next;
            }
        }
    }
    return 1;
    
}

//takes task name of input from the user and deletes the process from the Tasktree. Uses TaskList and the defined expression for initial preference to find the task
void deleteProcess(){
    char delName[15];
    int same;
    printf("Enter the name of the process to be deleted: ");
    scanf(" %s",delName);
    struct list_node* temp=TaskList;
    same=deleteProcessTaskList(delName);
    if(same == 0){
        search(TaskTree->root, delName);
    }else{
        printf("The entered string is not a valid process.\n");
    }
    printTaskList();
    printTree(TaskTree->root);

}

//takes input from the user that for how many nanoseconds processes should run and then prints the burstTime left for each process. Each nanosecond is divided into Quantas equal to number of processes.
void runProcesses(){
    float x;
    printf("Enter for how many nanoseconds processes should run : ");
    scanf("%f",&x);
    float i=0;
    while(i!=x){
        float Quanta=1.0/(TaskTree->size);
        //printf("Quanta = %f\n",Quanta);
        rb_node* cur_process=minValueNode(TaskTree->root);
        //printf("Min Value = %d\n",cur_process->data);
        delete(TaskTree->root,cur_process->data);
        printTree(TaskTree->root);
        if(cur_process->mytask->nice_value==-1){
            cur_process->data+=7;
            cur_process->mytask->vruntime+=Quanta;
            if(cur_process->mytask->vruntime<cur_process->mytask->burst_time){
                //printf("Inserting  vruntime== %f \n",cur_process->mytask->vruntime);
                insert(cur_process);
                printTree(TaskTree->root);
                printf("Inserted\n");
            }
            else{
                TaskTree->size--;
                No_Of_Tasks--;
            }
        }
        else if(cur_process->mytask->nice_value==0){
            cur_process->data+=5;
            cur_process->mytask->vruntime+=Quanta;
            
            if(cur_process->mytask->vruntime<cur_process->mytask->burst_time){
                printTree(TaskTree->root);
                printf("Inserting  vruntime== %f \n",cur_process->mytask->vruntime);
                //insert(TaskTree,cur_process);
                printf("Inserted\n");
                printTree(TaskTree->root);
                printf("printed\n");
            }
            else{
                TaskTree->size--;
                No_Of_Tasks--;
            }
}
        else{
            cur_process->data+=3;
            cur_process->mytask->vruntime+=Quanta;
            if(cur_process->mytask->vruntime<cur_process->mytask->burst_time){
                printf("root data = %d\n",TaskTree->root->data);
                insert(cur_process);
                printf("Inserted\n");
                printf("root data = %d\n",TaskTree->root->data);
                printTree(TaskTree->root);
                printf("Printed\n");
            }
            else{
                TaskTree->size--;
                No_Of_Tasks--;
            }
        }
        i+=Quanta;
    }

}


//takes task name of input from the user and deletes the process from the Tasktree. And insert it again with different Nice_value
void changePriority(){
    char changeName[15];
    int same;
    int newValue;
    printf("Enter the name of the process whose priority is to be changed : ");
    scanf(" %s",changeName); 
    printf("Enter the new priority value : ");
    scanf("%d",&newValue);
    struct list_node* temp=TaskList;
    while(temp!=NULL){
        if(!strcmp(changeName,temp->mytask->name)){
            temp->mytask->nice_value=newValue;
        }
        temp=temp->next;
    }
    printTaskList();
    
}


void run(int n){
    switch(n){
        case 1: addNewProcess();
                break;
        case 2: deleteProcess();
                break;
        case 3: runProcesses();
                break;
        case 4: changePriority();
                break;
    }
}

void main(){
    //char* file_name=start();
    TaskTree=create_tree();
    readFile();
    //printTaskList();
    struct list_node* process=TaskList;
    for(int i=0;i<No_Of_Tasks;i++){
        int initial_preference = process->mytask->nice_value + process->mytask->burst_time; //Defining an expression
        rb_node* node=create_node(initial_preference,process->mytask);
        printf("node->value == %d :\n",node->data);

        insert(node);
        printf("Root->mytask->value == %d    %d :\n",TaskTree->root->data,i);
        printTree(TaskTree->root);
        printf("\n");
        process=process->next;
    }
    free(process);
    //printTree(TaskTree->root);
    int n;
    do{
        afterReadingFile();
        scanf("%d",&n);
        run(n);
    }while(n!=5);

}