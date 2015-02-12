/*  main.c  - main */

#include <xinu.h>

process	main(void)
{
	/* Place your test cases here */
	int num;
	num = 3;
	printf("Below is the test samples of aligned_getmem with enough long memory space ,each test takes 3 loops in order to see the length of memory\n");
	printf("bytes = 16, alignment = 3 return: %d\n",aligned_getmem(16,3));
	printf("bytes = 9, alignment = 3 return: %d\n",aligned_getmem(9,3));
	printf("bytes = 66, alignment = 33 return: %d\n",aligned_getmem(66,33));
	printf("bytes = 16, alignment = 32 return: %d\n",aligned_getmem(16,32));
	printf("bytes = 0, alignment = 0 return: %d\n",aligned_getmem(0,0));
	printf("bytes = 32, alignment = 0 return: %d\n",aligned_getmem(32,0));
	int i;
	for(i = 0; i<num;i++){
	printf("bytes = 4, alignment = 4 return: %d\n",aligned_getmem(4,4));
	}
	for(i = 0; i<num;i++){
	printf("bytes = 128, alignment = 128 return: %d\n",aligned_getmem(128,128));
	}
	for(i = 0; i<num;i++){
	printf("bytes = 1024, alignment = 1024 return: %d\n",aligned_getmem(1024,1024));
	}
	for(i = 0; i<num;i++){
	printf("bytes = 16, alignment = 16 return: %d\n",aligned_getmem(16,16));
	}
	for(i = 0; i<num;i++){
	printf("bytes = 18, alignment = 2 return: %d\n",aligned_getmem(18,2));
	}
	for(i = 0; i<num;i++){
	
	printf("bytes = 4, alignment = 2 return: %d\n",aligned_getmem(4,2));
	}
	for(i = 0; i<num;i++){
	printf("bytes = 32, alignment = 2 return: %d\n",aligned_getmem(32,2));
	}
	for(i = 0; i<num;i++){
	printf("bytes = 32, alignment = 16 return: %d\n",aligned_getmem(32,16));
	}

	printf("\n\n\n\n\nGet a gap of memory with 128 bits length, then 32 bits memory, and 16 bits gap, then 32 bits memory:starts from memory address below              \n|||||||                   128 bits                       |||||  16 bits  |||||\n");
	char *temp1;
	char *temp2;
	temp1 = getmem(136);
	printf("the first gap starts with:%d\n",temp1);
	getmem(24);
	temp2 = getmem(24);
	printf("the second gap starts with:%d\n",temp2);
	getmem(24);
	freemem(temp1,128);
	freemem(temp2,16);
	char *test1;
	char *test2;
	char *test3;
	char *test4;
	test1 = aligned_getmem(128,128);
	printf("bytes = 128, alignment = 128 first gap: %d return: %d\n this doesn't work because of high alignment will result in movement on the start of the block, then the space would not be enough\n\n",temp1,test1);
	test2 = aligned_getmem(64,64);
	printf("bytes = 64, alignment = 64 first gap: %d return: %d\nthis works! Because this has lower alignment, even with some movement, we can restore out data in the first gap!\n",temp1,test2);
	printf("segments have formed, start of first gap change to %d\n",test2);
	printf("|||||||  48 bits segments  ||||64bits||||  16 bits left  |||||  16 bits  ||||\n\n");
	freemem(test2,64);
	printf("freemem of the first gap again: the remaining space for first gap is 80\n");
	printf("|||||||  48 bits segments    +      80 bits left         |||||  16 bits  ||||\n\n");
	test4 = aligned_getmem(80,8);
	printf("bytes = 80, alignment = 8 first gap: %d return: %d\nthis perfectly fits into the memory gap with exact bytes the gap free space has! because of very low alignment\n",test2,test4);
	printf("|||||||  48 bits segments  |||||||||||||||||||||||||||||||||||  16 bits  ||||\n\n");
	printf("Notice! The data above didn't fit into second gap\n\n");
	test3 = aligned_getmem(8,8);
	printf("bytes = 8, alignment = 8 second gap: %d return: %d\nthis fits into smaller second gap only when first gap has been filled\n",temp2,test3);
	printf("|||||||  48 bits segments  |||||||||||||||||||||||||||||||||||||||| 8bits||||\n\n");
	return OK;
}
