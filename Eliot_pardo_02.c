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
  printf("\nID\tArrival\tTotal\tStart\tEnd\tTurnaround\n");
  printf("--------------------------------------------------\n");
  
  for (int i=0; i<num_processes; i++){
    if (table_ptr[i].done ==1){
      printf("%d\t%d\t%d\t%d\t%d\t%d\n", 
    table_ptr[i].process_ID, table_ptr[i].arrival, table_ptr[i].total_cycles,
    table_ptr[i].start_time, table_ptr[i].end_time, table_ptr[i].turnaround);
    }else{
      printf("%d\t%d\t%d\t%s\t%s\t%s\n", 
    table_ptr[i].process_ID, table_ptr[i].arrival, table_ptr[i].total_cycles, "-", "-", "-");
    }    
  }  

  return;
}

void FIFO(){
  int count=0;
  int min_remaining_time;
  int min_index;
  int curr_cycle=0;

  for (int i = 0; i < num_processes; i++){
    table_ptr[i].done = 0;
  }

  // while there are still processes to schedule
  while (count < num_processes){
    min_remaining_time = INT_MAX;
    
    for (int i = 0; i < num_processes; i++){ // for each process not yet scheduled       
      if (table_ptr[i].done == 0){ // check if process has earlier arrival time than current earliest and update 
        if (table_ptr[i].arrival < min_remaining_time){
          min_remaining_time = table_ptr[i].arrival;
          min_index = i;
        } 
      }
    }
    // set start time, end time, turnaround time, done fields for unscheduled process with earliest arrival time
    table_ptr[min_index].start_time = max(curr_cycle, table_ptr[min_index].arrival);
    table_ptr[min_index].end_time = table_ptr[min_index].start_time + table_ptr[min_index].total_cycles;
    table_ptr[min_index].turnaround = table_ptr[min_index].end_time - table_ptr[min_index].arrival;
    table_ptr[min_index].done = 1;

    count++;
    curr_cycle = table_ptr[min_index].end_time;
  }

  print();
  
  return;
}

void SJF(){
  int count=0;
  int min_remaining_time;
  int min_index;
  int curr_cycle=0;
  int done_flag;

  // for each process, reset "done" field to 0 
  for (int i = 0; i < num_processes; i++){
    table_ptr[i].done = 0;
  }

  // while there are still processes to schedule
  while (count < num_processes){
    // initilize the lowest total cycle time to INT_MAX (largest integer value)
    min_remaining_time = INT_MAX;
    done_flag=0;
    
    // for each process not yet scheduled
    for (int i = 0; i < num_processes; i++){
      // check if process has shorter cpu time than current shortest and update 
      if (table_ptr[i].done == 0){
        if ((table_ptr[i].total_cycles < min_remaining_time) && (table_ptr[i].arrival <= curr_cycle)){
          min_remaining_time = table_ptr[i].total_cycles;
          min_index = i;
          done_flag = 1;
        } 
      }
    }

    if (done_flag == 0){
      curr_cycle++;
    } else {
      // set start time, end time, turnaround time, done fields for unscheduled process with lowest (and available) total cycle time
      table_ptr[min_index].start_time = max(curr_cycle, table_ptr[min_index].arrival);
      table_ptr[min_index].end_time = table_ptr[min_index].start_time + table_ptr[min_index].total_cycles;
      table_ptr[min_index].turnaround = table_ptr[min_index].end_time - table_ptr[min_index].arrival;
      table_ptr[min_index].done = 1;

      // update current cycle time and increment number of processes scheduled
      count++;
      curr_cycle = table_ptr[min_index].end_time;
    }
  }

  print();
  
  return;
}

void SRT(){
  int count=0;
  int min_remaining_time;
  int min_index;
  int curr_cycle=0;
  int done_flag;

  // for each process, reset "done", "total_remaining" and "already_started" fields to 0 
  for (int i = 0; i < num_processes; i++){
    table_ptr[i].done = 0;
    table_ptr[i].started = 0;
    table_ptr[i].total_remain = table_ptr[i].total_cycles;
  }

  // while there are still processes to schedule
  while (count < num_processes){
    // initilize the lowest total remaining time to INT_MAX (largest integer value)
    min_remaining_time = INT_MAX;
    done_flag=0;
    
    // for each process not yet scheduled
    for (int i = 0; i < num_processes; i++){
      // check if process has shorter remaining time than current shortest and update 
      if (table_ptr[i].done == 0){
        if ((table_ptr[i].total_remain < min_remaining_time) && (table_ptr[i].arrival <= curr_cycle)){
          min_remaining_time = table_ptr[i].total_remain;
          min_index = i;
          done_flag = 1;
        } 
      }
    }

    // check if process already partially-scheduled
    if (done_flag == 0){
      curr_cycle++;
    } else {
      if (table_ptr[min_index].started == 0){
        // if so, set "start time", "already_started" fields of process with lowest (and available) total remaining cycle time
        table_ptr[min_index].started = 1;
        table_ptr[min_index].start_time = max(curr_cycle, table_ptr[min_index].arrival);
        table_ptr[min_index].end_time = table_ptr[min_index].start_time+1;
        table_ptr[min_index].turnaround = table_ptr[min_index].end_time - table_ptr[min_index].arrival;
      } else {
        // set end time, turnaround time of process with lowest (and available) total remaining cycle time
        table_ptr[min_index].end_time = table_ptr[min_index].start_time+1;
        table_ptr[min_index].turnaround = table_ptr[min_index].end_time - table_ptr[min_index].arrival;
        table_ptr[min_index].total_remain--;
      }

      // decrement total remaining time of process with lowest (and available) total remaining cycle time
      // table_ptr[min_index].total_remain--;

      if (table_ptr[min_index].total_remain == 0){
        table_ptr[min_index].done = 1;
        count++;
        curr_cycle++;
      }
    }
    
  }

  print();
  
  return;
}

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
