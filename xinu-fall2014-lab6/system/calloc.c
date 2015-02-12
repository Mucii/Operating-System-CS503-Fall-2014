#include <xinu.h>
char *calloc(int32 n, int32 size){
	char *result = getmem(n*size);
	if(result!=NULL)
		(void) memset(result,0,n*size);
	return result;
}
