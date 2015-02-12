/* mutux.S - mutex_lock, mutex_unlock */

	.text
	.globl mutex_lock
	.globl mutex_unlock

/*------------------------------------------------------------------------
 * mutex_lock(uint32 *lock)  --  Acquire a lock
 *------------------------------------------------------------------------
 */
mutex_lock:

	/* Save registers that will be modified */

	pushl	%eax
	pushl	%ebx
	pushl	%ecx

	movl	$1, %ebx	/* Place the "locked" value in ebx	*/
	movl	16(%esp), %ecx	/* Place the address of the lock in ecx	*/

spinloop:
	movl	$0, %eax	/* Place the "unlocked" value in eax	*/
	lock	cmpxchg %ebx, (%ecx)  /* Atomic compare-and-exchange:	*/
				/* Compare eax with memory (%ecx)	*/
				/* if equal				*/
				/*	load %ebx in memory (%ecx), ZF=1*/
				/* else					*/
				/*	load memory in %eax, ZF=0	*/

	/* If ZF = 0, the mutex was locked, so continue the spin loop	*/

	jnz	spinloop

	/* We hold the lock now, so pop the saved registers and return	*/
	popl	%ecx
	popl	%ebx
	popl	%eax
	ret

/*------------------------------------------------------------------------
 * mutex_unlock (uint32 *lock) - release a lock
 *------------------------------------------------------------------------
 */
mutex_unlock:

	/* Save register eax */
	pushl	%eax

	/* Load the address of lock onto eax */
	movl	8(%esp), %eax

	/* Store the "unlocked" value in the lock, thereby unlocking it	*/
	movl	$0, (%eax)

	/* Restore the saved register and return */
	popl	%eax
	ret
