/* myadd.c - add1, add2 */

#include <xinu.h>

/*------------------------------------------------------------------------
 * add1  -  Adds one (1) to the argument and returns result
 *------------------------------------------------------------------------
 */
int32	add7 (
	int32	val	/* Value to be increased */
	)
{
	return (val + 7);
}

/*------------------------------------------------------------------------
 * add2  -  Adds two (2) to the argument and returns result
 *------------------------------------------------------------------------
 */
int32	add8 (
	int32	val	/* Value to be increased */
	)
{
	return (val + 8);
}
