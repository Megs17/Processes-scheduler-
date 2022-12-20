#include "headers.h"
struct node * newNode(int d,int p);
int peek(struct node ** head);
void push(struct node ** head ,int d,int p);
int isEmpty(struct node ** head);
void processended(int signum);
void deleteNode(struct node ** head,int id); //deletenode
void enqueue(struct node** front ,struct node** rear,int d,int p);
void dequeue(struct node** front ,struct node** rear);
struct node* headpq1=NULL;  //head priorityqueue1of forking ids
struct node* headpq2=NULL;  //head priorityqueue2of processesids
struct node* front1=NULL;        //frontofcircular  
struct node* rear1=NULL;         //rearofcircular
struct node* front2=NULL;        //frontofcircular  
struct node* rear2=NULL;         //rearofcircular
struct node* arrF1[11]={NULL}; //arrayoffront1pointers
struct node* arrR1[11]={NULL};//arrayofrear1pointers
struct node* arrF2[11]={NULL};//arrayoffront2pointers
struct node* arrR2[11]={NULL};//arrayofrear2pointers
int prunning=0;                           //7gat multifeedbackpriorityrunning  
int hprunning=0;                         //23lapriorityda5lt3nde
bool Hprocessonschedule=false; //Haveprocessrunningonschedule
int rprocessfid=-1;             //running process forking id
int rprocessrid=-1;            //running process real id 
struct PCBDATA *PCB;
int quantumcount=0;                                           
int main()
{
	
   int shmeid=shmget(SHKEY2,12,IPC_CREAT | 0644);
   struct schedulertype* schtypeptr =(struct schedulertype*)shmat(shmeid,(void*)0,0);                                                                                      //get type of schedule
     int schedulealgorithm= (*schtypeptr).sct;
     int quantum=(*schtypeptr).qm;
	 quantum=1;         ////////for testing only  
     int pn=(* schtypeptr).pn;                                                                                                  	 // ha5od hna algorithmof schedule 
	PCB=(struct PCBDATA *)malloc(pn*sizeof(struct PCBDATA)); 																													  // 3dd processes																															  //quantum used 
  int shmeid2=shmget(SHKEY3,16,IPC_CREAT | 0644);
  struct processData* processdata =(struct processData*)shmat(shmeid2,(void*)0,0);
  int shmeid3=shmget(getppid(),4,IPC_CREAT | 0644);
 struct isarriving* arriving=(struct isarriving*)shmat(shmeid3,(void*)0,0); 
 bool Hprocess=false;  //you getaprocess
 struct processData CPdata;  //carry process data
 struct processtimes* Ptimesptr=NULL;	//get address where processes data send to
   initClk();
//sjf
/*while(1)
{
 
 if((* processdata).arrivaltime!=0 )
 {     
	 CPdata=(*processdata);                                              //get processess data with arrival time first thing is done in the loop
	 (*processdata).arrivaltime=0;
	 Hprocess=true;
 }
 if(Hprocess)
 {    
	int idfork=fork();                                                                                //lw feh process gat  b3mlha fork 
      if(idfork==0)
	  {      
		      execl("/home/megz/Desktop/New Folder/code/process.out","process.out",NULL);
			  
	  }		 
	 int shid=shmget(getpid(),4,IPC_CREAT | 0644);
	 int *shpid = (int *)shmat(shid, (void *)0, 0);
	 while((*shpid)!=idfork);        
                                  	 //b3ml wait 34an execl bte2aa bstnaa l7d mtt3ml w t7ot idbta3ha
	  struct processtimes obj;                                           //struct used to share data between scheduler and process (first read process takes runningtime=remainingtime && quantum if no put it equal zero && waiting time return from process)
	  obj.remaining=CPdata.runningtime;                       //i will add another parameter to distinguish between scheduler and process in sending data to check synchronization of time (0)scheduler (1)process 
	  
	  obj.waiting=0;
	  obj.quantum=0;
	  obj.state=0;
	  obj.sender=0;
	int shmepid=shmget(idfork,18,IPC_CREAT | 0644);
	struct processtimes* Ptimesptr2 =(struct processtimes*)shmat(shmepid,(void*)0,0);                           //putting processtimesdata 
	(*Ptimesptr2)=obj;                                                                                                                             //put runningtime toread it when start;
	  push(&headpq1,idfork,CPdata.runningtime);                 //put forkingid in queue bst5dmo fe sharedmemory w signal sending(runningorblocking)
	  push(&headpq2,CPdata.id,CPdata.runningtime);    //put real id in queue bst5dmo fe pcb array 34an 24el data bta3t process 	 
	  PCB[CPdata.id-1].state=0;
      PCB[CPdata.id-1].runningtime=CPdata.runningtime;
      PCB[CPdata.id-1].remaining=CPdata.runningtime;
      PCB[CPdata.id-1].execution=0;
      PCB[CPdata.id-1].waitingtime=0;	    
	  Hprocess=false;
   	  //pcb
    while((*Ptimesptr2).sender!=1);

	(*Ptimesptr2).sender=0;
 }                                                                                            
 
 if(!Hprocessonschedule&&!isEmpty(&headpq1))     // lma bdlt conditions zabt 2le bet8eer fe signal yt7t 2awl (Hprocessonschedule)                                                                                
 {         	                                               //here send signal to processes to run b4of lw queueme4 fadya & w mfe4 7ad mst5dm scheduler
	 Hprocessonschedule=true;                                    //b2ol 2n feh process schedule wa5dha
	 rprocessfid=peek(&headpq1);                                // bgeeb 23la wa7da fe peek bta3 queue
	 rprocessrid=peek(&headpq2);     
	 //if state =0 then first run state=2 yb2a blocked w hna ht3ml resume me4 htst5dmfesjf  
	 int shmepid=shmget(rprocessfid,18,IPC_CREAT | 0644);
	Ptimesptr =(struct processtimes*)shmat(shmepid,(void*)0,0);  
	 printf( " id of processrunning %d \n",rprocessrid);
      (*Ptimesptr).state=1;
     while((*Ptimesptr).sender!=1);  //b3ml delay l7d mt2kd 2no 7t data
	 PCB[rprocessrid-1].state=1;
	 PCB[rprocessrid-1].waitingtime=(*Ptimesptr).waiting;
	 PCB[rprocessrid-1].remaining=(*Ptimesptr).remaining;
	 (*Ptimesptr).sender=0;         //brg3ha zero 2kd 2ne 2stlmt 34an check 2le b3de 
 }
  if((Ptimesptr)!=NULL)
  {
	  if((*Ptimesptr).remaining==0)
	{	
      Hprocessonschedule=false;
     deleteNode(&headpq1,rprocessfid);
	deleteNode(&headpq2,rprocessrid);
	while((*Ptimesptr).sender!=1); 
	printf(" process id %d finished \n",rprocessrid);
	printf(" process waiting time %d \n",(*Ptimesptr).waiting);
	printf(" clk: %d\n",getClk());	
	 (*Ptimesptr).sender=0;
	 PCB[rprocessrid-1].state=3;                   //process state deleted.
		Ptimesptr=NULL;
	}

}
}*/
//code HPF
while(1)
{
 
 if((* processdata).arrivaltime!=0 )
 {     
	 CPdata=(*processdata);                                              //get processess data with arrival time first thing is done in the loop
	 (*processdata).arrivaltime=0;
	 Hprocess=true;
 }
 if(Hprocess)
 {    
	int idfork=fork();                                                                                //lw feh process gat  b3mlha fork 
      if(idfork==0)
	  {      
		      execl("/home/megz/Desktop/New Folder/code/process.out","process.out",NULL);
			  
	  }		 
	 int shid=shmget(getpid(),4,IPC_CREAT | 0644);
	 int *shpid = (int *)shmat(shid, (void *)0, 0);
	 while((*shpid)!=idfork);                                     	                                                    //b3ml wait 34an execl bte2aa bstnaa l7d mtt3ml w t7ot idbta3ha
	  struct processtimes obj;                                           //struct used to share data between scheduler and process (first read process takes runningtime=remainingtime && quantum if no put it equal zero && waiting time return from process)
	  obj.remaining=CPdata.runningtime;                       //i will add another parameter to distinguish between scheduler and process in sending data to check synchronization of time (0)scheduler (1)process 
	  obj.waiting=0;
	  obj.quantum=0;
	  obj.state=0;
	  obj.sender=0;
	int shmepid=shmget(idfork,18,IPC_CREAT | 0644);
	struct processtimes* Ptimesptr2 =(struct processtimes*)shmat(shmepid,(void*)0,0);                           //putting processtimesdata 
	  (*Ptimesptr2)=obj;                                                                                                                             //put runningtime toread it when start;
	  push(&headpq1,idfork,CPdata.priority);                 //put forkingid in queue bst5dmo fe sharedmemory w signal sending(runningorblocking)
	  push(&headpq2,CPdata.id,CPdata.priority);    //put real id in queue bst5dmo fe pcb array 34an 24el data bta3t process 	 
	  PCB[CPdata.id-1].state=0;
      PCB[CPdata.id-1].runningtime=CPdata.runningtime;
      PCB[CPdata.id-1].remaining=CPdata.runningtime;
      PCB[CPdata.id-1].execution=0;
      PCB[CPdata.id-1].waitingtime=0;	    
	  Hprocess=false;
   	  //pcb
    while((*Ptimesptr2).sender!=1);
	(*Ptimesptr2).sender=0;
 }                                                                                            
 
 if(!Hprocessonschedule&&!isEmpty(&headpq1))     // lma bdlt conditions zabt 2le bet8eer fe signal yt7t 2awl (Hprocessonschedule)                                                                                
 {         	                                               //here send signal to processes to run b4of lw queueme4 fadya & w mfe4 7ad mst5dm scheduler
	 Hprocessonschedule=true;                                    //b2ol 2n feh process schedule wa5dha
	 rprocessfid=peek(&headpq1);                                // bgeeb 23la wa7da fe peek bta3 queue
	 rprocessrid=peek(&headpq2);     
	 //if state =0 then first run state=2 yb2a blocked w hna ht3ml resume me4 htst5dmfesjf  
	if(PCB[rprocessrid-1].state==0)
	 printf( " id of processrunning %d \n",rprocessrid);
 else
	 printf( " id of processresumed %d \n",rprocessrid);
       int shmepid=shmget(rprocessfid,18,IPC_CREAT | 0644);
	Ptimesptr =(struct processtimes*)shmat(shmepid,(void*)0,0);
	(*Ptimesptr).state=1;
     while((*Ptimesptr).sender!=1);  //b3ml delay l7d mt2kd 2no 7t data
	 PCB[rprocessrid-1].state=1;
	 PCB[rprocessrid-1].waitingtime=(*Ptimesptr).waiting;
	 PCB[rprocessrid-1].remaining=(*Ptimesptr).remaining;
	 (*Ptimesptr).sender=0;         //brg3ha zero 2kd 2ne 2stlmt 34an check 2le b3de 
 }
 if(rprocessfid!=peek(&headpq1)&&Hprocessonschedule)
 {  
    (*Ptimesptr).state=2;  
    while((*Ptimesptr).sender!=1);                                          //processblocked
	 if((*Ptimesptr).remaining>0)
	 {
		(*Ptimesptr).sender=0;
		PCB[rprocessrid-1].waitingtime=(*Ptimesptr).waiting;
		PCB[rprocessrid-1].remaining=(*Ptimesptr).remaining;
		 PCB[rprocessrid-1].state=2;
		 Hprocessonschedule=false;                            
		printf("id process blocked %d \n",rprocessrid);
	 }
	 else
	 {
		 (*Ptimesptr).state=1;        //hwa kan 5ls
	 }
 }
  if((Ptimesptr)!=NULL)
  {
	  if((*Ptimesptr).remaining==0)
	{	
      Hprocessonschedule=false;
     deleteNode(&headpq1,rprocessfid);
	deleteNode(&headpq2,rprocessrid);
	while((*Ptimesptr).sender!=1); 
	printf(" process id %d finished \n",rprocessrid);
	printf(" process waiting time %d \n",(*Ptimesptr).waiting);
	printf(" clk: %d\n",getClk());	
	 (*Ptimesptr).sender=0;
	 PCB[rprocessrid-1].state=3;                   //process state deleted.
		Ptimesptr=NULL;
	}

}

}
//roundrobin
/*while(1)
{
 if((*processdata).arrivaltime!=0)
 {     
	 CPdata=(*processdata);                                              //get processess data with arrival time first thing is done in the loop
	 (*processdata).arrivaltime=0;
	 Hprocess=true;
 }
 if(Hprocess)
 {    
	int idfork=fork();                                                                                //lw feh process gat  b3mlha fork 
      if(idfork==0)
	  {      
		      execl("/home/megz/Desktop/New Folder/code/process.out","process.out",NULL);
			  
	  }		 
	 int shid=shmget(getpid(),4,IPC_CREAT | 0644);
	 int *shpid = (int *)shmat(shid, (void *)0, 0);
	 while((*shpid)!=idfork);                                     	                                                    //b3ml wait 34an execl bte2aa bstnaa l7d mtt3ml w t7ot idbta3ha
	  struct processtimes obj;                                           //struct used to share data between scheduler and process (first read process takes runningtime=remainingtime && quantum if no put it equal zero && waiting time return from process)
	  obj.remaining=CPdata.runningtime;                       //i will add another parameter to distinguish between scheduler and process in sending data to check synchronization of time (0)scheduler (1)process 
	  obj.waiting=0;
	  obj.quantum=quantum;       
	  obj.state=0;
	  obj.sender=0;
	int shmepid=shmget(idfork,18,IPC_CREAT | 0644);
	struct processtimes* Ptimesptr2 =(struct processtimes*)shmat(shmepid,(void*)0,0);                           //putting processtimesdata 
	  (*Ptimesptr2)=obj;                                                                                                                             //put runningtime toread it when start;
	  enqueue(&front1,&rear1,idfork,CPdata.priority);                 //put forkingid in queue bst5dmo fe sharedmemory w signal sending(runningorblocking)
	  enqueue(&front2,&rear2,CPdata.id,CPdata.priority);          //put real id in queue bst5dmo fe pcb array 34an 24el data bta3t process 	 
	  PCB[CPdata.id-1].state=0;
      PCB[CPdata.id-1].runningtime=CPdata.runningtime;
      PCB[CPdata.id-1].remaining=CPdata.runningtime;
      PCB[CPdata.id-1].execution=0;
      PCB[CPdata.id-1].waitingtime=0;	    
	  Hprocess=false;
   	  //pcb
    while((*Ptimesptr2).sender!=1);
	(*Ptimesptr2).sender=0;
 }                                                                                            
 
 if(!Hprocessonschedule&&!isEmpty(&front1))     // lma bdlt conditions zabt 2le bet8eer fe signal yt7t 2awl (Hprocessonschedule)                                                                                
 {         	                                               //here send signal to processes to run b4of lw queueme4 fadya & w mfe4 7ad mst5dm scheduler
	 Hprocessonschedule=true;                                    //b2ol 2n feh process schedule wa5dha
	 rprocessfid=peek(&front1);                                // bgeeb 23la wa7da fe peek bta3 queue
	 rprocessrid=peek(&front2);     
	 //if state =0 then first run state=2 yb2a blocked w hna ht3ml resume me4 htst5dm fe sjf  
	if(PCB[rprocessrid-1].state==0)
	 printf( " id of processrunning %d \n",rprocessrid);
 else
	 printf( " id of processresumed %d \n",rprocessrid);
       int shmepid=shmget(rprocessfid,18,IPC_CREAT | 0644);
	Ptimesptr =(struct processtimes*)shmat(shmepid,(void*)0,0);
	(*Ptimesptr).state=1;
     while((*Ptimesptr).sender!=1);  
	 quantumcount=getClk()+quantum;
	//b3ml delay l7d mt2kd 2no 7t data
	 PCB[rprocessrid-1].state=1;
	 PCB[rprocessrid-1].waitingtime=(*Ptimesptr).waiting;
	 PCB[rprocessrid-1].remaining=(*Ptimesptr).remaining;
	 (*Ptimesptr).sender=0;   
 	 //brg3ha zero 2kd 2ne 2stlmt 34an check 2le b3de 
 }
 if(quantumcount<=getClk()&&Hprocessonschedule)        
 {  bool arrivingflag=false;
(*arriving).sender=1;
while((*arriving).sender==1);
if((*arriving).isarriving||(*processdata).arrivaltime!=0)
{
	arrivingflag=true;
}

    if(!arrivingflag)                                                  //(*processdata).arrivaltime==0 m3nah mfe4 haga gdedaa gat
	 { 
	  if(front1!=rear1&&(*Ptimesptr).quantum==0)                           //front1!=rear1 3aiz 23rf feh kam element 2ktr mn wa7d wla wa7d bs
	  { 
	  (*Ptimesptr).state=2;  
      while((*Ptimesptr).sender!=1);                                          //processblocked
	  if((*Ptimesptr).remaining>0)
	 {
		(*Ptimesptr).sender=0;
		PCB[rprocessrid-1].waitingtime=(*Ptimesptr).waiting;
		PCB[rprocessrid-1].remaining=(*Ptimesptr).remaining;
		PCB[rprocessrid-1].state=2;
		Hprocessonschedule=false;                            
		printf("id process blocked %d \n",rprocessrid);
		front1=front1->next;
		rear1=rear1->next;
		front2=front2->next;
		rear2=rear2->next;
	 }
	 else
	 {
		 (*Ptimesptr).state=1;        //hwa kan 5ls
	 }
	
	}
	 else
	 {
		 if(front1==rear1&&(*Ptimesptr).quantum==0)
		 {
			 quantumcount+=quantum;              //lw mfe4 8erha hzwdlhaa quantum tanii
		 }
		
	 }
	 }
 }
  if((Ptimesptr)!=NULL)
  {
	  if((*Ptimesptr).remaining==0)
	{	
      Hprocessonschedule=false;
     dequeue(&front1,&rear1);
	dequeue(&front2,&rear2);
	while((*Ptimesptr).sender!=1); 
	printf(" process id %d finished \n",rprocessrid);
	printf(" process waiting time %d \n",(*Ptimesptr).waiting);
	printf(" clk: %d\n",getClk());	
	 (*Ptimesptr).sender=0;
	 PCB[rprocessrid-1].state=3;                   //process state deleted.
		Ptimesptr=NULL;
	}

}

}*/
//////////////////////////////////////////////////////////////////////////////////////////////mfback
/*while(1)
{
 if((*processdata).arrivaltime!=0)
 {     
	 CPdata=(*processdata);                                              //get processess data with arrival time first thing is done in the loop
	 (*processdata).arrivaltime=0;
	 Hprocess=true;
 }
 if(Hprocess)
 {    
	int idfork=fork();                                                                                //lw feh process gat  b3mlha fork 
      if(idfork==0)
	  {      
		      execl("/home/megz/Desktop/New Folder/code/process.out","process.out",NULL);
			  
	  }		 
	 int shid=shmget(getpid(),4,IPC_CREAT | 0644);
	 int *shpid = (int *)shmat(shid, (void *)0, 0);
	 while((*shpid)!=idfork);                                     	                                                    //b3ml wait 34an execl bte2aa bstnaa l7d mtt3ml w t7ot idbta3ha
	  struct processtimes obj;                                           //struct used to share data between scheduler and process (first read process takes runningtime=remainingtime && quantum if no put it equal zero && waiting time return from process)
	  obj.remaining=CPdata.runningtime;                       //i will add another parameter to distinguish between scheduler and process in sending data to check synchronization of time (0)scheduler (1)process 
	  obj.waiting=0;
	  obj.quantum=quantum;       
	  obj.state=0;
	  obj.sender=0;
	int shmepid=shmget(idfork,18,IPC_CREAT | 0644);
	struct processtimes* Ptimesptr2 =(struct processtimes*)shmat(shmepid,(void*)0,0);                           //putting processtimesdata 
	  (*Ptimesptr2)=obj;                                                                                                                             //put runningtime toread it when start;
	  enqueue(&arrF1[CPdata.priority],&arrR1[CPdata.priority],idfork,CPdata.priority);                 //put forkingid in queue bst5dmo fe sharedmemory w signal sending(runningorblocking)
	  enqueue(&arrF2[CPdata.priority],&arrR2[CPdata.priority],CPdata.id,CPdata.priority);          //put real id in queue bst5dmo fe pcb array 34an 24el data bta3t process 	 
	  PCB[CPdata.id-1].state=0;
      PCB[CPdata.id-1].runningtime=CPdata.runningtime;
      PCB[CPdata.id-1].remaining=CPdata.runningtime;
      PCB[CPdata.id-1].execution=0;
      PCB[CPdata.id-1].waitingtime=0;	    
	  Hprocess=false;
   	  //pcb
    while((*Ptimesptr2).sender!=1);
	(*Ptimesptr2).sender=0;
	if(CPdata.priority<hprunning)
	{
	hprunning=CPdata.priority;	
	}
	else
	{
		hprunning=prunning;
	}
	
 }                                                                                            
 
 if(!Hprocessonschedule)                                                                                     
 {         	                                               //here send signal to processes to run b4of lw queueme4 fadya & w mfe4 7ad mst5dm scheduler
	 bool notempty=false;
	
	 for(int i=0;i<11;i++)
	 {
		 if(!isEmpty(&arrF1[i]))
		 {
			 prunning=i;                       //indexof ruuning priority
			 hprunning=i;
			 notempty=true;
			 break;
		 }	 
	 }
	  
	 if(notempty)
	 {		 
	 Hprocessonschedule=true;                                    //b2ol 2n feh process schedule wa5dha
	 rprocessfid=peek(&arrF1[prunning]);                                // bgeeb 23la wa7da fe peek bta3 queue
	 rprocessrid=peek(&arrF2[prunning]);     
	  int shmepid=shmget(rprocessfid,18,IPC_CREAT | 0644);
	Ptimesptr =(struct processtimes*)shmat(shmepid,(void*)0,0);
	(*Ptimesptr).state=1;
	while((*Ptimesptr).sender!=1);  
	 //if state =0 then first run state=2 yb2a blocked w hna ht3ml resume me4 htst5dm fe sjf  
	if(PCB[rprocessrid-1].state==0)
	{	
printf( " id of processrunning %d \n",rprocessrid);
quantumcount=getClk()+quantum;
	}
 else
 {	
printf( " id of processresumed %d \n",rprocessrid);
quantumcount=getClk()+(*Ptimesptr).quantum;
 }   
     
	//b3ml delay l7d mt2kd 2no 7t data
	 PCB[rprocessrid-1].state=1;
	 PCB[rprocessrid-1].waitingtime=(*Ptimesptr).waiting;
	 PCB[rprocessrid-1].remaining=(*Ptimesptr).remaining;
	 (*Ptimesptr).sender=0;   
	 } 
 //brg3ha zero 2kd 2ne 2stlmt 34an check 2le b3de 
 }
 if(quantumcount<=getClk()&&Hprocessonschedule)        
 { 
bool arrivingflag=false;
(*arriving).sender=1;
while((*arriving).sender==1);
if((*arriving).isarriving||(*processdata).arrivaltime!=0)
{
	arrivingflag=true;
}

    if(!arrivingflag)                                                  //(*processdata).arrivaltime==0 m3nah mfe4 haga gdedaa gat
	 { 
	  if((*Ptimesptr).quantum==0)                           //front1!=rear1 3aiz 23rf feh kam element 2ktr mn wa7d wla wa7d bs
	  {
		  if(arrF1[prunning]==arrR1[prunning]&&prunning==10)
		 {
			 quantumcount+=quantum;
		 }
else
{	
	  (*Ptimesptr).state=2;  
      while((*Ptimesptr).sender!=1);                                          //processblocked
	  if((*Ptimesptr).remaining>0)
	 { 
		(*Ptimesptr).sender=0;
		PCB[rprocessrid-1].waitingtime=(*Ptimesptr).waiting;
		PCB[rprocessrid-1].remaining=(*Ptimesptr).remaining;
		PCB[rprocessrid-1].state=2;
		Hprocessonschedule=false;                            
		printf("id process blocked %d \n",rprocessrid);
		if(prunning==10)
		{arrF1[prunning]=arrF1[prunning]->next;
		arrR1[prunning]=arrR1[prunning]->next;
		arrF2[prunning]=arrF2[prunning]->next;
		arrR2[prunning]=arrR2[prunning]->next;
		}
		else
		{
		 dequeue(&arrF1[prunning],&arrR1[prunning]);
	     dequeue(&arrF2[prunning],&arrR2[prunning]);	
		 enqueue(&arrF1[prunning+1],&arrR1[prunning+1],rprocessfid,prunning+1);       
		 enqueue(&arrF2[prunning+1],&arrR2[prunning+1],rprocessrid,prunning+1);
		 }
	 }

	  
	  else
	 {
		 (*Ptimesptr).state=1;        //hwa kan 5ls
	 }
}	  
	  }
	}
	 
	 }

  if((Ptimesptr)!=NULL)
  {
	  if((*Ptimesptr).remaining==0)
	{	
      Hprocessonschedule=false;
    dequeue(&arrF1[prunning],&arrR1[prunning]);
	    dequeue(&arrF2[prunning],&arrR2[prunning]);	
	while((*Ptimesptr).sender!=1); 
	printf(" process id %d finished \n",rprocessrid);
	printf(" process waiting time %d \n",(*Ptimesptr).waiting);
	printf(" clk: %d\n",getClk());	
	 (*Ptimesptr).sender=0;
	 PCB[rprocessrid-1].state=3;                   //process state deleted.
		Ptimesptr=NULL;
	}

}
}*/
}
//data structure used in scheduler 
//priority_queue
// Node
//priority_queue functions
struct node * newNode(int d,int p)
{
	struct node* temp=(struct node *)malloc(sizeof(struct node));
	temp->data=d;
	temp->priority=p;
	temp->next=NULL;
	return temp;
}

