/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>
/*
self send test case - a process registers an event handler and sends
 *     an event to itself
 ------------------------------------------------------------------------
 */

uint32 self_expected_event;
uint32 self_event_received;
uint32 self_send_test_success;
void self_event_handler(uint32 event) 
{
	if(event == self_expected_event) {
		kprintf("Received expected event\n");
		self_event_received = 1;
	} else {
		kprintf("Received unexpected event: %d\n", event);
		self_send_test_success = 0;
	}
}
void testcase_selfsend(void)
{
	uint32 rc;
	self_expected_event = 12;
	
	self_send_test_success = 1;
	self_event_received = 0;
	
	kprintf("Self send testcase\n");

	rc = regevent(self_event_handler, self_expected_event);
	if(rc != OK) {
		kprintf("ERROR: regevent failed: %d\n", rc);
		return;
	}
	
	rc = sendevent(getpid(), self_expected_event);
	if(rc != OK) {
		kprintf("ERROR: sendevent failed: %d\n", rc);
		return;
	}
	
	sleep(2);
	
	if(self_event_received != 1) {
		kprintf("Failed to receive an event\n");
		self_send_test_success = 0;
	}

	if(self_send_test_success == 1) {
		kprintf("Success\n");
	} else {
		kprintf("Failed\n");
	}
	printf("\n\n");
}
/*
 *  getmem test case - a process registers an event handler for the getmem
 *     OS event and then allocates memory
 */

uint32 getmem_event_received;
uint32 getmem_test_success;
void getmem_event_handler(uint32 event)
{
	if(event == PROCESS_GETMEM_EVENT) {
		kprintf("Received expected event\n");
		getmem_event_received = 1;
	} else {
		kprintf("Received unexpected event: %d\n", event);
		getmem_test_success = 0;
	}
}
void testcase_getmem(void)
{
	uint32 rc;
	char* ptr;
	
	getmem_test_success = 1;
	
	kprintf("getmem testcase\n");
	
	rc = regevent(getmem_event_handler, PROCESS_GETMEM_EVENT);
	if(rc != OK) {
		kprintf("ERROR: regevent failed: %d\n", rc);
		return;
	}
	
	ptr = getmem(32);
	freemem(ptr, 32);
	sleep(2);
	
	if(getmem_event_received != 1) {
		kprintf("Failed to receive an event\n");
		getmem_test_success = 0;
	}
	
	if(getmem_test_success == 1) {
		kprintf("Success\n");
	} else {
		kprintf("Failed\n");
	}
	printf("\n\n");
}


/*  Other process send test case - a process registers for an event
 *     then starts another process to send it the event.
 */

uint32 process_expected_event;
uint32 process_event_received;
uint32 process_send_test_success;
void proccess_send_event_handler(uint32 event)
{
	if(event == process_expected_event) {
		kprintf("Received expected event\n");
		process_event_received = 1;
	} else {
		kprintf("Received unexpected event: %d\n", event);
		process_send_test_success = 0;
	}
}
void process_send_process(pid32 pid, uint32 event_to_send)
{
	uint32 rc;
	rc = sendevent(pid, event_to_send);
	if(rc != OK) {
		kprintf("Send event failure: %d\n", rc);
		process_send_test_success = 0;
	}
}
void testcase_process_send(void)
{
	uint32 rc;
	pid32 send_process_pid;
	process_expected_event = 20;
	
	process_send_test_success = 1;
	process_event_received = 0;
	
	kprintf("Other process send testcase\n");

	rc = regevent(proccess_send_event_handler, process_expected_event);
	if(rc != OK) {
		kprintf("ERROR: regevent failed: %d\n", rc);
		return;
	}
	
	send_process_pid = create(process_send_process, 4096, 30, "Send Event", 2, getpid(), process_expected_event);
	resume(send_process_pid);
	
	sleep(2);
	
	if(process_event_received != 1) {
		kprintf("Failed to receive an event\n");
		process_send_test_success = 0;
	}
	
	if(!isbadpid(send_process_pid)) {
		kprintf("ERROR Send process still running\n");
		kill(send_process_pid);
		process_send_test_success = 0;
	}

	if(process_send_test_success == 1) {
		kprintf("Success\n");
	} else {
		kprintf("Failed\n");
	}
	printf("\n\n");
}

process	main(void)
{
	testcase_selfsend();
	testcase_getmem();
	testcase_process_send();
	
	return OK;
}





