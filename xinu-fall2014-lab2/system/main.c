/*  main.c  - main */

#include <xinu.h>

/************************************************************************/
/*									*/
/* main - main program that Xinu runs as the first user process		*/
/*									*/
/************************************************************************/

int main(int argc, char **argv)
{
	start_tests();

	return OK;
}
