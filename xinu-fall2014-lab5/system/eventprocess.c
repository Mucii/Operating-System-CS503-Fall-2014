/* eventprocess.c - eventprocess */

#include <xinu.h>

pid32 eventprocesspid;

/*------------------------------------------------------------------------
 *  eventprocess  -  Process for calling event handlers
 *------------------------------------------------------------------------
 */
void	eventprocess(void)
{
	umsg32 rcvd_msg;
	struct procent *prptr;
	while(1) {
		rcvd_msg = receive();
		event_handler_type msg_f;
		msg_f = (event_handler_type)(rcvd_msg);
		prptr = &proctab[currpid];
		msg_f(prptr->prevent);
	}
}
