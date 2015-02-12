/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>

extern uint32 nsaddr;

process	main(void)
{


	/* Start the network */
	int swtch;
	/*swtch for test cases
	 *1 shell test
	 *2 helloworld test
	 *3 hash table test
	 *4 load_library test
	 *5 load_library test for too many functions and duplicate functions
	 */
	swtch = 5;

	netstart();
	if(swtch == 1){
	   kprintf("\n...creating a shell\n");
	   recvclr();
	   resume(create(shell, 8192, 50, "shell", 1, CONSOLE));
	   
	/* Wait for shell to exit and recreate it */
	
	   while (TRUE) {
	   receive();
	   sleepms(200);
	   kprintf("\n\nMain process recreating shell\n\n");
	   resume(create(shell, 4096, 20, "shell", 1, CONSOLE));
	   }
	} 




	/* main for HellowWorld
	   */
	else if(swtch==2){
	void* helloworld = load_program("helloworld");
	  // kprintf("helloworld addr return %d\n",helloworld);
	   pid32 hi;

	   hi = create(helloworld,4096,20,"helloworld",2,0,NULL);
	   //kprintf("hi %d\n",hi);
	   resume(hi);
	   //kprintf("Done");
	   return OK;
	  } 





	/* Test case for the hashmap implementations
*/
	else if(swtch ==3){
	   map_t mymap = hashmap_new();	
	   kprintf("mymap address: %d\n", mymap);

	   char *str = "Helloworld";
	   int val = 100;

	   kprintf("put\n");
	   int rl = hashmap_put(mymap,str,val);
	   if (rl != MAP_OK){
	   kprintf("put failure");
	   return OK;
	   }
	   char *str1 = "Helloworld2";
	   int val1 = 101;
	   int rl1 = hashmap_put(mymap,str1,val1);
	   kprintf("put2\n");
	   if (rl1 != MAP_OK){
	   kprintf("put failure2");
	   return OK;
	   }
	   int *arg=0;
	   int rl2 = hashmap_get(mymap,str,arg);
	   kprintf("get\n");
	   if (rl2 != MAP_OK){
	   kprintf("get failure");
	   return OK;
	   }
	   else kprintf("arg: %d\n",*arg);
	   int *arg1=0;
	   int rl3 = hashmap_get(mymap,str1,arg1);
	   kprintf("get2\n");
	   if (rl3 != MAP_OK){
	   kprintf("get2 failure");
	   return OK;
	   }
	   else kprintf("arg1: %d\n",*arg1);
	   int len = hashmap_length(mymap);
	   kprintf("length :%d\n",len);
	   return OK;
	   }

	/* Test case for part two load_library
*/
	else if(swtch == 4){
	int myvalue = 2;

	if(load_library("myadd") == SYSERR) {
		return SYSERR;
	}
	if(load_library("myadd1") == SYSERR) {
		return SYSERR;
	}
	if(load_library("myadd2") == SYSERR) {
		return SYSERR;
	}
	if(load_library("myadd3") == SYSERR) {
		kprintf("main file notice, too many files in library\n");
	}
	if(load_library("myadd5") == SYSERR) {
		kprintf("main file notice, duplicate functions loaded\n");
	}
	if(load_library("myadd6") == SYSERR) {
		kprintf("main file notice, too many functions in a file\n");
	}

	int32 (*add1)(int32) = find_library_function("add1");
	if((int32)add1 == SYSERR) {
		return SYSERR;
	}
	kprintf("final result of add1: %d\n",add1(myvalue));
	int32 (*add2)(int32) = find_library_function("add2");
	if((int32)add2 == SYSERR) {
		return SYSERR;
	}
	kprintf("final result of add2: %d\n",add2(myvalue));
	int32 (*add3)(int32) = find_library_function("add3");
	if((int32)add3 == SYSERR) {
		return SYSERR;
	}
	kprintf("final result of add3: %d\n",add3(myvalue));
	int32 (*add4)(int32) = find_library_function("add4");
	if((int32)add4 == SYSERR) {
		return SYSERR;
	}
	kprintf("final result of add4: %d\n",add4(myvalue));
	int32 (*add5)(int32) = find_library_function("add5");
	if((int32)add5 == SYSERR) {
		return SYSERR;
	}
	kprintf("final result of add5: %d\n",add5(myvalue));
	int32 (*add6)(int32) = find_library_function("add6");
	if((int32)add6 == SYSERR) {
		return SYSERR;
	}
	kprintf("final result of add6: %d\n",add6(myvalue));
	return OK;
	}
	else if(swtch == 5){
	int myvalue = 2;
	if(load_library("myadd") == SYSERR) {
		return SYSERR;
	}
	if(load_library("myadd5") == SYSERR) {
		kprintf("main file notice, duplicate functions loaded\n");
	}
	if(load_library("myadd6") == SYSERR) {
		kprintf("main file notice, too many functions in a file\n");
	}

	return OK;
	}
	return SYSERR;
}
