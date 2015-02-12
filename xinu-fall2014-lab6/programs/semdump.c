#include <xinu.h>
#include <string.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * this command takes no parameters and prints the contents of the semaphore table to the user. You must provide a readable table format for the output including a table header and the contents of each semaphore table entry. See semaphore.h for a description of the contents of the semaphore table.
 *------------------------------------------------------------------------
 */
shellcmd main() {
	int i;
	struct sentry *semptr;
	kprintf("Entry   State   Count   Queue\n");
	for(i=0;i<NSEM;i++){
		struct sentry *pt =(struct sentry *) getsemtab();
		semptr = &pt[i];
		kprintf("%d       ",i);
		if(semptr->sstate==S_FREE) kprintf("S_FREE  0       0\n");
		else if(semptr->sstate==S_USED){
		kprintf("S_USED  ");
		kprintf("%d       %d\n",semptr->scount,semptr->squeue);
		}
	}
	return 1;
}
