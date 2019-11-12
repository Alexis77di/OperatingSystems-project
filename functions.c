#include "headers.h"
#include "functions.h"

//-----Initialization semaphore---//
//-------------------------------//
int sem_init(int sem_id,int val){
	union semun x;
	int check;
	x.val = val;
	check = semctl(sem_id, 0, SETVAL, x);

	if ( check == -1 ) {
		perror("Initialization error");
		return -1;
	}
	else {
		return 0;
	}
}

//-- Αuxisi semaphore --- Praxi apodesmeusis --//
//------------------------------------------------//

int sem_up( int sem_id ){

	struct sembuf sem_b;
	int check;
	sem_b.sem_num = 0;    //-- kathorizei se poion semaphore tha ginei h praxi --//
	sem_b.sem_op = 1;     
	sem_b.sem_flg = 0;
	check = semop(sem_id, &sem_b, 1);
	if ( check == -1 ) {
		perror("Up function error");
		return -1;
	}
	else{ 
		return 0;
	}	
}

//-- Meiwsi semaphore --- Praxi desmeusis --//
//----------------------------------------------//

int sem_down(int sem_id){
	
	struct sembuf sem_b;
	int check;
	sem_b.sem_num = 0;      //-- kathorizei se poion semaphore tha ginei h praxi --//
	sem_b.sem_op = -1;		
	sem_b.sem_flg = 0;
	check = semop(sem_id, &sem_b, 1);
	if(check == -1){
		perror("Down function error");
		return -1;
	}
	else{
		return 0;
	}

}


