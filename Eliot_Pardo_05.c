#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

// Declare global variables, including dynamic array to store sequence of traversed tracks
int* sequence = NULL;
int sequence_size;

void parameters(){ // option #1
    
  printf("Enter size of sequence: ");
  scanf("%d", &sequence_size);

  // allocate memory for the sequence of traversed tracks
  sequence = (int *)malloc(sequence_size * sizeof(int));

  // prompt for starting track, store in index 0
  printf("Enter starting track: ");
  scanf("%d", &sequence[0]);
  
  // prompt for sequence of tracks to seek, store in index 1 to "sequence size-1"
  printf("Enter sequence of tracks to seek: ");
  for (int i=1; i<sequence_size; i++){
    scanf("%d", &sequence[i]);
  }  

  return;
}


void FIFO(){ // option #2
  int total_distance = sequence[0];

  // calculate total distance of traversed tracks
  for (int i=1; i<sequence_size; i++){
    total_distance += abs(sequence[i]-sequence[i-1]);
  }
  
  // print sequence of traversal
  printf("Traversed sequence: ");
  for (int i=0; i<sequence_size; i++){
    printf("%d ", sequence[i]);
  }
  
	// print total distance of tracks traversed
  printf("\nThe distance of the traversed tracks is: %d", total_distance);
  printf("\n");

  return;
}

void SSTF(){ // option #3
  int total_distance = sequence[0];
  int curr_track = sequence[0];  // initialize current track and distance traversed to starting track
  int* done = (int *)calloc(sequence_size, sizeof(int));
  int num_done = 1;
  int shortest_distance;
  int shortest_track;

	// begin printing sequence of traversal
  // until every track is traversed
  printf("Traversed sequence: %d ", sequence[0]);
  while (num_done < sequence_size){	
		// initilize shortest distance to INT_MAX
    shortest_distance = INT_MAX;
		// for each track in sequence
    for (int i=1; i<sequence_size; i++){
      // if not already traversed
      if(done[i]==0){
        //if distance to traverse is shorter than current shortest distance
        if( abs(sequence[i]-curr_track) < shortest_distance){
          // set current shortest distance and index of the track	w/ shortest distance
          shortest_distance = abs(sequence[i]-curr_track);
          shortest_track = i;
        }        
      }					
    }			
		// set "done" value for track w/shortest distance to 1 (mark as traversed)
    // increment number of tracks that have been traversed
    // update total distance traversed
    // set current track to new position, print position
    done[shortest_track] = 1;
    num_done++;
    total_distance += shortest_distance;
    curr_track = sequence[shortest_track];
    printf("%d ", curr_track);    
  }					
  	
  // print total distance traversed
  printf("\nThe distance of the traversed tracks is: %d", total_distance);
  printf("\n");
	
	return;
}

void Scan(){ // option #4
  int direction = 0;
  int total_distance = sequence[0];
  int curr_track = sequence[0];  // initialize current track and distance traversed to starting track
  int* done = (int *)calloc(sequence_size, sizeof(int));
  int num_done = 1;
  int shortest_distance;
  int shortest_track;
  int at_least_one;

  //prompt for initial direction (0=descreasing, 1=increasing)
  printf("Enter initial direction: (0=decreasing, 1=increasing): ");
  scanf("%d", &direction);

	// begin printing sequence of traversal
  // until every track is traversed
  printf("Traversed sequence: %d ", sequence[0]);
  while (num_done < sequence_size){
    at_least_one = 0;
		// initilize shortest distance to INT_MAX
    shortest_distance = INT_MAX;
		// for each track in sequence
    for (int i=1; i<sequence_size; i++){
      // if not already traversed
      if(done[i]==0){
				//if distance to traverse is shorter than current shortest distance in the current direction
        if(
          (abs(sequence[i]-curr_track < shortest_distance)) &&
          (((sequence[i]-curr_track)>0) && direction==1) || 
          (((sequence[i]-curr_track)<0) && direction==0)
          ){
          // set current shortest distance and index of the track	w/ shortest distance
          shortest_distance = abs(sequence[i]-curr_track);
          shortest_track = i;
          at_least_one = 1;
        }
      }
    }
    // if at least one track was traversed
    if(at_least_one==1){
      // set "done" value for track w/shortest distance to 1 (mark as traversed)
      // increment number of traversed tracks
      // update total distance traversed
      //set current track to new position, print position
      done[shortest_track]=1;      
      num_done++;			
      total_distance += shortest_distance;
      curr_track=sequence[shortest_track];
      printf("%d ", curr_track);
    } else {
      // else change direction
      direction = 1-direction;
    }
  }		
 	// print total distance traversed
  printf("\nThe distance of the traversed tracks is: %d", total_distance);
  printf("\n");

	return;
}

