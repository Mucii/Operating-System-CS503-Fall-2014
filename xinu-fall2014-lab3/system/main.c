/*  main.c  - main */

#include <xinu.h>
void myreceiver(umsg32 *pt){
	umsg32 temp;
	uint32 temp2;
	struct procent *prptr;
	prptr = &proctab[currpid];
	init_buffer(prptr->prmsgbuf,20);
	temp = myreceive();
	printf("myreceive from mysend: %d\n",temp);
	init_buffer(prptr->prmsgbuf,20);
	temp = myreceive();
	printf("myreceive from mysendN=10: %d\n",temp);
	init_buffer(prptr->prmsgbuf,20);
	myreceiven(pt,10);
	init_buffer(prptr->prmsgbuf,20);
	myreceiven(pt,10);
	init_buffer(prptr->prmsgbuf,20);
	myreceiven(pt,10);
	init_buffer(prptr->prmsgbuf,20);
	temp2 = myrecvtime(1000,pt,10);
	printf("myrecvtime from mysendN first condition: %d\n",temp2);
	init_buffer(prptr->prmsgbuf,20);
	temp2 = myrecvtime(1000,pt,10);
	printf("myrecvtime from mysendN second condition: %d\n",temp2);

}
void mysender(pid32 id,umsg32 msg,umsg32 *pt){
	mysend(id,msg);
	uint32 num;
	num = mysendn(id,pt,10);
	printf("mysendN=10 to myreceive: %d\n",num);
	mysend(id,msg);
	printf("mysend to myreceiveN=10\n");
	num = mysendn(id,pt,10);
	printf("mysendN=10 to myreceiveN: %d\n",num);
	num = mysendn(id,pt,3);
	printf("1st mysendN=3 to myreceiveN: %d\n",num);
	num = mysendn(id,pt,4);
	printf("2nd mysendN=4 to myreceiveN: %d\n",num);
	num = mysendn(id,pt,5);
	printf("3rd mysendN=5 to myreceiveN: %d\n",num);
	num = mysendn(id,pt,3);
	printf("1st mysendN=3 to myrecvtime10 first condition: %d\n",num);
	num = mysendn(id,pt,4);
	printf("2nd mysendN=4 to myrecvtime10 first condition: %d\n",num);
	num = mysendn(id,pt,5);
	printf("3rd mysendN=5 to myrecvtime10 first condition: %d\n",num);
	num = mysendn(id,pt,3);
	printf("mysendN=3 to myrecvtime10 second condition: %d\n",num);
}
process	main(void)
{
	/* Place test cases here */
	pid32 proc_sender,proc_receiver;
	umsg32 msgs;
	msgs = 21;
	umsg32 *ptmsg;
	umsg32 buffer[100];
	ptmsg = buffer;
	uint32 i;
	for(i = 0;i<100;i++){
		buffer[i] = i;
	}
	
	proc_receiver = create(myreceiver,1024, 20, "myreceive", 1, ptmsg);    
	proc_sender = create(mysender,1024, 20, "mysend", 3, proc_receiver,msgs,ptmsg);    
	resume(proc_receiver);
	resume(proc_sender);

	return OK;
}
