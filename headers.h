#include <stdio.h> //if you don't use scanf/printf change this include
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
typedef short bool;
#define true 1
#define false 0

#define SHKEY 300
#define SHKEY2 600   //used by processgenerator&schedulefor scheduleoption
#define SHKEY3 800 //used by processgenerator&scheduleforprocesses
///==============================
//process data used in process generator
struct processData
{
    int arrivaltime;
    int priority;
    int runningtime;
    int id;
};

//scheduletypeandquantum
struct schedulertype
{
	int sct;    //scheduletype
	int qm;    //quantum
	int pn;  
};

struct processtimes
{
	bool sender;          //0 for scheduler & 1 for process
	int remaining;
	int waiting;
	int quantum;     //if 0 so no quantum
    int state;        //(zero)waiting&(ONE)running&(TWO)BLOCKED&(THREE)DELETED
};
 struct node 
 {

	int data;
	int priority;
	struct node* next;
	
};

struct PCBDATA 
{
	 int state; //(zero)waiting&(ONE)running&(TWO)BLOCKED&(THREE)DELETED
     int runningtime;
	 int remaining;
	 int execution;                  //a7na fhmnah 2no lw2t 2le hea 24t8lto running-remaining
	 int waitingtime;
};
struct isarriving
{
	bool sender;
	bool isarriving;
};
//don't mess with this variable//
int *shmaddr; 
//
//===============================

int getClk()
{
    return *shmaddr;
}

/*
 * All processes call this function at the beginning to establish communication between them and the clock module.
 * Again, remember that the clock is only emulation!
*/
void initClk()
{
    int shmid = shmget(SHKEY, 4, 0444);
    while ((int)shmid == -1)
    {
        //Make sure that the clock exists
        printf("Wait! The clock not initialized yet!\n");
        sleep(1);
        shmid = shmget(SHKEY, 4, 0444);
    }
    shmaddr = (int *)shmat(shmid, (void *)0, 0);
}

/*
 * All processes call this function at the end to release the communication
 * resources between them and the clock module.
 * Again, Remember that the clock is only emulation!
 * Input: terminateAll: a flag to indicate whether that this is the end of simulation.
 *                      It terminates the whole system and releases resources.
*/

void destroyClk(bool terminateAll)
{
    shmdt(shmaddr);
    if (terminateAll)
    {
        killpg(getpgrp(), SIGINT);
    }
}
