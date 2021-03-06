#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include "i8254.h"
#include "timer.h"


//Global Variables
int hookID = 0;
int counter = 0;

int timer_set_frequency(unsigned char timer, unsigned long freq){

	unsigned char stat;

	//Read timer conf
	if(timer_get_conf(timer, &stat)){
		printf("Error reading timer configurations\nIn timer_set_frquency::timer_get_conf()\n");
		return 1;
	}

	if(freq==1){
		printf("Freq cannot be 1\nMode 3 does not allow frequency of 1\n");
		return 1;
	}

	unsigned short div;
	unsigned short LSB;
	unsigned short MSB;

	if(freq==0){

	}else{
	div=TIMER_FREQ/freq;
	}


	LSB = div & 0xFF;
	MSB = (div>>8) & 0xFF;


	unsigned long timer_sel;

	switch(timer){
	case 0 :
		timer_sel= TIMER_SEL0;
		break;
	case 1 :
		timer_sel = TIMER_SEL1;
		break;
	case 2 :
		timer_sel = TIMER_SEL2;
		break;
	default:
		printf("Wrong timer");
		return 1;
	}

	//Writing to control reg to program timer
	if(sys_outb(TIMER_CTRL, (stat & 0xF)|(timer_sel|TIMER_LSB_MSB))){
		printf("Error in writing to control reg!\nIn timer_set_frequency::sys_outb()\n");
		return 1;
	}

	unsigned long porta;

	switch(timer){
		case 0 :
			porta = TIMER_0;
			break;
		case 1 :
			porta = TIMER_1;
			break;
		case 2 :
			porta = TIMER_2;
			break;
		default:
			printf("Wrong timer");
			return 1;
		}

	//Writing LSB of initial value to timer port
	if(sys_outb(porta, LSB)){
		printf("Error writing to LSB");
		return 1;
	}
	//Writing MSB of initial value to timer port
	if(sys_outb(porta, MSB)){
		printf("Error writing to MSB");
		return 1;
	}


	return 0;
}

int timer_subscribe_int(void ){

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

int timer_get_conf(unsigned char timer, unsigned char *st){

	//Check if user input one of the 3 existing timers
	if(timer<0 || timer>2){
		printf("Error timer_get_vonf(). Invalid timer\n");
		return 1;
	}

	//Define the read back command
	unsigned long read_back_cmd;
	read_back_cmd = (TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL((unsigned long)timer));


	//Writing rbc to the timer control reg
	if(sys_outb(TIMER_CTRL, read_back_cmd)){
		printf("Error writing(sys_outb) read back cmd!\n");
		return 1;
	}

	unsigned long timer_port;
	if(timer==0) timer_port=TIMER_0;
	else if(timer==1) timer_port=TIMER_1;
	else if(timer==2) timer_port=TIMER_2;

	unsigned long status;

	//Reading from the timer port the response to the rbc
	if(sys_inb(timer_port, &status)){
		printf("Error reading read back response (sys_inb)");
		return 1;
	}

	*st = (unsigned char)status;

	return 0;
}

int timer_display_conf(unsigned char conf){
	
	printf("Timer Status: 0x%x\n\n",conf);
	printf("Now for humans:\n");

	//Print meaning of BIT 7 - The current OUT line value
	if(conf & BIT(7)){
		printf("Current OUT line: 1\n");
	} else {
		printf("Current OUT line: 0\n");
	}

	//Print meaning of BIT 6 - Null Count
	if(conf & BIT(6)){
		printf("Null Count bit ON!\n");
	} else {
		printf("Null Count bit OFF!\n");
	}

	//Print meaning of BIT's 4 and 5 - Access Type
	//TIMER_LSB_MSB must be the first one to be check because of how macros are defined
	if(conf & TIMER_LSB_MSB){
		printf("Type of Access: LSB followed by MSB\n");
	} else if(conf & TIMER_MSB){
		printf("Type of Access: MSB\n");
	} else if(conf & TIMER_LSB){
		printf("Type of Access: LSB\n");
	} else {
		printf("Type of Access: Counter Latch Mode\n"); //if BIT's 4 and 5 are both 0(zero).
	}

	//Print meaning of BIT's 1, 2 and 3 - Counting Mode
	//TMER_SQR_WAVE must be check first because of BIT 3 not mattering and because of how macros are defined
	if(conf & TIMER_SQR_WAVE){
		printf("Counting Mode: 3 - Square Wave Generator\n");
	} else if(conf & TIMER_RATE_GEN){
		printf("Counting Mode: 2 - Rate Generator (divide-by-N counter)\n");
	} else if(conf & TIMER_MODE_FIVE){
		printf("Counting Mode: 5\n");
	} else if(conf & TIMER_MODE_FOUR){
		printf("Counting Mode: 4\n");
	} else if(conf & TIMER_MODE_ONE){
		printf("Counting Mode: 1 - Hardware retriggerable one-shot\n");
	} else {
		printf("Counting Mode: 0- Interrupt on terminal count â€“ for counting events\n");
	}

	//Print meaning of BIT 0 - Counting in either Binary or Binary Converted Decimal
	if(conf & TIMER_BCD){
		printf("Counting in BCD\n");
	} else{
		printf("Counting in Binary\n");
	}

	return 0;
}

int timer_test_time_base(unsigned long freq){

	//Calls timer_set_frequency() forcing timer 0 as required
	if(timer_set_frequency(0,freq))
		return 1;

	return 0;
	}

int timer_test_int(unsigned long time){
	
	int irq_set=timer_subscribe_int();
	int ipc_status;
	message msg;
	int r;

	if(irq_set>=0){
		irq_set=BIT(irq_set);
	} else {
		printf("Error in timer_test_int::timer_subscribe_int()\n");
		return 1;
	}

	//Force timer 0 frequency to 60Hz in case it was changed before
	if(timer_set_frequency(0,TIMER0_DEFAULT_FREQUENCY))
		return 1;

	//We check if the time was negative by considering it overflows, since we can't change lab2.c according to lab monitor
	if(time > 1000000){
		printf("\nPlease input a positive value!\n");
		return 1;
	}

	//"Interrupts cycle"
	while( counter < time*60) { //jumps how of the loop if counter exceeds number of seconds specified by the user
		/* Get a request message. */
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set) { /* subscribed interrupt */

				timer_int_handler();

				if(counter%60 == 0)
					printf("%d second(s) passed!\n",counter/60);
				}
				break;
			default:
				break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
	}

	//Unsubscribe timer 0 interrupts
	if( timer_unsubscribe_int()){
		printf("Unsubscribe failed\n");
		return 1;
	}

	return 0;

}

int timer_test_config(unsigned char timer) {
	
	unsigned char status;

	//Read the select timer configuration
	if(timer_get_conf(timer, &status)){
		printf("Error in timer_test_config::timer_get_conf()\n");
		return 1;
	}

	//Display said configuration in a human friendly way
	printf("\nYou are know reading the configuration of timer %d\n",timer);
	if(timer_display_conf(status)){
		printf("Error in timer_test_config::timer_display_conf()\n");
		return 1;
	}

	return 0;
}
