#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/driver.h>
#include <minix/com.h>
#include <minix/sysutil.h>
#include "mouse.h"
#include "i8042.h"
#include "keyboard.h"
#include "video_gr.h"

static int mouse_hook = 2;
unsigned char packet[3]="";
int sync_flag = 0;
int packet_index = 0;
unsigned long byte;

Mouse* mouse = NULL;

Mouse* newMouse() {
	Mouse* mouse = (Mouse*) malloc(sizeof(Mouse));

	mouse->x = 0;
	mouse->y = 0;

	mouse->packet_idx = 0;
	mouse->complete_packet = 0;

	mouse->leftButtonDown = 0;
	mouse->leftButtonReleased = 0;

	mouse->draw =1;

	return mouse;

}

Mouse* getMouse() {
	if (!mouse) {
		mouse_enable_data_report();
		mouse = newMouse();
	}

	return mouse;
}

void updateMouse() {

	unsigned long b1, b2, b3;

	packet_handler();

	if (mouse->complete_packet) {
		b1 = mouse->packet[0];
		b2 = mouse->packet[1];
		b3 = mouse->packet[2];

		if ((b1 & XSIGN) != 0) {
			b2 = (0xFF & ~(0xFF & b2)); //change to FF macro
			b2 += 1;
			b2 = -b2;
		}

		if ((b1 & YSIGN) != 0) {
			b3 = (0xFF & ~(0xFF & b3));
			b3 += 1;
			b3 = -b3;
		}

		mouse->x += b2;
		mouse->y -= b3;

		if (mouse->x >= getHorResolution())
			mouse->x = getHorResolution() - 10;
		else if (mouse->x < 0)
			mouse->x = 0;

		if (mouse->y >= getVerResolution())
			mouse->y = getVerResolution() - 10;
		else if (mouse->y < 0)
			mouse->y = 0;

		if (mouse->packet[0] & BIT(0)) { //change  to macro

			mouse->leftButtonDown = 1;
			mouse->leftButtonReleased = 0;

		} else if (mouse->leftButtonDown) {

			mouse->leftButtonDown = 0;
			mouse->leftButtonReleased = 1;

		} else {
			mouse->leftButtonDown = 0;
			mouse->leftButtonReleased = 0;
		}

	}

}

int checkInside(unsigned xi, unsigned xf, unsigned yi, unsigned yf) {
	if ((mouse->x >= xi) && (mouse->x <= xf))
		if ((mouse->y >= yi) && (mouse->y <= yf))
			return 1;

	return 0;
}

void drawMouse(unsigned char * cursor, int width, int height) {

	vg_png(cursor, width, height, mouse->x, mouse->y);
	vg_copy();
}

void deleteMouse() {
	free(getMouse());
}

void packet_handler() {

	unsigned long byte;

	sys_inb(OUT_BUF, &byte);

	switch (mouse->packet_idx) {
	case 0:
		if (byte & BIT(3)) {
			mouse->packet[mouse->packet_idx] = byte;
			mouse->packet_idx = 1;
			mouse->complete_packet = 0;
		}
		break;

	case 1:
		mouse->packet[mouse->packet_idx] = byte;
		mouse->packet_idx = 2;
		mouse->complete_packet = 0;
		break;

	case 2:
		mouse->packet[mouse->packet_idx] = byte;
		mouse->packet_idx = 0;
		mouse->complete_packet = 1;
		break;

	};

}

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
			return 1;
		}
	}
	return 0;
}
