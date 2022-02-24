#include <stdio.h>
#include <stdlib.h>

int num_processes;

struct node1 { // link list children
  int process_ID;
  struct node1* link;
}*child_process=NULL;
typedef struct node1 list_type;

struct node2 { // pcb array
  int parent;
  list_type* children;
}*pcb_array=NULL;
typedef struct node2 pcb_type;

void parameters(){
  
  printf("Enter maximum number of processes: "); // max array size
  scanf("%d", &num_processes);

  pcb_array = (pcb_type*)malloc(num_processes * sizeof(pcb_type));
  pcb_array[0].parent = 0;
  pcb_array[0].children = NULL;

  for (int i=1; i<num_processes; i++){
    pcb_array[i].parent = -1;
    pcb_array[i].children = NULL;
  }

  if (pcb_array[0].parent = -1){
    printf("Process 0 was created as the top-level process\n\n");
  }

  return;
}

void hierarchy(){

  list_type* curr_child;

  printf("\nProcess Hierarchy:\n");

  for (int i=0; i<num_processes; i++){
    if ((pcb_array[i].parent != -1) || (pcb_array[i].children != NULL)){
      curr_child = pcb_array[i].children;
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

  printf("Enter the parent process index: ");
  scanf("%d", &p);

  while ( (q<num_processes) && (pcb_array[q].parent != -1) ){
    q++;
  }

  child_process = (list_type*)malloc(sizeof(list_type));
  child_process->process_ID = q;
  child_process->link = NULL;

  pcb_array[q].parent = p;

  if (pcb_array[p].children == NULL){
    pcb_array[p].children = child_process;
  }else{
    curr_node = pcb_array[p].children;
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
    destroyer(pcb_array[q].children);
    
    printf("Process %d was destroyed\n", q);
    
    free(node);
    pcb_array[q].parent = -1;
    node = NULL;
  }

  return;
}

void destroy(){
  int p;
  
  printf("Enter the parent process whose descendants are to be destroyed: ");
  scanf("%d", &p);
  
  destroyer(pcb_array[p].children);

  pcb_array[p].children = NULL;

  return;
}

void quit(){

  if (pcb_array == NULL){
    printf("\nNo processes to destroy");
    return;    
  }else{
    destroyer(pcb_array[0].children);
    free(pcb_array);
  }

  printf("\n\nQuitting program...\n\n");

  return;  
}

int main(){
  int selection;

  do {
    printf("Process creation and destruction\n");
    printf("--------------------------------\n");
    printf("1) Enter Parameters\n");
    printf("2) Create a new child process\n");
    printf("3) Destroy all descendants of a process\n");
    printf("4) Quit Program and free memory\n");

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