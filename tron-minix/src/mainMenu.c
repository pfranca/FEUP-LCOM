#include <minix/drivers.h>
#include <minix/syslib.h>
#include <minix/driver.h>
#include <minix/com.h>
#include <minix/sysutil.h>
#include <limits.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <math.h>
#include "video_gr.h"
#include "keyboard.h"
#include "mouse.h"
#include "i8042.h"
#include "timer.h"
#include "i8254.h"
#include "menu.h"
#include "stbi_wrapper.h"

int menuLoop() {


	int timer_irq=timer_subscribe_int();
	int kbd_irq=kbc_subscribe_int();
	int mouse_irq=mouse_subscribe_int();
	int ipc_status;
	message msg;
	int r;
	unsigned long scancode;

	if(kbd_irq>=0){
		kbd_irq=BIT(kbd_irq);
	} else {
		printf("Error subscribing keyboard\n");
		return 1;
	}


	if(timer_irq>=0){
		timer_irq=BIT(timer_irq);
	} else {
		printf("Error subscribing timer\n");
		return 1;
	}

	if(mouse_irq>=0){
		mouse_irq=BIT(mouse_irq);
	} else {
		printf("Error subscribing mouse\n");
		return 1;
	}

	if(vg_init(0x118) == NULL){
		printf("Cannot initialize in graphics mode 0x118");
		return 1;
	}

	//-----------------------------------------------------------


	Menu* menu = newMenu();
	//drawMenu(menu);

	int width, height;
	unsigned char* cursor = stbi_png_load(&width, &height, "/home/lcom/tron-minix/images/seta.png");

	while(scancode!=BREAKCODE_ESC && menu->done == 0) {
		/* Get a request message. */
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */

				if (msg.NOTIFY_ARG & timer_irq) { /* subscribed interrupt - timer */
					drawMenu(menu);
					if( getMouse()->draw == 1){
						drawMouse(cursor, width, height);
					}
					vg_clear();
					updateMenu(menu, scancode);
				}

				if (msg.NOTIFY_ARG & kbd_irq) { /* subscribed interrupt - keyboard */
					scancode=kbc_read();
				}

				if (msg.NOTIFY_ARG & mouse_irq) {
					updateMouse();

				}

				break;
			default:
				break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
						/* no standard messages expected: do nothing */
		}
	}

	vg_exit();

	//Unsubscribe timer 0 interrupts
	if( timer_unsubscribe_int()){
		printf("Unsubscribe timer failed\n");
		return 1;
	}

	//Unsubscribe kbd interrupts
	if(kbc_unsubscribe_int()){
		printf("Unsubscribe keybaord failed\n");
		return 1;
	}

	//Unsubscribe Mouse interrupts
	if(mouse_unsubscribe_int()){
		printf("Unsubscribe mouse failed\n");
		return 1;
	}

	if(menu->done == 2){
		deleteMenu(menu);
		return 1;
	}

	return 0;

}
