/* find_library_function.c - find_library_function */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  find_library_function  -  Locate a dynamically loaded library function
 *------------------------------------------------------------------------
 */
void*	find_library_function(
	  char		*name		/* Function name to locate	*/
        )
{
	struct procent *prptr;
	prptr = &proctab[currpid];
	int32 len_hash;
	len_hash = hashmap_length(prptr->pr_hashtab);
	if(len_hash==0) return (void*) SYSERR;
	int32 *arg;
	//have to use getmem to reference so we can change outsize of this function
	arg = (int32 *)getmem(sizeof(int32 *));
	int32 result;
	result = hashmap_get(prptr->pr_hashtab,name,arg);
	if(result == MAP_OK){
	  return (void*)*arg;
	}
	return (void*)SYSERR;
}
