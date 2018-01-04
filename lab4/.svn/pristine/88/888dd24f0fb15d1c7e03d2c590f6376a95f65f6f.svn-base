#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/driver.h>
#include <minix/com.h>
#include <minix/sysutil.h>
#include "test4.h"
#include "keyboard.h"
#include "i8042.h"
#include "i8254.h"
#include "timer.h"
#include "mouse.h"

int mouse_test_packet(unsigned short cnt){


	//Subscribe Mouse interrupts
	int irq_set=mouse_subscribe_int();
	int ipc_status;
	message msg;
	int r;


	unsigned short nrpackets = 0;
	unsigned long ps2;

	if(irq_set>=0){
		irq_set=BIT(irq_set);
	} else {
		printf("Error in mouse_test_packet::mouse_subscribe_int()\n");
		return 1;
	}

	//Set stream mode, if it's not already for some reason
	do{
		ps2 = mouse_set_stream_mode();
	}while(ps2 != ACK);

	//Enable data report
	do{
		ps2 = mouse_enable_data_report();
	}while(ps2 != ACK);

	while(nrpackets < cnt) {

			if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
				printf("driver_receive failed with: %d", r);
				continue;
			}
			if (is_ipc_notify(ipc_status)) {
				switch (_ENDPOINT_P(msg.m_source)) {
				case HARDWARE:
					if (msg.NOTIFY_ARG & irq_set) {

						if(mouse_get_packet()){
							nrpackets++;
						}
					}
					break;
				default:

					break;
				}
			} else {

			}
	}

	//Disable data report
	do{
		ps2 = mouse_disable_data_report();
	}while(ps2 != ACK);

	//Unsubscribe Mouse interrupts
	if(mouse_unsubscribe_int()){
		printf("Unsubscribe failed\n In mouse_test_packet::mouse_unsubscribe_int()\n");
		return 1;
	}

	//Clear OUT_BUF in case something was left in there...
	kbc_read();

	return 0;
}

int mouse_test_async(unsigned short idle_time){

	int timer_irq=timer_subscribe_int();
	int mouse_irq=mouse_subscribe_int();
	int ipc_status;
	message msg;
	int r;

	unsigned long ps2;

	if(mouse_irq>=0){
		mouse_irq=BIT(mouse_irq);
	} else {
		printf("Error in mouse_test_async::mouse_subscribe_int()\n");
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

	//We check if the time was negative by considering it overflows.
	if(idle_time > 1000000){
		printf("\nPlease input a positive value!\n");
		return 1;
	}

	//Set stream mode, if it's not already for some reason
	do{
		ps2 = mouse_set_stream_mode();
	}while(ps2 != ACK);

	//Enable data report
	do{
		ps2 = mouse_enable_data_report();
	}while(ps2 != ACK);

	while(counter<idle_time*60) {

		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & timer_irq) { /* subscribed interrupt */

					//increment counter
					timer_int_handler();
					if(counter==idle_time*60)
						printf("%d seconds have passed since last mouse status change! EXITING!\n",idle_time);

				}

				if (msg.NOTIFY_ARG & mouse_irq) {

					if(mouse_get_packet())
						reset_counter();

				}
				break;
			default:
				break;
			}
		} else {

		}
	}

	//Disable data report
	do{
		ps2 = mouse_disable_data_report();
	}while(ps2 != ACK);

	//Unsubscribe timer 0 interrupts
	if( timer_unsubscribe_int()){
		printf("Unsubscribe failed\n In kbd_test_timed_scan::timer_unsubscribe_int()\n");
		return 1;
	}

	//Unsubscribe Mouse interrupts
	if(mouse_unsubscribe_int()){
		printf("Unsubscribe failed\n In mouse_test_packet::mouse_unsubscribe_int()\n");
		return 1;
	}

    return 0;
}

