/*  main.c  - main */

#include <xinu.h>

void start_farm(void);
void stop_farm(void);
void print_farm_report(void);

/************************************************************************/
/*									*/
/* main - main program that Xinu runs as the first user process		*/
/*									*/
/************************************************************************/

int main(int argc, char **argv)
{
	/* Start the farm simulation */
	start_farm();
	
	/* Sleep for 30 seconds to let the simulation run */
	sleep(30);
	
	/* Stop the farm simulation */
	stop_farm();
	
	/* Print the farm report */
	print_farm_report();
	
	return OK;
}
