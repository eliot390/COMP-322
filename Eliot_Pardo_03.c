#include <stdio.h>
#include <stdlib.h>

int num_processes;
int num_resources;

int *resource = NULL;
int *available = NULL;
int *max_claim = NULL;
int *allocated = NULL;
int *needed = NULL;

void print_resources(){
  printf("\nResources:\n");
  for(int i=0; i<num_resources; i++){
    printf("\tr%d", i);
  }
  printf("\n");  
  
  for (int i=0; i<num_resources; i++){
    printf("\t%d", resource[i]);    
  }
  printf("\n");

  return;
}

void print_avilable(){
  printf("\nAvailable:\n");
  for(int i=0; i<num_resources; i++){
    printf("\tr%d", i);
  }
  printf("\n");  
  
  for (int i=0; i<num_resources; i++){
    printf("\t%d", available[i]);    
  }
  printf("\n");

  return;
}

void print_maxclaim(){
  printf("\nMaximum:\n");
  for(int i=0; i<num_resources; i++){
    printf("\tr%d", i); // r0 r1 r2...
  }
  printf("\n");

  for(int i=0; i<num_processes; i++){
    printf("p%d\t", i);
    for (int j=0; j<num_resources; j++){
      printf("%d\t", max_claim[i*num_resources+j]);
    }
    printf("\n");
    
  }
  printf("\n");

  return;
}

void print_allocated(){
  printf("\nAllocated:\n");
  for(int i=0; i<num_resources; i++){
    printf("\tr%d", i); // r0 r1 r2...
  }
  printf("\n");

  for(int i=0; i<num_processes; i++){
    printf("p%d\t", i);
    for (int j=0; j<num_resources; j++){
      printf("%d\t", allocated[i*num_resources+j]);
    }
    printf("\n");
    
  }
  printf("\n");

  return;
}

void print_needed(){
  printf("\nNeed:\n");
  for(int i=0; i<num_resources; i++){
    printf("\tr%d", i); // r0 r1 r2...
  }
  printf("\n");

  for(int i=0; i<num_processes; i++){
    printf("p%d\t", i);
    for (int j=0; j<num_resources; j++){
      printf("%d\t", (max_claim[i*num_resources+j])-(allocated[i*num_resources+j]));
    }
    printf("\n");
    
  }
  printf("\n");

  return;
}
void parameters(){ // option #1
  int unit_resources;
  
  printf("Enter total number of processes: ");
  scanf("%d", &num_processes);
  printf("Enter total number of resources: ");
  scanf("%d", &num_resources);

  // allocate memory for vectors and arrays: resource, available, max_claim, allocated, need
  resource = (int *)malloc(num_resources * sizeof(int));
  available = (int *)malloc(num_resources * sizeof(int));
  max_claim = (int *)malloc(num_resources * num_processes * sizeof(int));
  allocated = (int *)malloc(num_resources * num_processes * sizeof(int));
  needed = (int *)malloc(num_resources * num_processes * sizeof(int));  

  // for each resource, prompt for number of units, set resource and available vectors indices
  printf("Enter number of units for resources (r0 to r%d): ", num_resources-1);
  for (int j=0; j<num_resources; j++){    
    scanf("%d", &unit_resources);
    resource[j] = unit_resources;
    available[j] = unit_resources;
  }

  // for each process, for each resource, prompt for maximum number of units requested by process, update maxclaim and need arrays
  for (int i=0; i<num_processes; i++){
    printf("Enter maximum units process p%d will request from each resource (r0 to r%d): ", i, num_resources-1);
    for (int j=0; j<num_resources; j++){
      scanf("%d", &unit_resources);
      max_claim[i*num_resources+j] = unit_resources;
      needed[i*num_resources+j] = unit_resources;
    }
  }
  
  // for each process, for each resource, prompt for number of resource units allocated to process 
  for (int i=0; i<num_processes; i++){
    printf("Enter number of units of each resource (r0 to r%d) allocated to process p%d: ", num_resources-1, i);
    for (int j=0; j<num_resources; j++){
      scanf("%d", &unit_resources);
      allocated[i*num_resources+j] = unit_resources;
      needed[i*num_resources+j] -= unit_resources;
      available[j] -= unit_resources;
    }
  }

  print_resources();
  print_avilable();
  print_maxclaim();
  print_allocated();
  print_needed();

  return;
}

void bankers(){ //option #2
  int num_done=0;
  int *done = (int *)calloc(num_processes, sizeof(int));
  int less_than = 1;
  int at_least_one = 1;

  // while not all processes are processed
  while((num_done < num_processes)&&(at_least_one==1)){
    at_least_one = 0;
    for(int i=0; i<num_processes; i++){
      less_than = 1;
      if(done[i] == 0){
        printf("Comparing: < ");
        for(int j=0; j<num_resources; j++){
          printf("%d ", needed[i*num_resources+j]);
        }

        printf("> <= < ");
        for(int j=0; j<num_resources; j++){
          printf("%d ", available[j]);
        }
        printf("> : ");

        for(int j=0; j<num_resources; j++){
          less_than &= (needed[i*num_resources+j] <= available[j]);
        }

        // if each resource is available
        if (less_than == 1){
          // print message that process can be processed
          printf("Yes --> p%d can be processed\n", i);
          // update number of available units of resource
          at_least_one = 1;
          for(int j=0; j<num_resources; j++){
            available[j] += allocated[i*num_resources+j];
            // free all resources allocated to process
            // increment number of sequenced processes
            allocated[i*num_resources+j] = 0;
            num_done++;
            done[i]=1;
          }
        }else{
          printf("No --> p%d could not be processed\n", i);
        }
      }
    }
  }

  if(num_done < num_processes){
    printf("Deadlock among processes: ");
    for (int i=0; i<num_processes; i++){
      if(done[i]==0){
        printf("p%d ", i);
      }
    }
    printf("\n");
  }
  
  return;
}

void quit(){ //option #3
  if (resource != NULL){
    free(resource);
    //repeat for all
  }
  printf("\n\nQuitting program...\n\n");

  return;  
}

int main(){
  int selection;

  do {
    printf("\nBanker's Algorithm\n");
    printf("------------------\n");
    printf("1) Enter Parameters\n");
    printf("2) Determine safe sequence\n");
    printf("3) Quit program\n");

    printf("\nEnter selection: ");
    scanf("%d", &selection);

    switch (selection) {
      case 1:
        parameters();
        break;
      case 2:
        bankers();
        break;
      case 3:
        quit();
        break;
      default:
        printf("\nINVALID SELECTION\n\n");
    }
  } while (selection !=3);

  return 0;
}
