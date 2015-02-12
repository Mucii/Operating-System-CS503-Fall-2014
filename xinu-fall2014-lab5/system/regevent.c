/* regevent.c - regevent */

#include <xinu.h>
extern pid32 eventprocesspid;
/*------------------------------------------------------------------------
 *  regevent  -  Register an event handler
 *------------------------------------------------------------------------
 */
syscall	regevent(
	  void (*eventhandler)(umsg32 event),	/* Pointer to event	*/
						/*   handler		*/
	  uint32 event			/* Event to register for	*/
	)
{
	intmask mask;
	struct procent *prptr;
	prptr = &proctab[currpid];
	mask = disable();
	if(event > MAX_EVENTS || event<0){
		restore(mask);
		return SYSERR;
	}
	if(event>=USER_EVENT_BOUNDARY){
	prptr->ptrevent[event] = eventhandler;
	restore(mask);
	return OK;
	}
	else{
	send_process_event(eventprocesspid, event, (umsg32)(eventhandler));
	restore(mask);
	return OK;
	}
}
void event_handler(uint32 event){
	if(event == PROCESS_END_EVENT){
		pid32 cur;
		void f();
		cur = create(f,1024,20,"test",0,NULL);
		kprintf("A process has been created");
		kill(cur);
		kprintf("A process has been killed, end event has been handled\n");
	}
	if(event == PROCESS_GETMEM_EVENT){
		char *addr;
		addr = getmem(10);
		kprintf("getmem event has been handled, the address is %d\n",addr);
	}
	if(event == PROCESS_FREEMEM_EVENT){
		char *addr2;
		addr2 = getmem(10);
		freemem(addr2,10);
		kprintf("freemem event has been handled, the address is %d, the size is 10\n");
	}
	if(event == PROCESS_WAITSEM_EVENT){
		sid32 mutex;
		mutex = semcreate(0);
		kprintf("waitsem event has been handled,the procedure will be locked\n");
		wait(mutex);
		signal(mutex);
	}
}


void f(){
}
