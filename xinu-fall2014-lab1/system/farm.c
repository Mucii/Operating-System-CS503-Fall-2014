/*  farm.c  - definition of farmer and vegetarian lab */
#include <xinu.h>
#include "farm.h"
/*------------------------------------------------------------------------
 *  start_farm  -  Initializes and starts the farm simulation
 *------------------------------------------------------------------------
 */
#include <stdio.h>
#include <stdlib.h>
struct buffer {
	int size;
    int start;
    //int end;  // position of last element
    /* Tracking start and end of buffer would waste
     * one position. A full buffer would always have
     * to leave last position empty or otherwise
     * it would look empty. Instead this buffer uses
     * count to track if buffer is empty or full
     */
    int count; // number of elements in buffer
    /* Two ways to make buffer element type opaque
     * First is by using typedef for the element
     * pointer. Second is by using void pointer.
     */
    /* different types of buffer:
    int *element;   // array of integers
    char *element;  // array of characters
    void *element;  // array of void type (could cast to int, char, etc)
    char **element; //array of char pointers (array of strings)
    void **element; // array of void pointers
    Choosing array of void pointers since it's the most flexible */
    char element[FIELDSIZE];
};
sid32 prod_sema,cons_sema,cons_sema_t;
typedef struct buffer buffer_t;
sid32 farmer[NFARMERS];
pid32 prc_farmer[NFARMERS];
sid32 vegetarian[NVEGETARIANS];
pid32 prc_vege[NVEGETARIANS];
char result[NVEGETARIANS][FIELDSIZE*NFARMERS];
void init_buffer(buffer_t *buffer, int size) {
    buffer->size = size;
    buffer->start = 0;
    buffer->count = 0;
    //buffer->element = malloc(sizeof(buffer->element)*size);
    /* allocated array of void pointers. Same as below */
    //buffer->element = malloc(sizeof(void *) * size);
     
}


int full(buffer_t *buffer) {
    if (buffer->count == buffer->size) {
        return 1;
    } else {
        return 0;
    }
}
int size(buffer_t *buffer) {
	return buffer->count;
} 
int empty(buffer_t *buffer) {
    if (buffer->count == 0) {
        return 1;
    } else {
        return 0;
    }
}
     
void push(buffer_t *buffer, char data) {
    int index;
    if (full(buffer)) {
        printf("Buffer overflow\n");
    } else {
        index = buffer->start + buffer->count++;
        if (index >= buffer->size) {
            index = 0;
        }
        buffer->element[index] = data;
    }
}
 
 
char popqueue(buffer_t *buffer) {
	char element;
		/* FIFO implementation */
		element = buffer->element[buffer->start];
		buffer->start++;
		buffer->count--;
		if (buffer->start == buffer->size) {
			buffer->start = 0;
		}

		return element;
}
 
char popstack(buffer_t *buffer) {
	int index;
		/* LIFO implementation */
		index = buffer->start + buffer->count - 1;
		if (index >= buffer->size) {
			index = buffer->count - buffer->size - 1;
		}     
		buffer->count--;
		return buffer->element[index];
}
 
void prod(sid32 selfsema,sid32 othersema, buffer_t *buf, char tags, int times){
	//kprintf("prod\n");	
	while(1){
		wait(selfsema);
		push(buf, tags);
		signal(othersema);
		sleepms(times);
	
	}

}

void cons(int idx, sid32 cons_sema,sid32 selfsema, sid32 othersema, buffer_t *buf, char tags, int hungers, int times){
	int j,count,j2;
	count = 0;
	while(1){
		wait(selfsema);
		for (j=0; j<hungers;j++){
			wait(cons_sema);
			
			//wait(selfsema);
			//sleepms(1);
			//kprintf("NNNNNN%d,MMMMMM%d,BBBBB%c",count,idx,temp);
			//kprintf("Num%d,Farmer%c,Vege%d\n",count,temp,idx);
			//push(buf_r[idx], temp);
			char temp;
			temp = popqueue(buf);
			result[idx][count]=temp;
			count++;
			}

		//for (j1=0; j1<((int)(cons_sema)-hungers);j1++){
		//	signal(nextsema);
		//}	
		signal(selfsema);
		for (j2=0; j2<hungers;j2++){
			signal(othersema);
		}
		
		sleepms(times);
	}
}

