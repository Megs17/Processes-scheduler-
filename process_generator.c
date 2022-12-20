#include "headers.h"
void clearResources(int);
int main(int argc, char *argv[])
{
    signal(SIGINT, clearResources);
    // TODO Initialization
    // 1. Read the input files.
    FILE *ptrf;
    ptrf=fopen(argv[1],"r");
    int pcount=0;                          //processes count
    char c;
    do{
    c=fgetc(ptrf);
    if(c=='\n')
    pcount++;
    } while(c!=EOF);
    fclose(ptrf);
    pcount--;      
           
struct processData *ptrd=malloc(pcount*sizeof(struct processData));  //array of proceesses dataa.
ptrf=fopen(argv[1],"r");
int datap;                                                  //carry processes dataa
int index=0;                                                    
fscanf(ptrf,"%*[^\n]\n");                                 //neglect first line of file 
while(fscanf(ptrf,"%d",&datap)==1)
{ 
   ptrd[index].id=datap;
   
   fscanf(ptrf,"%d",&datap);
   ptrd[index].arrivaltime=datap;
   
   fscanf(ptrf,"%d",&datap);
   ptrd[index].runningtime=datap;
   
   fscanf(ptrf,"%d",&datap);
   ptrd[index].priority=datap;   
index++; 
}
fclose(ptrf);
//read processes dataaa finished.
 
 
    // 2. Read the chosen scheduling algorithm and its parameters, if there are any from the argument list.
    //int sdoption=atoi(argv[3]);   //schedule option.
    //int quantum=0;
    //if(sdoption>2)
    //{
    //quantum=atoi(argv[5]);
    //} 
    // read chosen alogrtihm finished
    // 3. Initiate and create the scheduler and clock processes.
	
	//sending scheduleoption and quantum to scheduler before the start of clock
	struct schedulertype sch_type_obj;
	sch_type_obj.sct=1;
	sch_type_obj.qm=0;
	sch_type_obj.pn=pcount;   //processes number
	//sch_type_obj.processesnum=pcount;
	int shmeid=shmget(SHKEY2,12,IPC_CREAT | 0644);
	struct schedulertype* schtypeptr =(struct schedulertype*)shmat(shmeid,(void*)0,0);   
	(*schtypeptr)=sch_type_obj;
	int shmeid2=shmget(SHKEY3,16,IPC_CREAT | 0644);
	struct processData* processdata =(struct processData*)shmat(shmeid2,(void*)0,0); 
	(*processdata).arrivaltime=0;
	int shmeid3=shmget(getpid(),4,IPC_CREAT | 0644);
	struct isarriving* arriving=(struct isarriving*)shmat(shmeid3,(void*)0,0);
	(*arriving).sender=0;
	(*arriving).isarriving=0;
	//shared memory used to transfer processes 

    int clock=-1;
   
    clock=fork();
    if(clock==0)
    { 
    execl("/home/megz/Desktop/New Folder/code/scheduler.out","scheduler.out",NULL);

    }
	 int sc=-1;
	 int sendedprocesses=0;
	
	sc=fork();
	if(sc==0)
	{       
	  execl("/home/megz/Desktop/New Folder/code/clk.out","clk.out",NULL);
		    	
	}
	
     initClk();
	   while(getClk()!=0);                                   // 4. Use this function after creating the clock process to initialize clock.
   

      while(1)    // TODO Generation Main Loop
     {
          if(sendedprocesses<pcount)
		  {
			  if((*arriving).sender)
			  {
				  if(ptrd[sendedprocesses].arrivaltime<=getClk())
				  {
					(*arriving).isarriving=1;  
				  }
				  else
				  {
					  (*arriving).isarriving=0;
				  }
				  (*arriving).sender=0;
			  }
			
				  
			if(ptrd[sendedprocesses].arrivaltime<=getClk()&&(*processdata).arrivaltime==0)
			  {                                                                                   //sending proceesses with arrival time
				  (*processdata)=ptrd[sendedprocesses];   
				  sendedprocesses++;
				 
			  }
			  
			  
			
			  
		  }
		  else
		  {   
	           if((*arriving).sender)
			  {
				  (*arriving).isarriving=0;
				  (*arriving).sender=0; 
			  }
			  
		  }
		                                                          // To get time use this function. 

     }
	 //lazm ymot b3d scheduler 34an hwar (*arriving).sender y2ma yb3t hagaa t5leh me3ml4 check bta3 is arriving34an lw mrd4 3leh deadlock
     destroyClk(true);  
    // 5. Create a data structure for processes and provide it with its parameters.
    // 6. Send the information to the scheduler at the appropriate time.
    // 7. Clear clock resources
    
}

void clearResources(int signum)
{
    //TODO Clears all resources in case of interruption
    //kill(clock,SIGINT);
    //kill(sc,SIGINT);
    //exit(0);
    
}