int mouse_test_remote(unsigned long period, unsigned short cnt){
	unsigned long cmd_byte;
	unsigned long aux;
	unsigned short nrpackets = 0;

	//Disable Mouse Interrupts
	kbc_write(KBC_CMD_REG, READ_COMMAND_BYTE);
	cmd_byte = kbc_read();
	kbc_write(KBC_CMD_REG, WRITE_COMMAND_BYTE);
	kbc_write(IN_BUF, (cmd_byte & ~BIT(1)));

	//Disable data report
	do{
		aux = mouse_disable_data_report();
	}while(aux != ACK);

	//Set mouse to remote mode
	do{
		aux = mouse_set_remote_mode();
	}while(aux != ACK);

	while(nrpackets < cnt){

		//Write Byte to mouse command to the kbc cmd register
		if(kbc_write(KBC_CMD_REG, BYTE_TO_MOUSE)){
			printf("Error in mouse_test_remote()-BYTE TO MOUSE\n");
			return 1;
		}

		//Sent read data comand to the mouse by writting it to the kbc IN_BUF and reading the ACK
		do{
			kbc_write(IN_BUF, READ_DATA);
			aux = kbc_read();
		}while(aux != ACK);

		//Read the 3 data bytes in response to the read data comand and display the packet
		packet[0] = kbc_read();
		packet[1] = kbc_read();
		packet[2] = kbc_read();
		mouse_display_packet(packet);
		nrpackets++;
		tickdelay(micros_to_ticks(period*1000));
	}

	//Set the mouse back to stream mode
	do{
		aux = mouse_set_stream_mode();
	}while(aux != ACK);



	//Enable Mouse Interrupts
	kbc_write(KBC_CMD_REG, WRITE_COMMAND_BYTE);
	kbc_write(IN_BUF, ENABLE_MOUSE_INT);

    return 0;
}	

int mouse_test_gesture(short length){

	//Subscribe Mouse interrupts
	int irq_set=mouse_subscribe_int();
	int ipc_status;
	message msg;
	int r;

	long dist = 0;
	static state_t state = INIT;
	unsigned long aux;

	if(irq_set>=0){
		irq_set=BIT(irq_set);
	} else {
		printf("Error in mouse_test_packet::mouse_subscribe_int()\n");
		return 1;
	}

	//Set stream mode, if it's not already for some reason
	do{
		aux = mouse_set_stream_mode();
	}while(aux != ACK);

	//Enable data report
	do{
		aux = mouse_enable_data_report();
	}while(aux != ACK);

	while(state!=COMP){

		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set) {


					if(mouse_get_packet()){
						// Ignoring Overflow values
						if(!((packet[0]&XOVF)||(packet[0]&YOVF))){
							// Right Button is pressed
							if((packet[0]&RB)){
								check_for_line(&state, EV_RDOW);
							}
							// Right Button is not pressed
							if(!((packet[0]&RB)>>1)){
								check_for_line(&state, EV_RUP);
							}
							// Drawing and the direction is inverted
							if((state==DRAW) && (packet[0]&XSIGN)){
								dist=0;
								check_for_line(&state, EV_RESETLEN);
							}
							// Drawing in the right direction
							if((state==DRAW) && (packet[1]>0)){
								if(!(packet[1]&YOVF)){
									dist += packet[1];
								}
								check_for_line(&state, EV_DIAGONALMOVE);
								//printf("Distance = %d\n",dist);
							}
							// Stop drawing and achieve the length pretended
							if((state==DRAW) && (dist>length)){
								check_for_line(&state, EV_EXIT);
								//printf("Total Distance = %d\n",dist);
								printf("Exiting line detected during required length. EXITING!\n");
							}
						}
					}
				}
				break;
			default:
				break;
			}

		} else {

		}
	}


	//Disable data report
	do{
		aux = mouse_disable_data_report();
	}while(aux != ACK);

	//Unsubscribe Mouse interrupts
	if(mouse_unsubscribe_int()){
		printf("Unsubscribe failed\n In mouse_test_packet::mouse_unsubscribe_int()\n");
		return 1;
	}

	//Clear OUT_BUF in case something was left in there...
	kbc_read();

	return 0;
}
