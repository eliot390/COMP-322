#include <stdio.h>
#include <stdlib.h>

// decalare global variables including linked list of blocks containing 
// id, starting address, ending address, link to next node in list
struct node {
  int id;
  int start_address;
  int end_address;
  struct node* link;
}* block_ptr = NULL;

typedef struct node block_type;

int pm_size;
int remaining_memory;

void parameters(){ // option #1
    
  printf("Enter size of physical memory: ");
  scanf("%d", &pm_size);

  // initilize remaining memory to size of physical memory
  remaining_memory = pm_size;

  // allocate memory for linked list of blocks and
  // create "dummy" block with id -1, staring/ending address of 0 and link of NULL
  block_ptr = (block_type *)malloc(sizeof(block_type));
  block_ptr->id = -1;
  block_ptr->start_address = 0;
  block_ptr->end_address = 0;
  block_ptr->link = NULL;

  return;
}

void printBlocks(){

  block_type* curr_node = block_ptr->link;

  // print header of table containing fields: ID, Start, End
  printf("\nID\tStart\tEnd\n");
  printf("-------------------\n");

  // print each block in list with while-loop, loop until end of list, advancing "current block" pointer
  while (curr_node != NULL){
    printf("%d\t%d\t%d\n", curr_node->id, curr_node->start_address, curr_node->end_address);
    curr_node = curr_node->link;
  }

  return;
}

void allocateFirstFit(){ // option #2

  int block_ID;
  int block_size;
  int hole_start;
  int hole_end;
  int hole_size;
  block_type* new_block;
  block_type* curr_node = block_ptr; // initialize "current block" to compare hole

  // prompt for block id & block size
  printf("Enter block ID: ");
  scanf("%d", &block_ID);
  printf("Enter block size: ");
  scanf("%d", &block_size);
  
	// if block size is larger than remaining memory, print message, return
  if (block_size > remaining_memory){
    printf("\nNot enough memory.\n");
    return;
  }
  
	// allocate memory for new block, set block id
  new_block = (block_type *)malloc(sizeof(block_type));
  new_block->id = block_ID;

	// if block list is "empty", set fields for new block, link to block list, reduce remaining memory, return
  if (block_ptr->link == NULL){    
    new_block->start_address = block_ptr->end_address;
    new_block->end_address = block_ptr->start_address+block_size;
    new_block->link = NULL;
    block_ptr->link = new_block;
    remaining_memory -= block_size;
    printBlocks();
    return;
  }
  
	// while not end of list, loop
  while (curr_node != NULL){
    //if duplicate id, print message, return
    if (block_ID == curr_node->id){
      printf("\nDuplicates not allowed.\n");
      return;
    }
    curr_node = curr_node->link;
  }

  curr_node = block_ptr;
  while (curr_node != NULL){
    //set values for start and end of current hole
    hole_start = curr_node->end_address;

    if(curr_node->link == NULL){
      hole_end = pm_size;
    }else{
      hole_end = curr_node->link->start_address;
    }

    hole_size = hole_end - hole_start;

    // if block fits in hole, set fields for new block, link to block list, reduce remaining memory, return
    if (block_size <= hole_size){
      new_block->start_address = hole_start;
      new_block->end_address = new_block->start_address+block_size;
      new_block->link = curr_node->link;
      curr_node->link = new_block;
      remaining_memory -= block_size;
      printBlocks();
      return;
    }

    // advance "current block" pointer	
    curr_node = curr_node->link;    
  }
				
	// if end of list reached, print message no fitting hole
  if(curr_node == NULL){
    printf("\nNo fitting hole.\n");
  }

  return;
}