void CScan(){ // option #5
  int end_reached=0;
  int total_distance = sequence[0];
  int curr_track = sequence[0];  // initialize current track and distance traversed to starting track
  int* done = (int *)calloc(sequence_size, sizeof(int));
  int num_done = 1;
  int shortest_distance;
  int shortest_track;
  int at_least_one;

	// begin printing sequence of traversal
  // until every track is traversed
  printf("Traversed sequence: %d ", sequence[0]);
  while (num_done < sequence_size){
    at_least_one = 0;
		// initilize shortest distance to INT_MAX
    shortest_distance = INT_MAX;
		// for each track in sequence
    for (int i=1; i<sequence_size; i++){
      // if not already traversed
      if(done[i]==0){
				//if distance to traverse is shorter than current shortest distance in the current direction
        if((sequence[i]-curr_track < shortest_distance) && ((sequence[i]-curr_track)>0)){
          // set current shortest distance and index of the track	w/ shortest distance
          shortest_distance = sequence[i]-curr_track;
          shortest_track = i;
          at_least_one = 1;
        }
      }
    }
    // if at least one track was traversed
    if(at_least_one==1){
      // set "done" value for track w/shortest distance to 1 (mark as traversed)
      // increment number of traversed tracks
      done[shortest_track]=1;      
      num_done++;

      // if largest track was reached
      if(end_reached==1){        
        // update total distance traversed by derementing by distance to track (subtracts distance from 0 to track)
        // reset "largest track" flag
        total_distance -= shortest_distance;
        end_reached = 0;        
      }else{
        // update total distance traversed by distance to track
        total_distance += shortest_distance;
      }

      //set current track to new position, print position
      curr_track=sequence[shortest_track];
      printf("%d ", curr_track);

    }else{
      // update total distance by current track (adds remaining distance before going back to 0)		
			// reset current track to 0 (loop back around)
			// set "end reached" flag to 1
      total_distance+=curr_track;
      curr_track=0;
      end_reached=1;
    }    
  }
 	// print total distance traversed
  printf("\nThe distance of the traversed tracks is: %d", total_distance);
  printf("\n");

	return;
}

void quit(){       
  if (sequence != NULL){
    free(sequence);
  }
  printf("\nQuitting program...\n\n");

  return;  
}

int main(){
  int selection;

  do {
    printf("\nDisk Scheduling\n");
    printf("-----------------\n");
    printf("1) Enter Parameters\n");
    printf("2) Calculate distance to traverse tracks using FIFO\n");
    printf("3) Calculate distance to traverse tracks using SSTF\n");
    printf("4) Calculate distance to traverse tracks using Scan\n");
    printf("5) Calculate distance to traverse tracks using C-Scan\n");
    printf("6) Quit program and free memory\n");

    printf("\nEnter selection: ");
    scanf("%d", &selection);

    switch (selection) {
      case 1:
        parameters();
        break;
      case 2:
        FIFO();
        break;
      case 3:
        SSTF();
        break;
      case 4:
        Scan();
        break;
      case 5:
        CScan();
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