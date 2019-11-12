#ifndef HEADER_H
#define HEADER_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <ctype.h>
#include <sys/types.h>
#include "functions.h"



typedef struct 
{	
	int value;
	double stamp;
}shared_memory;



#endif
