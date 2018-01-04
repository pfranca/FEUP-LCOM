#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/driver.h>
#include <minix/com.h>
#include "i8254.h"
#include "timer.h"


//Global Variables
static int hookID = 0;
int counter = 0;

int timer_subscribe_int(void){

	//Save original hook ID for the irq_set
	int originalHook=hookID;

	//Subscribe timer 0 interrupts
	if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE,&hookID) != OK || sys_irqenable(&hookID) != OK)
		return -1;

	return originalHook;
}

int timer_unsubscribe_int(){

	//Unsubscribe timer 0 interrupts
	if(sys_irqdisable(&hookID) != OK || sys_irqrmpolicy(&hookID) != OK)
		return 1;

	return 0;
}

void timer_int_handler(){

	//Increment the counter
	counter++;
}
