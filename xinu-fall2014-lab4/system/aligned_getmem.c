/* aligned_getmem.c - aligned_getmem */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  aligned_getmem  -  Allocate heap storage, returning lowest word address
 *                       on an aligned memory boundary.
 *------------------------------------------------------------------------
 */
char  	*aligned_getmem(
	  uint32	nbytes,		/* Size of memory requested	*/
	  uint32	alignment	/* Alignment in bytes 		*/
	)
{

	intmask	mask;			/* Saved interrupt mask		*/
	struct	memblk	*prev, *curr, *leftover;

	mask = disable();
	if (nbytes == 0 || !(((alignment) & (alignment-1)) == 0) || alignment <=0 || nbytes % alignment != 0) {
		restore(mask);
		return (char *)SYSERR;
	}
	nbytes = (uint32) roundmb(nbytes);	/* Use memblk multiples	*/
	alignment = (uint32) roundmb(alignment);
	prev = &memlist;
	curr = memlist.mnext;
	
	while (curr != NULL) {			/* Search free list	*/
		uint32 temp;
		uint32 dif;
		if( ((uint32)(curr) & (alignment-1)) == 0) temp = 0;
		else temp = (uint32)(alignment - ((uint32)(curr) & (alignment-1)));
		if(curr->mlength>=temp) dif = curr->mlength-temp;
		if(temp<=curr->mlength && dif>= nbytes)	curr->mlength -= temp;
		//printf("curr%d,length%d, %d\n",(uint32)(curr),curr->mlength,temp);
		if (dif == nbytes) {	/* Block is exact match	*/
			prev->mnext = curr->mnext;
			memlist.mlength -= nbytes;
			restore(mask);

			return (char *)((alignment - 1 + (uint32)(curr)) & (~(alignment-1)));
		} else if (dif > nbytes) { /* Split big block	*/
			leftover = (struct memblk *)((uint32) ((alignment - 1 + (uint32)(curr)) & (~(alignment-1)))+nbytes);
			prev->mnext = leftover;
			leftover->mnext = curr->mnext;
			leftover->mlength = curr->mlength - nbytes;
			memlist.mlength -= nbytes;
			restore(mask);
			return (char *)((alignment - 1 + (uint32)(curr)) & (~(alignment-1)));
		} else {			/* Move to next block	*/
			prev = curr;
			curr = curr->mnext;
		}
	}
	restore(mask);
	return (char *)SYSERR;
}

