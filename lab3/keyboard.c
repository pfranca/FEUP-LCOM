#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include <minix/sysutil.h>
#include "keyboard.h"
#include "i8042.h"


static int kbc_hook = 1;
static unsigned int sysinb_counter=0;
static int twobyteflag = 0;

void print_and_reset_sys_counter(){
	printf("Number of sys_inb() used: %d\n",sysinb_counter);
	sysinb_counter=0;
}

int sys_inb_cnt(unsigned long porto, unsigned long *reg){
	sysinb_counter++;
	if(sys_inb(porto,reg)){
		printf("Error in sys_inb()");
		return 1;
	}
	return 0;
}

int kbc_subscribe_int(void){

	//Save original hook ID for the irq_set
	int originalHook=kbc_hook;

	//Subscribe kbc interrupts
	if(sys_irqsetpolicy(KBC_IRQ, IRQ_REENABLE|IRQ_EXCLUSIVE, &kbc_hook) != OK || sys_irqenable(&kbc_hook) != OK)
		return -1;

	return originalHook;
}

int kbc_unsubscribe_int(){

	//Unsubscribe kbc interrupts
	if(sys_irqdisable(&kbc_hook) != OK || sys_irqrmpolicy(&kbc_hook) != OK)
		return 1;

	return 0;
}


int kbc_read(){

	int timeout=0;
	unsigned long stat;
	unsigned long data;

	while( timeout<TIMEOUT ) {
		if(sys_inb_cnt(STAT_REG, &stat)){
			printf("Error in kbc_write::sys_inb_cnt()");
			return 1;
		}
		/* loop while 8042 output buffer is empty */
		if( stat & OBF ) {
			if(sys_inb_cnt(OUT_BUF, &data)){
				printf("Error in kbc_write::sys_inb_cnt()");
				return 1;
			}

			if ( (stat &(PAR_ERR | TO_ERR)) == 0 ){
				tickdelay(micros_to_ticks(DELAY_US));
				return data;
			}else{
				tickdelay(micros_to_ticks(DELAY_US));
				return 0xFFF;
			}

			tickdelay(micros_to_ticks(DELAY_US));
			timeout++;
		}

	}
	return 0XFFF;
}

void display_scancode(unsigned long scancode){

	if(scancode == TOW_BYTE_CODE){
		twobyteflag=1;
	}else if((twobyteflag==1)){
		twobyteflag=0;
		if(scancode & BREAKCODE){
			printf("BREAKcode: 0xE0 0x%02X\n",scancode);
		} else{
			printf("MAKEcode: 0xE0 0x%02X\n",scancode);
		}
	}

	else if((scancode & BREAKCODE)){
		printf("BREAKcode: 0x%02X\n",scancode);
	}
	else{
		printf("MAKEcode: 0x%02X\n",scancode);
	}
}

int kbc_write(unsigned long porto, unsigned long cmd){

	int timeout=0;
	unsigned long stat;

	while( timeout<TIMEOUT ) {
		if(sys_inb_cnt(STAT_REG, &stat)){
			printf("Error in kbc_write::sys_inb_cnt()");
			return 1;
		}
		/* loop while 8042 input buffer is not empty */
		if( (stat & IBF) == 0 ) {
			if(sys_outb(porto, cmd)){
				printf("Error in kbc_write::sys_outb()\n");
				return 1;
			}
			return 0;
		}
		tickdelay(micros_to_ticks(DELAY_US));
	}

	return 1;
}

