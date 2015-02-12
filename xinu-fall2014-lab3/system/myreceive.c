/* myreceive.c - myreceive */

#include <xinu.h>
/*------------------------------------------------------------------------
 *  myreceive  -  Wait for a message and return the message to the caller
 *------------------------------------------------------------------------
 */

umsg32	myreceive(void)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process' table entry	*/
	umsg32	msg;			/* Message to return		*/

	mask = disable();
	prptr = &proctab[currpid];
	prptr->msgreq = 1;
	if (prptr->prhasmsg == FALSE) {
		prptr->prstate = MYRECV;
		resched();		/* Block until message arrives	*/
	}
	msg =popqueue(prptr->prmsgbuf);		/* Retrieve message		*/
	if(prptr->prmsgbuf->count == 0){
		prptr->prhasmsg = FALSE;	/* Reset message flag		*/
	}
	restore(mask);
	return msg;

}


/*------------------------------------------------------------------------
 *  myreceiven  -  Wait for 'n' messages and return to the caller
 *------------------------------------------------------------------------
 */
syscall	myreceiven(
	  umsg32* 	msgs,		/* Message contents		*/
	  uint32 	msg_count	/* Number of messages		*/
	)
{	
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process' table entry	*/
	mask = disable();
	prptr = &proctab[currpid];
	prptr->msgreq = msg_count; //keep track of the number requirement
	if (prptr->prmsgbuf->count < msg_count) {
		prptr->prstate = MYRECV;
		resched();		/* Block until message arrives	*/
	}
	if (prptr->prmsgbuf->count >= msg_count) {
	
	uint32 i;
	i = 0;
		while(i<msg_count){
			*msgs =popqueue(prptr->prmsgbuf);		/* Retrieve message		*/
			msgs++;
			i++;
		}
	}
	if(prptr->prmsgbuf->count == 0){
		prptr->prhasmsg = FALSE;	/* Reset message flag		*/
	}
	restore(mask);
	return OK;
}


uint32	myrecvtime(
	  int32		maxwait,	/* Ticks to wait before timeout */
	  umsg32*       msgs,
	  uint32	msg_count
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent	*prptr;		/* Tbl entry of current process	*/

	if (maxwait < 0) {
		return SYSERR;
	}
	mask = disable();

	/* Schedule wakeup and place process in timed-receive state */
	prptr->msgreq = msg_count; //keep track of the number requirement
	prptr = &proctab[currpid];
	if (prptr->prmsgbuf->count < msg_count) {
		if (insertd(currpid,sleepq,maxwait) == SYSERR) {
			restore(mask);
			return SYSERR;
		}
		prptr->prstate = PR_RECTIM;
		resched();		/* Block until message arrives	*/
	}

	/* Either message arrived or timer expired */

	uint32 i;
	i = 0;
	uint32 minval;
	if(msg_count<=prptr->prmsgbuf->count) minval = msg_count;
	else minval = prptr->prmsgbuf->count;
	while(i<minval){
		*msgs =popqueue(prptr->prmsgbuf);/* Retrieve message	*/
		msgs++;
		i++;
	}
	if(prptr->prmsgbuf->count == 0){
		prptr->prhasmsg = FALSE;	/* Reset message flag	*/
	}
	restore(mask);
	return minval;
}