void allocateBestFit(){ // option #3

  int block_ID;
  int block_size;
  int hole_start;
  int hole_end;
  int hole_size;
  int best_hole = pm_size;
  int best_start;
  int at_least_one = 0;
  block_type* best_block_ptr;
  block_type* new_block;
  block_type* curr_node = block_ptr; // initialize "current block" to compare hole

  // prompt for block id & block size
  printf("Enter block ID: ");
  scanf("%d", &block_ID);
  printf("Enter block size: ");
  scanf("%d", &block_size);
  
	// if block size is larger than remaining memory, print message, return
  if (block_size > remaining_memory){
    printf("\nNot enough memory.\n");
    return;
  }
  
	// allocate memory for new block, set block id
  new_block = (block_type *)malloc(sizeof(block_type));
  new_block->id = block_ID;

	// if block list is "empty", set fields for new block, link to block list, reduce remaining memory, return
  if (block_ptr->link == NULL){    
    new_block->start_address = block_ptr->end_address;
    new_block->end_address = block_ptr->start_address+block_size;
    new_block->link = NULL;
    block_ptr->link = new_block;
    remaining_memory -= block_size;
    printBlocks();
    return;
  }
  
	// while not end of list, loop
  while (curr_node != NULL){
    //if duplicate id, print message, return
    if (block_ID == curr_node->id){
      printf("\nDuplicates not allowed.\n");
      return;
    }
    curr_node = curr_node->link;
  }

  curr_node = block_ptr;
  while (curr_node != NULL){
    //set values for start and end of current hole
    hole_start = curr_node->end_address;

    if(curr_node->link == NULL){
      hole_end = pm_size;
    }else{
      hole_end = curr_node->link->start_address;
    }

    hole_size = hole_end - hole_start;

    // if block fits in hole, set fields for new block, link to block list, reduce remaining memory, return
    if (block_size <= hole_size){
      // set flag "at least one" fitting hole found
      at_least_one = 1;

      // if current hole is smaller than best so far
      // set new value for "best so far", "best start", copy "best block" to current block
      if (hole_size < best_hole){
        best_hole = hole_size;
        best_start = hole_start;
        best_block_ptr = curr_node;
      }
    }

    // advance "current block" pointer	
    curr_node = curr_node->link;    
  }

  // if no hole was found large enough, print message, return
  if(at_least_one == 0){
    printf("\nNo hole found.\n");
    return;
  }

  // set fields for new block, link to block list, reduce remaining memory
  new_block->start_address = best_start;
  new_block->end_address = new_block->start_address+block_size;
  new_block->link = best_block_ptr->link;
  best_block_ptr->link = new_block;
  remaining_memory -= block_size;
  printBlocks();

  return;  
}

void deallocate(){ // option #4
  int block_ID;
  int block_size;

  // prompt for block id
  printf("Enter block ID: ");
  scanf("%d", &block_ID);

	// initialize "current block" to head of list
  block_type* new_block;
  block_type* prev_node;
  block_type* curr_node = block_ptr;

	// while id not found and end of block list not reached
  while ((curr_node != NULL) && (block_ID != curr_node->id)){
    // advance "previous block" pointer and "current block" pointer
    prev_node = curr_node;
    block_size = curr_node->end_address-curr_node->start_address;
    curr_node = curr_node->link;
  }  
		
	// if "current block" is NULL, print message id not found
  if(curr_node == NULL){
    printf("Block ID not found.\n");
    return;
  // else remove block, adjusting "previous block" pointer around it, 
  // increment remaining memory with block size, free block with matching id
  }else{
    prev_node->link = curr_node->link;
    remaining_memory += block_size;
    free(curr_node);
  }

  return;
}

void defrag(){ // option #5
  int prev_node_end = 0;
  int curr_size;

  // initialize "current block" to head of list
  block_type* new_block;
  block_type* curr_node = block_ptr;

	// while end of block list not reached
  // adjust start and end fields of each block, compacting together
  while (curr_node != NULL){
    curr_size = curr_node->end_address-curr_node->start_address;
    curr_node->start_address = prev_node_end;
    curr_node->end_address = curr_node->start_address+curr_size;
    prev_node_end = curr_node->end_address;
    curr_node = curr_node->link;
  }

  return;
}

void quit(){ //option #6
  if (block_ptr->link != NULL){
    free(block_ptr->link);
  }
  printf("\nQuitting program...\n\n");

  return;  
}

int main(){
  int selection;

  do {
    printf("\nMemory Allocation\n");
    printf("-----------------\n");
    printf("1) Enter Parameters\n");
    printf("2) Allocate memory for block using First-fit\n");
    printf("3) Allocate memory for block using Best-fit\n");
    printf("4) Deallocate memory for block\n");
    printf("5) Defragment memory\n");
    printf("6) Quit program\n");

    printf("\nEnter selection: ");
    scanf("%d", &selection);

    switch (selection) {
      case 1:
        parameters();
        break;
      case 2:
        allocateFirstFit();
        break;
      case 3:
        allocateBestFit();
        break;
      case 4:
        deallocate();
        printBlocks();
        break;
      case 5:
        defrag();
        printBlocks();
        break;
      case 6:
        quit();
        break;
      default:
        printf("\nINVALID SELECTION\n\n");
    }
  } while (selection !=6);

  return 0;
}
