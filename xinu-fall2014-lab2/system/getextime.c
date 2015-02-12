/* getextime.c - getextime, extime_init */

#include <xinu.h>
#include <extime.h>

/*------------------------------------------------------------------------
 * extime_init  -  Initialize the RTC time registers
 *------------------------------------------------------------------------
 */
void	extime_init()
{
	int32	i;	/* index register	*/
	byte	regb;	/* register B value	*/

	/* Turn off the time update, to initialize the clock	*/

	outb(RTC_IR, RTC_REGBOFF);
	regb = inb(RTC_TR);

	outb(RTC_IR, RTC_REGBOFF);
	outb(RTC_TR, regb | 0x80);

	/* Zero out the secs, mins, hours counters	*/

	for(i = 0; i < 10; i++) {
		outb(RTC_IR, i);
		outb(RTC_TR, 0);
	}

	/* Statr the clock now */

	outb(RTC_IR, RTC_REGBOFF);
	regb = inb(RTC_TR);

	outb(RTC_IR, RTC_REGBOFF);
	outb(RTC_TR, (regb & 0x7F) | 0x06);
}

/*------------------------------------------------------------------------
 * getextime  -  get time from the Real-Time clock (external time)
 *------------------------------------------------------------------------
 */
uint32	getextime()
{
	uint32	secs, mins, hours, dow;

	/* Read the number of seconds	*/

	outb(RTC_IR, 0);
	secs = inb(RTC_TR);

	/* Read the number of minutes	*/

	outb(RTC_IR, 2);
	mins = inb(RTC_TR);

	/* Read the number of hours	*/

	outb(RTC_IR, 4);
	hours = inb(RTC_TR);

	/* Read the Day of Week */

	outb(RTC_IR, 6);
	dow = inb(RTC_TR);

	/* Calculate the total number of seconds and return */

	return (secs + 60*mins + 3600*hours + 3600*24*dow);
}
