/* delay.h - DELAY */

/*
 * Delay units are in microseconds.
 */
#define	delayus(n)                                	\
{                                               	\
	register int i = 0;				\
							\
	while(i < (n * 100)) i++;			\
}							\

/*
 * Delay units are in milli-seconds.
 */
#define	delayms(n)					\
{							\
	register long i;				\
							\
	for (i=n;i>0;i--) {				\
		delayus(1000);				\
	}						\
}

#define DELAY	delayus
#define MDELAY	delayms
