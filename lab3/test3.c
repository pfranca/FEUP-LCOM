#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/driver.h>
#include <minix/com.h>
#include <minix/sysutil.h>
#include "test3.h"
#include "keyboard.h"
#include "i8042.h"
#include "i8254.h"
#include "timer.h"

int counter;
static unsigned long scancode=0;

int kbd_test_scan(unsigned short ass) {

	int irq_set=kbc_subscribe_int();
	int ipc_status;
	message msg;
	int r;


	if(irq_set>=0){
		irq_set=BIT(irq_set);
	} else {
		printf("Error in kbd_test_scan::kbc_subscribe_int()\n");
		return 1;
	}


	while(scancode != BREAKCODE_ESC) {
		/* Get a request message. */
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set) { /* subscribed interrupt */
					if(ass==0){ //Use IH written in C
						scancode=kbc_read();
						if(scancode!=0xFFF){
							display_scancode(scancode);
							if(scancode == BREAKCODE_ESC)
								printf("BreakCode of ESC read! EXITING!\n");
						} else {
							printf("Error in kbd_test_poll()\nInvalid Scancode red from kbc()\n");
						return 1;
						}
					}else{ //Use IH written in assembly code
						scancode=asm_handler();
						display_scancode(scancode);
						if(scancode == BREAKCODE_ESC)
							printf("BreakCode of ESC read! EXITING!\n");
					}
				}

				break;
			default:
				break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
					/* no standard messages expected: do nothing */
		}
	}

	//Unsubscribe kbc interrupts
	if(kbc_unsubscribe_int()){
		printf("Unsubscribe failed\n In kbd_test_scan::kbc_unsubscribe_int()\n");
		return 1;
	}

	if(ass==0)
		print_and_reset_sys_counter();


	return 0;
}




int kbd_test_poll() {

	unsigned long cmd_byte;


	while(scancode != BREAKCODE_ESC){
		scancode=kbc_read();

		if(scancode!=0xFFF)
			display_scancode(scancode);

		if(scancode == BREAKCODE_ESC)
			printf("BreakCode of ESC read! EXITING!\n");
	}

	print_and_reset_sys_counter();

	//enable interrupts again after esc breakcode is read
	kbc_write(KBC_CMD_REG, READ_COMMAND_BYTE);
	cmd_byte = kbc_read();
	kbc_write(KBC_CMD_REG, WRITE_COMMAND_BYTE);
	kbc_write(IN_BUF, (cmd_byte | BIT(0)));

	return 0;

}

int kbd_test_timed_scan(unsigned short n) {

	int timer_irq=timer_subscribe_int();
	int kbc_irq=kbc_subscribe_int();
	int ipc_status;
	message msg;
	int r;

	if(kbc_irq>=0){
		kbc_irq=BIT(kbc_irq);
	} else {
		printf("Error in kbd_test_scan::kbc_subscribe_int()\n");
		return 1;
	}

	if(timer_irq>=0){
		timer_irq=BIT(timer_irq);
	} else {
		printf("Error in timer_test_int::timer_subscribe_int()\n");
		return 1;
	}

	//Force timer 0 frequency to 60Hz in case it was changed before
	if(timer_set_frequency(0,TIMER0_DEFAULT_FREQUENCY))
		return 1;

	//We check if the time was negative by considering it overflows, since we can't change lab2.c according to lab monitor
	if(n > 1000000){
		printf("\nPlease input a positive value!\n");
		return 1;
	}

	while((scancode != BREAKCODE_ESC ) && (counter < n*60)) { //breaks out of loop if break code of ESC is red or time specified passes
		/* Get a request message. */
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & timer_irq) { /* subscribed interrupt */

					//increment counter
					timer_int_handler();
					if(counter==n*60)
						printf("%d seconds have passed since last imput! EXITING!\n",n);

				}

				if (msg.NOTIFY_ARG & kbc_irq) { /* subscribed interrupt */

					//reset counter upon recieving a kbc interrupt
					reset_counter();
					//read kbc out_buf upon recieving a kbc interrupt which should be a scan code and saves it in scancode
					scancode=kbc_read();
					//displays scan code read from kbc out_buf
					display_scancode(scancode);
					if(scancode == BREAKCODE_ESC)
						printf("BreakCode of ESC read! EXITING!\n");

				}

				break;
			default:
				break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
	}

	//Unsubscribe kbc interrupts
	if(kbc_unsubscribe_int()){
		printf("Unsubscribe failed\n In kbd_test_timed_scan::kbc_unsubscribe_int()\n");
		return 1;
	}

	//Unsubscribe timer 0 interrupts
	if( timer_unsubscribe_int()){
		printf("Unsubscribe failed\n In kbd_test_timed_scan::timer_unsubscribe_int()\n");
		return 1;
	}

return 0;

}