int peek(struct node ** head)
{
	if((*head)==NULL)
	{  
		return rprocessfid;
	}
	
	return (*head)->data;
}
int peekp(struct node ** head)
{
	if((*head)==NULL)
	{  
		return rprocessfid;
	}
	
	return (*head)->priority;
}
void pop(struct node ** head)
{
	struct node * temp=*head;
	(*head)=(*head)->next;
	free(temp);
}
void push(struct node ** head ,int d,int p)
{
if((*head)==NULL)
{
	*head=newNode(d,p);
	return;
}	
	struct node * start=(*head);
	struct node * temp=newNode(d,p);
	if((*head)->priority>p)
	{
		temp->next=*head;
		(*head)=temp;
		
	}
	else
	{
		while(start->next!=NULL&&(start->next->priority) <= p)
		{
			start=start->next;
		}
	
	temp->next=start->next;
	start->next=temp;
	}
}
int isEmpty(struct node ** head)
{
	return (*head)==NULL;
}
void deleteNode(struct node ** head,int id)
{
	struct node* temp;
	if((*head)->data==id)
	{
	 temp=*head;
	 *head=(*head)->next;
	 free(temp);
	}
	else
	{
		struct node* current=*head;
		while(current->next!=NULL)
		{
			if(current->next->data==id)
			{
				temp=current->next;
				current->next=current->next->next;
				free(temp);
				break;
			}
			else
			{
				current=current->next;
			}
		}
		
	}

	
}
void enqueue(struct node** front ,struct node** rear,int d,int p)
{
	struct node*temp=newNode(d,p);
	temp->next=NULL;
	if((*front)==NULL)
	{
		(*front)=(*rear)=temp;
	}
	else
	{
		(*rear)->next=temp;
		(*rear)=temp;
	}
	(*rear)->next=(*front);
}
void dequeue(struct node** front ,struct node** rear)
{
	struct node* temp=*front;
	if(((*front)==NULL)&&((*rear)==NULL))
	{
		return;
	}
	else if((*front)==(*rear))
	{
		(*front)=(*rear)=NULL;
		free(temp);
	}
	else
	{
		(*front)=(*front)->next;
		(*rear)->next=(*front);
		free(temp);
	}
}

