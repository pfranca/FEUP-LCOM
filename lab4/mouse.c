#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/driver.h>
#include <minix/com.h>
#include <minix/sysutil.h>
#include "mouse.h"
#include "i8042.h"
#include "keyboard.h"

static int mouse_hook = 2;
unsigned char packet[3]="";
int sync_flag = 0;
int packet_index = 0;
unsigned long byte;

int mouse_subscribe_int(void){

	//Save original hook ID for the irq_set
	int originalHook=mouse_hook;

	//Subscribe mouse interrupts
	if(sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE|IRQ_EXCLUSIVE, &mouse_hook) != OK || sys_irqenable(&mouse_hook) != OK)
		return -1;

	return originalHook;
}

int mouse_unsubscribe_int(){

	//Unsubscribe mouse interrupts
	if(sys_irqdisable(&mouse_hook) != OK || sys_irqrmpolicy(&mouse_hook) != OK)
		return 1;

	return 0;
}

int mouse_enable_data_report(){

	//Write Byte to Mouse Comand to the cmd register of the kbc
	if(kbc_write(KBC_CMD_REG, BYTE_TO_MOUSE)){
		printf("Error in mouse_enable_data_report()-BYTE TO MOUSE\n");
		return 1;
	}

	//Write to the mouse the cmd we want through IN_BUF
	if(kbc_write(IN_BUF, ENABLE_DATA_REPORT)){
		printf("Error in mouse_enable_data_report()-ENABLE DATA REPORT\n");
		return 1;
	}

	//Return the mouse response (ACK,NACK,ERROR)
	return kbc_read();
}

int mouse_disable_data_report(){

	//Write Byte to Mouse Comand to the cmd register of the kbc
	if(kbc_write(KBC_CMD_REG, BYTE_TO_MOUSE)){
		printf("Error in mouse_disable_data_report()-BYTE TO MOUSE\n");
		return 1;
	}

	//Write to the mouse the cmd we want through IN_BUF
	if(kbc_write(IN_BUF, DISABLE_DATA_REPORT)){
		printf("Error in mouse_disable_data_report()-ENABLE DATA REPORT\n");
		return 1;
	}

	//Return the mouse response (ACK,NACK,ERROR)
	return kbc_read();
}

int mouse_set_stream_mode(){

	//Write Byte to Mouse Comand to the cmd register of the kbc
	if(kbc_write(KBC_CMD_REG, BYTE_TO_MOUSE)){
		printf("Error in mouse_set_stream_mode()-BYTE TO MOUSE\n");
		return 1;
	}

	//Write to the mouse the cmd we want through IN_BUF
	if(kbc_write(IN_BUF, SET_STREAM_MODE)){
		printf("Error in mouse_set_stream_mode()-ENABLE DATA REPORT\n");
		return 1;
	}

	//Return the mouse response (ACK,NACK,ERROR)
	return kbc_read();
}

int mouse_set_remote_mode(){

	//Write Byte to Mouse Comand to the cmd register of the kbc
	if(kbc_write(KBC_CMD_REG, BYTE_TO_MOUSE)){
		printf("Error in mouse_set_remote_mode()-BYTE TO MOUSE\n");
		return 1;
	}

	//Write to the mouse the cmd we want through IN_BUF
	if(kbc_write(IN_BUF, SET_REMOTE_MODE)){
		printf("Error in mouse_set_remote_mode()-ENABLE DATA REPORT\n");
		return 1;
	}

	//Return the mouse response (ACK,NACK,ERROR)
	return kbc_read();
}

void mouse_display_packet(unsigned char packet[]){

	short auxiliar;

	if(!((packet[0]&XOVF)||(packet[0]&YOVF))){

		printf("B1=0x%X ", packet[0]);
		printf("B2=0x%X ", packet[1]);
		printf("B3=0x%X ", packet[2]);
		printf("LB=%X ", (packet[0] & LB));
		printf("MB=%X ", (packet[0] & MB)>>2);
		printf("RB=%X ", (packet[0] & RB)>>1);
		printf("XOV=%X ", (packet[0] & XOVF)>>6);
		printf("YOV=%X ", (packet[0] & YOVF)>>7);

		if(packet[0]&XSIGN){
			auxiliar = (~(packet[1]) + 255);

			if(auxiliar == -1){
				printf("X=-  1"); //normalize case of kbc error
			} else{
				printf("X=-%3d ", auxiliar);
			}
		}else{
			printf("X=%3d ", packet[1]);
		}

		if(packet[0]&YSIGN){
			auxiliar = (~(packet[2]) + 255);

			if(auxiliar == -1){
				printf("Y=-  1\n\n"); //normalize case of kbc error
			} else{
				printf("Y=-%3d\n\n", auxiliar);
			}
		}else{
			printf("Y=%3d\n\n", packet[2]);
		}
	}
}

// Gets the packets from the mouse and print them on the screen

int mouse_get_packet(){

	byte = kbc_read();

	packet[packet_index] = byte;

	if((packet[0] & BIT(3))==BIT(3)){
		sync_flag = 1;
	} else {
		sync_flag = 0;
		packet_index = 0;
	}
	if(sync_flag){
		packet_index++;
		if(packet_index>2){
			packet_index = 0;
			mouse_display_packet(packet);
			return 1;
		}
	}
	return 0;
}

//State Machine

void check_for_line(state_t *stat, ev_type_t evt){

	switch(*stat) {
	case INIT:
		if(evt == EV_RDOW){
			*stat = DRAW;
		}
		break;
	case DRAW:
		if(evt == EV_DIAGONALMOVE){
			*stat = DRAW;
		} else if(evt == EV_RESETLEN){
			*stat= DRAW;
		} else if(evt == EV_RUP){
			*stat = INIT;
		} else if(evt==EV_EXIT){
			*stat = COMP;
		}
		break;
	default:
		break;
	}
}





