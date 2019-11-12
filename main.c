//----------------Alexis Chaidos-------------------//
//---------------A.M.:1115201300197----------------//
//------------------------------------------------//
//---------------Feeder.c------------------------//

#include "headers.h"
#include "functions.h"


//---------Function for feeder job-------------//
//---------------------------------------------//
void feeder_job(int m,int sem_c,int sem_f,shared_memory *shm_adr,int n){
		struct timeval begin;  //--gia xrisi tou gettimeofday()--//

		int *array;

		int i,j;

		array = malloc((m)*sizeof(int));

		for(int i=0; i<m; i++){   //--pernaw m tyxaious akeraious ston array--//

        	array[i]=rand();
        	printf("Element %d from the feeder is  %d\n",i+1,array[i]);
		}

				for( i=0; i<m; i++){

					for( j=0; j<n; j++){
            			sem_down(sem_f); //--so to block the feeder process--//
            		}

					shm_adr->value = array[i];
					//printf("father value = %d\n",shm_adr->value);

					gettimeofday(&begin, NULL);
					shm_adr->stamp = (begin.tv_sec)+((begin.tv_usec)/1000000.0);
					
					
            		for(int j=0; j<n; j++){
            			sem_up(sem_c); //--so to unblock the child processes--//
            		}
            	}
        free(array);    	

}

//------------------- producer job--------------------//
//-----------------------------------------------------//
void child_job(int m,int sem_c,int sem_f,shared_memory *shm_adr,int last){
	struct timeval end;
	double total;      //--Gia tin apothikeusi tis synolikis kathysterisis--//
	int *child_array;
	int i;

	child_array = malloc(sizeof(int)*m);	

	for( i=0; i<m; i++){
		sem_down(sem_c);   //--so to block the child processes--//

		child_array[i] = shm_adr->value;
		//printf("child value = %d\n",child_array[i]);   //--an thelw kathe diergasia na ektypwnei to value amesws molis to lavei apo to shared memory--//

		gettimeofday(&end, NULL);

		total = (((end.tv_sec)+((end.tv_usec)/1000000.0)) - shm_adr->stamp) ;

		sem_up(sem_f);  //--so to unblock the feeder process--//
	}

	if(last){
		printf("\n");
		printf("Print the array from the last consumer\n");
		printf("\n");
		for( i=0; i<m; i++){

			printf("Element %d from the last process is %d\n",i+1,child_array[i]);
		}
		printf("My pid = %d\n",getpid());
		printf("Total Delay for the last consumer is %f seconds \n",total);
	}

	free(child_array);
}

//------------Function main----------------------//
//----------------------------------------------//
int main(int argc,char *argv[]){

	int i,j;
	pid_t pid;
	int shm_id,sem_p_id,sem_c_id;
	shared_memory  *shm_adr;    //-------Pointer to a shared memory----////


	if(argc > 3){
		printf("Too many arguments \n");
		return -1;
	}

	if(argc < 3){
		printf("Too few arguments\n");
		return -1;
	}

	int m = atoi(argv[1]);
	if(m>3000){
		printf(" Number of Integers = %d\n",m);
	}
	else{
		printf(" 1st argument must be bigger than 3000 -> Please try again\n");
		return 0;
	} 

	int n = atoi(argv[2]);
	printf(" Number of Processes = %d\n",n);

	

	//------Shared-Memory Creation---------------//
	//-------------------------------------------//	

	shm_id = shmget(IPC_PRIVATE,sizeof(shared_memory),IPC_CREAT | 0660);

	if( shm_id == -1){
		shmctl(shm_id, IPC_RMID, 0);               //-----Remove a shared memory segment-------//
		perror("Shared Memory Error ");
		return -1;
	}

    shm_adr = (shared_memory *)shmat( shm_id, NULL, 0);


    if (shm_adr == (shared_memory*) -1) {
    	shmctl(shm_id, IPC_RMID, 0);              //-----Remove a shared memory segment-------////---IPC_RMID:gia leitourgeia diagrafis----//
       	perror("Shared memory segment attachment error ");  
		return -1;
	}

	//-----------Semaphores Creation----------------//
	//-----------2 counting-semaphores--------//

	sem_p_id = semget(IPC_PRIVATE, 1, 0660|IPC_CREAT);
	if(sem_p_id==-1){
		perror("Semaphore's creation error");
		semctl(sem_p_id,0,IPC_RMID);
		return -1;
	}

	if (sem_init(sem_p_id, n) == -1) {      //---- initialize semaphore to n(number of processes) ----//
        perror("Initialization error");     
        semctl(sem_p_id, 0, IPC_RMID);
        return -1;
    }


    sem_c_id = semget(IPC_PRIVATE,1,0660|IPC_CREAT);
    if(sem_c_id==-1){
		perror("Semaphore's creation error");
		semctl(sem_c_id,0,IPC_RMID);
		return -1;
	}

	if (sem_init(sem_c_id, 0) == -1) {      //---- initialize semaphore to 0 ----//
        perror("Initialization error");
        semctl(sem_c_id, 0, IPC_RMID);
        return -1;
    } 


	//-------Processes Generator------------------//
	//--------------------------------------------//

	for(j = 0; j < n; j++) {

    	pid = fork();

    	if(pid < 0) {
        	perror("Forking Error");
        	exit(1);
    	} 
    	else if (pid == 0) {                     //--vriskomai stin diergasia paidi--//
    		child_job(m,sem_c_id,sem_p_id,shm_adr,j==n-1);
    		//--to teleutaio orisma tis child_job xrisimopoieitai gia na mporesw na katalavw pote vriskomai-
    		//--stin teleutaia diergasia--//
    		break;
    	} 

	}


	if(pid != 0){                   //--vriskomai stin diergasia tou patera--//

            feeder_job(m,sem_c_id,sem_p_id,shm_adr,n);

            for(j=0; j<n; j++){
            	wait(NULL);
            }
            printf("This programm  has finished \n "); 

			//---------Shared-Memory Deletion----//
			//--------------------------------//		
			shmdt((int*)shm_adr);
    		if (shmctl(shm_id, IPC_RMID, (struct shmid_ds *)0) == -1) {
        		perror("Shared memory deletion error");
        		return -1;
    		}

    		//--Semaphores Deletion----//
    		//------------------------//
    		if (semctl(sem_p_id, 0, IPC_RMID) < 0) {
        		perror("Semaphore deletion error");
        		return -1;
    		}

    		if (semctl(sem_c_id, 0, IPC_RMID) < 0) {
        		perror("Semaphore deletion error");
        		return -1;
    		}
    }

	
	return 0;
}





