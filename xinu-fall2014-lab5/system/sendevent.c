/* sendevent.c - sendevent */

#include <xinu.h>

extern pid32 eventprocesspid;

syscall	broadcastevent(
	  uint32	event		/* Event to send		*/
	
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	mask = disable();
	uint32 i;
	static pid32 nextpid = 1;
	for( i = 0; i < NPROC; i++) {
		nextpid %= NPROC; /* Wrap around to beginning */
		if (proctab[nextpid].ptrevent[event] != NULL) {
			sendevent(nextpid,event);
			nextpid++;
		} else {
			nextpid++;
		}
	}
	
	restore(mask);		/* Restore interrupts */
	return OK;

}
syscall	send_process_event(
	  pid32		pid,		/* ID of recipient process	*/
	  uint32	event,		/* Event to send		*/
	  umsg32        msg
	
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process' table entry	*/
	mask = disable();
	if (isbadpid(pid)) {
		restore(mask);
		return SYSERR;
	}
		
	prptr = &proctab[pid];
	if ((prptr->prstate == PR_FREE) ||  event > MAX_EVENTS) {
		restore(mask);
		
		return SYSERR;
	}
	 
	prptr->prmsg =msg;/* Deliver message	*/
	prptr->prhasmsg = TRUE;	/* Indicate message is waiting	*/
	prptr->prevent = event;
	/* If recipient waiting or in timed-wait make it ready */
	//kprintf("hasmsg sign to be TRUE ptrevent:%d\nafter transform%d",prptr->ptrevent[event], prptr->prmsg);
	if (prptr->prstate == PR_RECV) {
		ready(pid);
	} else if (prptr->prstate == PR_RECTIM) {
		unsleep(pid);
		ready(pid);
	}
	restore(mask);		/* Restore interrupts */
	return OK;

}

/*
 * ------------------------------------------------------------------------
 *  sendevent  -  Sends an event to the given process
 ------------------------------------------------------------------------
 */
syscall	sendevent(
	  pid32		pid,		/* ID of recipient process	*/
	  uint32	event		/* Event to send		*/
	)
{
	umsg32 msg;
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process' table entry	*/
	mask = disable();
	if (isbadpid(pid)) {
		restore(mask);
		return SYSERR;
	}
	
	prptr = &proctab[pid];
	if ((prptr->prstate == PR_FREE) || prptr->ptrevent[event] == NULL || event < USER_EVENT_BOUNDARY ||  event > MAX_EVENTS) {
		restore(mask);
		
		return SYSERR;
	}
	 
	
	msg =(umsg32)(prptr->ptrevent[event]);/* Deliver message	*/
	/* If recipient waiting or in timed-wait make it ready */
	//kprintf("hasmsg sign to be TRUE ptrevent:%d\nafter transform%d",prptr->ptrevent[event], prptr->prmsg);
	send_process_event(eventprocesspid, event, msg);
	

	restore(mask);		/* Restore interrupts */
	return OK;

}


