#ifndef FUNCTIONS_H
#define FUNCTIONS_H

union semun{
    int val;                 //--- value for SETVAL ---//
    struct semid_ds *buf;    //--- buffer for IPC_STAT, IPC_SET ---//
    unsigned short *array;   //--- array for GETALL, SETALL ---//
};


int sem_init(int, int);
int sem_up( int );
int sem_down(int );
#endif


