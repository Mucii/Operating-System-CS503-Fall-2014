/* lab2tests.c - start_tests */

#include <xinu.h>

/*------------------------------------------------------------------------
 * start_tests  -  Lab 2 implementation
 *------------------------------------------------------------------------
 */
int time;
uint32 i;
uint32 j;
pid32 sema_prc[10];
pid32 interrupt_prc[10];
pid32 spinlock_prc[10];
pid32 nodelay_prc[10];
sid32 sema;
uint32 starttime,endtime;

//*****
//
//Interrupt
//
void report_n(int time, uint32 result){
	printf("Exclusive access method non-exclusive: delay of %d usecs, %d\n",time,result);
}
void report_i(int time, uint32 result){
	printf("Exclusive access method disable/restore: delay of %d usecs, %d\n",time,result);
}

void report_s(int time, uint32 result){
	printf("Exclusive access method semaphore: delay of %d usecs, %d\n",time,result);
}
void report_l(int time, uint32 result){
	printf("Exclusive access method spin lock: delay of %d usecs, %d\n",time,result);
}
void interrupt1( int time, sid32 sema_in){
	wait(sema_in);
	while(1){	
		intmask mask;
		mask = disable();
		i++;
		delayus(time);
		restore(mask);
	}
}
float run_interrupt( int time){
	int n,n1,n2;
	i = 0;
	sid32 sema_total;
	sema_total = semcreate(0);
	for(n=0; n<10;n++){	
	interrupt_prc[n] = create(interrupt1, 1024, 20, "interrupt",2 ,time, sema_total);
	}
	for(n1=0; n1<10;n1++){	
	resume(interrupt_prc[n1]); 
	}

	starttime = getextime();
	
	signaln(sema_total,11);//This start all the processes into the ready list;
	//sleep
	
	//sleep(30);
	sleep(30);
	//kill all the process and semaphore
	for(n2= 0; n2<10;n2++){	
		kill(interrupt_prc[n2]);
	}
	semdelete(sema_total);

	endtime = getextime();

	uint32 result;
	uint32 time_dif; 
	time_dif = endtime-starttime;
	result = i/time_dif;
	return result;
}

//*****
//
//Semaphore
//
//****
void semaphore( int time, sid32 sema_in){
	wait(sema_in);
	while(1){	
		wait(sema);
		i++;
		delayus(time);
		signal(sema);
	}
}
float run_sema( int time){
	int n,n1,n2;
	i = 0;
	sid32 sema_total;
	sema_total = semcreate(0);
	for(n = 0; n<10;n++){	
	sema_prc[n] = create(semaphore, 1024, 20, "interrupt",2 ,time, sema_total);
	}
	for(n1=0; n1<10;n1++){	
	resume(sema_prc[n1]); 
	}
	
	starttime = getextime();

	signaln(sema_total,11);//This start all the processes into the ready list;
	//sleep
	
	//sleep(30);
	sleep(30);
	//kill all the process and semaphore
	
	for(n2=0; n2<10;n2++){	
		kill(sema_prc[n2]);
	}
	semdelete(sema);
	semdelete(sema_total);
	endtime = getextime();
	uint32 result;
	result = i/(endtime - starttime);
	return result;
}

//*****
//
//Interrupt
//
//****
void spinlock( int time, sid32 sema_in){
	wait(sema_in);
	while(1){	
		
		mutex_lock(&j);
			
		i++;
		delayus(time);
		mutex_unlock(&j);
	}
}
int run_spinlock( int time){
	int n,n1,n2;
	i = 0;
	mutex_unlock(&j);
	sid32 sema_total;
	sema_total = semcreate(0);
	for(n=0; n<10;n++){	
	spinlock_prc[n] = create(spinlock, 1024, 20, "interrupt",2 ,time, sema_total);
	}
	for(n1=0; n1<10;n1++){	
	resume(spinlock_prc[n1]); 
	}
	
	starttime =getextime();

	signaln(sema_total,11);//This start all the processes into the ready list;
	//sleep
	
	//sleep(30);
	sleep(30);
	//kill all the process and semaphore
	for(n2 = 0; n2<10;n2++){	
		kill(spinlock_prc[n2]);
	}
	semdelete(sema_total);
	endtime = getextime();
	uint32 result;
	result = i/(endtime - starttime);
	return result;
}
void nodelay( int time){
	while(1){	
		
			
		i++;
		delayus(time);
	}
}
int run( int time){
	int n,n1,n2;
	i = 0;
	for(n=0; n<5;n++){	
	nodelay_prc[n] = create(nodelay, 1024, 20, "interrupt",1 ,time);
	}
	for(n=5; n<10;n++){	
	nodelay_prc[n] = create(nodelay, 1024, 20, "interrupt",1 ,time);
	}
	starttime =getextime();
	for(n1=0; n1<10;n1++){	
	resume(nodelay_prc[n1]); 
	delayus(1);
	}
	

	//sleep
	
	//sleep(30);
	sleep(30);
	//kill all the process and semaphore
	for(n2 = 0; n2<10;n2++){	
		kill(nodelay_prc[n2]);
	}
	endtime = getextime();
	uint32 result;
	result = i/(endtime - starttime);
	return result;
}
uint32 start_interrupt;
uint32 end_interrupt;
void	start_tests ()
{
	uint32 result;
	time = 400;
	starttime = 0;
	endtime = 0;
	start_interrupt = 0;
	end_interrupt = 0;
	start_interrupt = getextime();
	result = run_interrupt(time);
	end_interrupt = getextime();
	report_i(time,result);
	sema = semcreate(1);
	result = run_sema(time);
	report_s(time,result);
	result = run_spinlock(time);
	report_l(time,result);
	//result = run(time);
	//report_n(time,result);
	//printf("%d\n",end_interrupt - start_interrupt);

/* Your implementation of Lab 2 goes here */
}
