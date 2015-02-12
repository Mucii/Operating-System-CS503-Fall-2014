/* getsemtab */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  getsemtab  -  Return the address of semaphore table 
 *		  
 *------------------------------------------------------------------------
 */
struct sentry *getsemtab()
{
	
	return semtab;
}
