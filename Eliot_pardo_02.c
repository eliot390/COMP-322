#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define max(a,b) (a > b ? a : b)

int num_processes;

struct node {
  int process_ID;
  int arrival;
  int total_cycles;
  int total_remain;
  int done;
  int start_time;
  int started;
  int end_time;
  int turnaround;
}*table_ptr=NULL;

typedef struct node table_type;

void parameters(){
  
  printf("Enter total number of processes: ");
  scanf("%d", &num_processes);

  table_ptr = (table_type*)malloc(num_processes * sizeof(table_type));
  int id, arrival, cycles;

  for (int i=0; i<num_processes; i++){
    printf("Enter process ID: ");
    scanf("%d", &id);
    table_ptr[i].process_ID = id;
    printf("Enter arrival cycle for Process %d: ", id);
    scanf("%d", &arrival);
    table_ptr[i].arrival = arrival;
    printf("Enter total cycles for Process %d: ", id);
    scanf("%d", &cycles);
    table_ptr[i].total_cycles = cycles;
  }
  return;
}

void print(){
  printf("\nID\tArrival Time\tTotal Cycles\tStart\tEnd\tTurnaround\n");
  printf("------------------------------------------------------------------\n");
  
  for (int i=0; i<num_processes; i++)  {
    printf("%d\t%d\t%d\n", table_ptr[i].process_ID, table_ptr[i].arrival, table_ptr[i].total_cycles);
  }  

  //if (table_ptr[i].done == 1)
  // if process has been scheduled ("done" field is 1, print other contents (start time, end time, turnaround time)
  return;
}

void FIFO(){
  int count=0;
  int min_time;
  int min_index;

  for (int i = 0; i < num_processes; i++){
    table_ptr[i].done = 0;
  }

  // while there are still processes to schedule
  while (count<num_processes){
    min_time = INT_MAX;
    
    for (int i = 0; i < num_processes; i++){ // for each process not yet scheduled       
      if (table_ptr[i].done == 0){ // check if process has earlier arrival time than current earliest and update 
        if (table_ptr[i].arrival < num_processes){
          // new earliest time = table_ptr[i].arrival;
          // min_index = i;
        } 
      }
    }
    // set start time, end time, turnaround time, done fields for unscheduled process with earliest arrival time
    table_ptr[min_index].start_time =
    table_ptr[min_index].end_time = 
    table_ptr[min_index].turnaround =  
    table_ptr[min_index].done = 1;

    count++;
  } 
  
  return;
}
void SJF(){}
void SRT(){}

void quit(){       
  free(table_ptr);
  printf("\n\nQuitting program...\n\n");

  return;  
}

int main(){
  int selection;

  do {
    printf("\nBatch Scheduling\n");
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
        print();
        break;
      case 2:
        FIFO();
        break;
      case 3:
        SJF();
        break;
      case 4:
        SRT();
        break;
      case 5:
        quit();
        break;
      default:
        printf("\nINVALID SELECTION\n\n");
    }
  } while (selection !=5);

  return 0;
}