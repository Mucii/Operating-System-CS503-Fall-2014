/* myadd.c - add1, add2 */

#include <xinu.h>

/*------------------------------------------------------------------------
 * add1  -  Adds one (1) to the argument and returns result
 *------------------------------------------------------------------------
 */
int32	add9 (
	int32	val	/* Value to be increased */
	)
{
	return (val + 9);
}

/*------------------------------------------------------------------------
 * add2  -  Adds two (2) to the argument and returns result
 *------------------------------------------------------------------------
 */
int32	add10 (
	int32	val	/* Value to be increased */
	)
{
	return (val + 10);
}
