/*  farm.h  - definition of farmer and vegetarian lab */

/* Globals variables */
#define NFARMERS	3		/* The number of farmers	*/
#define NVEGETARIANS	3		/* The number of vegetarians	*/
#define FIELDSIZE	200		/* The size of the field	*/

/* Tags for each farmer							*/
char farmer_tags[NFARMERS] = { 'A', 'B', 'C' };	

/* Grow times (sleep time) for each farmer				*/
int farmer_grow_times[NFARMERS] = { 1000, 1000, 1000 };


/* Tags for each vegetarian						*/
char vegetarian_tags[NVEGETARIANS] = { 'a', 'b', 'c' };

/* Sleep times for each*/
int vegetarian_sleep_times[NVEGETARIANS] = { 1000, 1000, 1000 };
/* Vegetarian hunger values, the number of carrots consumed		*/
int vegetarian_hungers[NVEGETARIANS] = { 1, 2, 1 };
