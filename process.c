#include "headers.h"
/* Modify this file as needed*/
int premainingtime=10;  //any numberpositive for processremainingtime
int pwaitingtime=0;
int quantum=0;                         //for testing
int quantumcount=0;
bool quant=false;
                                                                  //process count blocking time
//int realquantum=0;    //process quantum if 0 yb2a mlha4 aslun
//bool isrunning=false;  //used to make process running or waiting
//bool firstread=true;   //used to read remainingtime first time;
//void running(int signum);
//void blocking(int signum);
int main(int agrc, char *argv[])
{
bool runturn=false;
bool waitturn=true; 
	int shid=shmget(getppid(),4,IPC_CREAT | 0644);
	int *shpid = (int *)shmat(shid, (void *)0, 0);
	(*shpid)=getpid();
    initClk();
	
int shmepid=shmget(getpid(),18,IPC_CREAT | 0644);	
struct processtimes* Ptimesptr =(struct processtimes*)shmat(shmepid,(void*)0,0);
while((*Ptimesptr).sender!=0||(*Ptimesptr).state!=0||(*Ptimesptr).remaining==0);  //first initialization of memory is equal to zero;
premainingtime=(*Ptimesptr).remaining;
quantum=(*Ptimesptr).quantum;
    //TODO The process needs to get the remaining time from somewhere
    //remainingtime = ??;
	int x=getClk();
	
    while (premainingtime > 0)
    {
		
		if((*Ptimesptr).state==1)
		{ 
	        if(runturn)
			{
				runturn=false;
				waitturn=true;
			(*Ptimesptr).waiting=pwaitingtime;
			(*Ptimesptr).remaining=premainingtime;
			(*Ptimesptr).sender=1;
			}
	         if(getClk()>x)
			{
				premainingtime--;
				quantumcount--;
				x=getClk();
				if(quantumcount<=0&&quant)
				{
				(*Ptimesptr).quantum=0;	
				quant=false;
				}
			}
			
		}	
		else
		{
			if(waitturn)
			{ 
			runturn=true;
			waitturn=false;
			(*Ptimesptr).waiting=pwaitingtime;
			(*Ptimesptr).remaining=premainingtime;
			if(quantumcount<=0)
			{
			(*Ptimesptr).quantum=quantum;
			quantumcount=quantum;
			}
			else
			{
				(*Ptimesptr).quantum=quantumcount;
			}
			(*Ptimesptr).sender=1;
			
			quant=true;
			}
			
			if(getClk()>x)
			{
				pwaitingtime++;
				x=getClk();
			}
			
		}
     
    }
	//kill(getppid(),SIGUSR1);
	//int shmepid=shmget(getpid(),14,IPC_CREAT | 0644);
	//struct processtimes* Ptimesptr =(struct processtimes*)shmat(shmepid,(void*)0,0);    //mmkn n7sn deh
	    while((*Ptimesptr).sender!=0);                                                               //bnrg3 daata lma bn5ls 34an n4lha fe pcb
	(*Ptimesptr).waiting=pwaitingtime;
	(*Ptimesptr).remaining=premainingtime; 
	(*Ptimesptr).sender=1;
	while((*Ptimesptr).sender!=0);
    destroyClk(false);
    return 0;
     
}
/*void running(int signum)
{
int shmepid=shmget(getpid(),14,IPC_CREAT | 0644);	
struct processtimes* Ptimesptr =(struct processtimes*)shmat(shmepid,(void*)0,0); 
	if(firstread)
	{ 
       firstread=false;
	    //bnreadremainingtime=runningtime first running only
	premainingtime=(*Ptimesptr).remaining;
	realquantum=(*Ptimesptr).quantum;
	}
	(*Ptimesptr).remaining=premainingtime;
	(*Ptimesptr).waiting=pwaitingtime;
	(*Ptimesptr).sender=1;
	isrunning=true;
}*/

/*void blocking(int signum)
{   
     int shmepid=shmget(getpid(),14,IPC_CREAT | 0644);
	 struct processtimes* Ptimesptr =(struct processtimes*)shmat(shmepid,(void*)0,0);    //mmkn n7sn deh
	 (*Ptimesptr).remaining=premainingtime;                                                                      //bnrg3 daata lma y3mle blocking 34an n4lha fe pcb
	 (*Ptimesptr).waiting=pwaitingtime;
	 (*Ptimesptr).sender=1;
	 isrunning=false;
	 while((*Ptimesptr).sender!=0); 
}*/
