/* mysend.c - mysend and mysendn */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  mysend  -  Pass a message to a process and start recipient if waiting
 *------------------------------------------------------------------------
 */
void init_buffer(buffer_t *buffer, int size) {
    buffer->size = size;
    buffer->start = 0;
    buffer->count = 0;
    //buffer->element = malloc(sizeof(buffer->element)*size);
    /* allocated array of void pointers. Same as below */
    //buffer->element = malloc(sizeof(void *) * size);
     
}

int full(buffer_t *buffer) {
    if (buffer->count == buffer->size) {
        return 1;
    } else {
        return 0;
    }
}
int size(buffer_t *buffer) {
	return buffer->count;
} 
int empty(buffer_t *buffer) {
    if (buffer->count == 0) {
        return 1;
    } else {
        return 0;
    }
}
     
void push(buffer_t *buffer, umsg32 data) {
    int index;
    if (full(buffer)) {
        ;
    } else {
        index = buffer->start + buffer->count++;
        if (index >= buffer->size) {
            index = 0;
        }
        buffer->element[index] = data;
    }
}
 
 
umsg32 popqueue(buffer_t *buffer) {
	umsg32 element;
		/* FIFO implementation */
		element = buffer->element[buffer->start];
		buffer->start++;
		buffer->count--;
		if (buffer->start == buffer->size) {
			buffer->start = 0;
		}

		return element;
}
 
umsg32 popstack(buffer_t *buffer) {
	int index;
		/* LIFO implementation */
		index = buffer->start + buffer->count - 1;
		if (index >= buffer->size) {
			index = buffer->count - buffer->size - 1;
		}     
		buffer->count--;
		return buffer->element[index];
}

/*end of implementation of buffer*/
syscall	mysend(
	  pid32		pid,		/* ID of recipient process	*/
	  umsg32	msg		/* Contents of message		*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process' table entry	*/
	
	mask = disable();
	prptr = &proctab[pid];
	if(!prptr->prhasmsg) init_buffer(prptr->prmsgbuf, 20); 
	if (isbadpid(pid)) {
		restore(mask);
		return SYSERR;
	}

	if ((prptr->prstate == PR_FREE) || prptr->prmsgbuf->count == 20) {
		restore(mask);
		return SYSERR;
	}
	prptr->prmsg = msg;		/* Deliver message		*/
	push(prptr->prmsgbuf,msg);      /* Push the message into the buf*/
	prptr->prhasmsg = TRUE;		/* Indicate message is waiting	*/

	/* If recipient waiting or in timed-wait make it ready */

	if (prptr->prstate == MYRECV) {
		ready(pid);
	} else if (prptr->prstate == PR_RECTIM) {
		unsleep(pid);
		ready(pid);
	}
	restore(mask);		/* Restore interrupts */
	return OK;


}

/*------------------------------------------------------------------------
 *  mysendn  -  Pass 'n' messages to a process and start recipient if waiting
 *------------------------------------------------------------------------
 */
uint32	mysendn(
	  pid32		pid,		/* ID of recipient process	*/
	  umsg32* 	msgs,		/* Message contents		*/
	  uint32 	msg_count	/* Number of messages		*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process' table entry	*/
	mask = disable();
	prptr = &proctab[pid];
	if(!prptr->prhasmsg) init_buffer(prptr->prmsgbuf, 20); 
	if (isbadpid(pid)) {
		restore(mask);
		return SYSERR;
	}

	if ((prptr->prstate == PR_FREE) || (prptr->prmsgbuf->count == 20)) {
		restore(mask);
		return SYSERR;
	}

	prptr->prmsg = *msgs;		/* Deliver message		*/
	uint32 i;
	i = 0;

	while((prptr->prmsgbuf->count!=20) &&  (i<msg_count) && (prptr->prmsgbuf->count<prptr->msgreq)){
		push(prptr->prmsgbuf,*msgs);      /* Push the message into the buf*/
		msgs++;
		i++;
	}
	prptr->prhasmsg = TRUE;		/* Indicate message is waiting	*/
	/* If recipient waiting or in timed-wait make it ready */

	if ((prptr->prstate == MYRECV) && (prptr->msgreq <= prptr->prmsgbuf->count)) {
		ready(pid);
	} else if ((prptr->prstate == PR_RECTIM) && (prptr->msgreq <= prptr->prmsgbuf->count)) {
		unsleep(pid);
		ready(pid);
	}
	restore(mask);		/* Restore interrupts */
	return i;
}
