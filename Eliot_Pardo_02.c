#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int num_processes;

// declare global variables including a table structure to hold scheduling information
struct node {
  int process_ID;
  int arrival;
  int total_cycles;
  int done;
  // 6 remaining fields
}*table_ptr=NULL;

typedef struct node table_type;

// optional: define a function that finds the maximum of two integers
#define max(a,b) (a > b ? a : b)



void parameters(){
  
  printf("Enter total number of processes: "); // max array size
  scanf("%d", &num_processes);

  table_ptr = (table_type*)malloc(num_processes * sizeof(table_type));
  int id;

  for (int i=0; i<num_processes; i++){
    printf(""); // prompt for all entries
    scanf("%d", &id);
    table_ptr[i].process_ID = id;
    //...
  }

  //print contents

  return;
}

void print(){
  printf("ID\tArrival Time\tTotal Cycles...\n");
  // for each process 0 to num_processes-1
  // print node.process_id node.arrival etc...

  if (table_ptr[i].done == 1)
  // if process has been scheduled ("done" field is 1, print other contents (start time, end time, turnaround time)
  return;
}

void FIFO(){
  int count=0;
  int min_time;
  int min_index;

  // for each process reset done to 0
  for (int i = 0; i < num_processes; i++){
    table_ptr[i].done = 0;
  // etc...
  }  

  // while there are still processes to schedule
  while (count<num_processes){
    min_time = INT_MAX;

    // for each process not yet scheduled 
    for (int i = 0; i < num_processes; i++){
      // check if process has earlier arrival time than current earliest and update 
      if (table_ptr[i].done == 0){
        if (table_ptr[i].arrival < ){
          // new earliest time = table_ptr[i].arrival;
          // min_index = i;
        } 
      }
    }
    // set start time, end time, turnaround time, done fields for unscheduled process with earliest arrival time
    table_ptr[min_index].start_time =
    table_ptr[min_index].end_time = 
    table_ptr[min_index].turnaround_time =  
    table_ptr[min_index].done = 1;

    count++;
  } 
  
  return;
}
void SJF(){}
void SRT(){}

void hierarchy(){

  list_type* curr_child;

  printf("\nProcess Hierarchy:\n");

  for (int i=0; i<num_processes; i++){
    if ((table_ptr[i].parent != -1) || (table_ptr[i].children != NULL)){
      curr_child = table_ptr[i].children;
      while (curr_child != NULL){
        printf("        Process %d is the parent of Process %d\n", i, curr_child->process_ID);
        curr_child = curr_child->link;
      }      
    }
  }
  printf("\n");

  return;  
}

void create(){
  int p; // parent
  int q=1; // child
  list_type* child_process;
  list_type* curr_node;

  printf("Enter the process ID: ");
  scanf("%d", &p);

  while ( (q<num_processes) && (table_ptr[q].parent != -1) ){
    q++;
  }

  child_process = (list_type*)malloc(sizeof(list_type));
  child_process->process_ID = q;
  child_process->link = NULL;

  table_ptr[q].parent = p;

  if (table_ptr[p].children == NULL){
    table_ptr[p].children = child_process;
  }else{
    curr_node = table_ptr[p].children;
    while (curr_node->link != NULL){
      curr_node = curr_node->link;
    }
    curr_node->link = child_process;
  }
  printf("Process %d was created as a child process of Process %d\n", q, p);
  hierarchy();

  return;  
}

void destroyer(list_type* node){

  if (node == NULL){
    return;
  }else{
    int q = node->process_ID;

    destroyer(node->link);
    destroyer(table_ptr[q].children);
    
    printf("Process %d was destroyed\n", q);
    
    free(node);
    table_ptr[q].parent = -1;
    node = NULL;
  }

  return;
}

void destroy(){
  int p;
  
  printf("Enter the parent process whose descendants are to be destroyed: ");
  scanf("%d", &p);
  
  destroyer(table_ptr[p].children);

  table_ptr[p].children = NULL;

  return;
}

void quit(){

  if (table_ptr == NULL){
    printf("\nNo processes to destroy");
    return;    
  }else{
    destroyer(table_ptr[0].children);
    free(table_ptr);
  }

  printf("\n\nQuitting program...\n\n");

  return;  
}

int main(){
  int selection;

  do {
    printf("Batch Scheduling\n");
    printf("----------------\n");
    printf("1) Enter Parameters\n");
    printf("2) Schedule processes with FIFO algorithm\n");
    printf("3) Schedule processes with SJF algorithm\n");
    printf("4) Schedule processes with SRT algorithm\n");
    printf("5) Quit and free memory\n");

    printf("\nEnter selection: ");
    scanf("%d", &selection);

    switch (selection) {
      case 1:
        parameters();
        break;
      case 2:
        create();
        break;
      case 3:
        destroy();
        hierarchy();
        break;
      case 4:
        quit();
        break;
      default:
        printf("\nINVALID SELECTION\n\n");
    }
  } while (selection !=4);

  return 0;
}