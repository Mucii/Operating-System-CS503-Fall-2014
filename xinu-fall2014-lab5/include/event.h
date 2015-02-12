/* event.h */

/* Operating system events						*/
#define PROCESS_END_EVENT 	1	/* Event generated when a	*/
					/*   process finishes execution	*/
#define PROCESS_GETMEM_EVENT	2	/* Event generated when a 	*/
					/*   process successfully 	*/
					/*   allocates memory with 	*/
					/*   getmem			*/
#define PROCESS_FREEMEM_EVENT	3	/* Event generated when a 	*/
					/*   process succesfully	*/
					/*   frees memory with freemem	*/
#define PROCESS_WAITSEM_EVENT	4	/* Event generated when a 	*/
					/*   process calls wait on a	*/
					/*   semaphore and the value	*/
					/*   of the semaphore causes 	*/
					/*   it to go on the wait queue	*/

/* User events - 							*/
/*    callers of sendevent cannot use event values less than the	*/
/*    USER_EVENT_BOUNDARY						*/
#define USER_EVENT_BOUNDARY	10	/* User events begin here	*/


#define MAX_EVENTS		50	/* Maximum number of events	*/