void start_farm(void)
{
	//kprintf("start\n");	
	buffer_t *buf=NULL;
	init_buffer(buf, FIELDSIZE*3);
	prod_sema = semcreate(FIELDSIZE);
	//kprintf("%d\n",prod_sema);
	cons_sema_t = semcreate(0);
	cons_sema = semcreate(1);
	int j1,j2;
	for(j1=0;j1<NVEGETARIANS;j1++){
		for(j2=0;j2<NFARMERS;j2++){
			result[j1][j2] = '\0';
		}
	}

	//kprintf("%d\n",cons_sema);
	//int bufsize = FIELDSIZE*3;
	int i,j,i1,i2;
	/*for(i3=0;i3<NVEGETARIANS;i3++){
		init_buffer(buf_r[i3], bufsize);
	}
	*/
	for(i=0; i<NFARMERS; i++){
		prc_farmer[i] = create(prod, 1024, 200, "farmer", 5, prod_sema, cons_sema_t, buf, farmer_tags[i], farmer_grow_times[i]);
		//kprintf("process start prod\n");
	}
	for( j=0; j<NVEGETARIANS; j++){
	prc_vege[j] = create(cons, 1024, 20 , "vegetarian", 8, j,cons_sema_t, cons_sema, prod_sema, buf, vegetarian_tags[j], vegetarian_hungers[j], vegetarian_sleep_times[j]);
	}
//	prc_vege[0] = create(cons,  1024, 50 , "vegetarian1",8 , 0, cons_sema_t, cons_sema,prod_sema, buf, vegetarian_tags[0], vegetarian_hungers[0], vegetarian_sleep_times[0]);
//	prc_vege[1] = create(cons, 1024, 100 , "vegetarian2", 8, 1, cons_sema_t,cons_sema, prod_sema, buf, vegetarian_tags[1], vegetarian_hungers[1], vegetarian_sleep_times[1]);
//	prc_vege[2] = create(cons, 1024, 150 , "vegetarian3", 8, 2, cons_sema_t,cons_sema, prod_sema, buf, vegetarian_tags[2], vegetarian_hungers[2], vegetarian_sleep_times[2]);
		//kprintf("process start cons\n");
//	}
	for(i1=0; i1<NFARMERS; i1++){
		resume(prc_farmer[i1]);	
		//	kprintf("resume here farmers\n");
	}
	for( i2=0; i2<NVEGETARIANS; i2++){
		resume(prc_vege[i2]);	
		//	kprintf("resume here vegetarians\n");
	}
}

/*------------------------------------------------------------------------
 *  stop_farm  -  Stops the currently executing farm simulation
 *------------------------------------------------------------------------
 */
void stop_farm(void)
{
	//kprintf("start to stop\n");	
	semdelete(prod_sema);
	semdelete(cons_sema_t);
	semdelete(cons_sema);
	int i,i1;
	for( i=0; i<NFARMERS; i++){
		kill(prc_farmer[i]);
	}
	for( i1=0; i1<NVEGETARIANS; i1++){
		kill(prc_vege[i1]);
	}
}


/*------------------------------------------------------------------------
 *  print_farm_report  -  Prints the final report for the farm simulation
 *------------------------------------------------------------------------
 */
void print_farm_report(void)
{
	//kprintf("start to print\n");		
	int num_f[NFARMERS];
	int i,i1,i2,j1,j2,i3,j3,j4;
	for(i = 0; i<NFARMERS; i++){
		num_f[i] = 0;
	}
	int num_v[NVEGETARIANS][NFARMERS];

	for(i1 = 0; i1<NVEGETARIANS; i1++){
		for(j1 = 0; j1<NFARMERS; j1++){
			num_v[i1][j1] = 0;
		}
	}
	//memset( (void*)num_v,NVEGETARIANS*NFARMERS,0);

	for(i2 = 0; i2<NVEGETARIANS; i2++){
		int ncount;
		ncount = 0;
		//buffer_t *temp_buf = buf_r[i2];
		//kprintf("a size?%d\n",size(temp_buf));
		while(1){
			char temp;
			temp = result[i2][ncount];
			if(temp == '\0') break;
			//kprintf("%d,%c\n",i2,temp);
			for(j2 = 0;j2<NFARMERS; j2++){
				if(temp == farmer_tags[j2]){ 
					num_v[i2][j2]++;
					num_f[j2]++;
			//		kprintf("%d\n",num_v[i2][j2]);
			//		kprintf("%d\n",num_f[j2]);
				};
			}
		//kprintf("a size?%d\n",size(buf_r[0]));
		//kprintf("b size?%d\n",size(buf_r[1]));
		//kprintf("c size?%d\n",size(buf_r[2]));
		ncount++;
		}
	}

	for(i3 = 0; i3<NFARMERS; i3++){
		kprintf("Farmer %c:sold %d carrots\n",farmer_tags[i3], num_f[i3]);

	}
	for(j3 = 0; j3<NVEGETARIANS; j3++){
		kprintf("Vegetarian %c: ",vegetarian_tags[j3]);
		for(j4 = 0; j4<NFARMERS-1;j4++){
			kprintf("%d from farmer %c, ", num_v[j3][j4],farmer_tags[j4]);
		}
		kprintf("%d from farmer %c.", num_v[j3][NFARMERS-1],farmer_tags[NFARMERS-1]);
		kprintf("\n");
	}
}
