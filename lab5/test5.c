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
#include "video_test.h"
#include "video_gr.h"
#include "keyboard.h"
#include "i8042.h"
#include "timer.h"
#include "i8254.h"
#include "read_xpm.h"



int video_test_init(unsigned short mode, unsigned short delay) {


	if(vg_init(mode) == NULL){
		printf("Cannot initialize in graphics mode 0x105");
		return 1;
	}

	sleep(delay);

	vg_exit();

	return 0;

}

int video_test_square(unsigned short x, unsigned short y, unsigned short size, unsigned long color) {

	//TODO:validar inputs

	int kbd_irq=kbc_subscribe_int();
	int ipc_status;
	message msg;
	int r;
	unsigned long scancode;

	if(kbd_irq>=0){
		kbd_irq=BIT(kbd_irq);
	} else {
		printf("Error in video_test_square::kbc_subscribe_int()\n");
		return 1;
	}

	if(vg_init(0x105) == NULL){
		printf("Cannot initialize in graphics mode 0x105");
		return 1;
	}


	vg_draw_centered_square(x,y,size,color);
	video_dump_fb();


	while(scancode != BREAKCODE_ESC) {
		/* Get a request message. */
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & kbd_irq) { /* subscribed interrupt */

						scancode=kbc_read();

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

	//Unsubscribe kbd interrupts
	if(kbc_unsubscribe_int()){
		printf("Unsubscribe failed\n In kbd_test_scan::kbc_unsubscribe_int()\n");
		return 1;
	}

	return 0;

}

int video_test_line(unsigned short xi, unsigned short yi, unsigned short xf, unsigned short yf, unsigned long color) {

	//TODO:validar inputs

	int kbd_irq=kbc_subscribe_int();
	int ipc_status;
	message msg;
	int r;
	unsigned long scancode;

	if(kbd_irq>=0){
		kbd_irq=BIT(kbd_irq);
	} else {
		printf("Error in video_test_square::kbc_subscribe_int()\n");
		return 1;
	}

	if(vg_init(0x105) == NULL){
		printf("Cannot initialize in graphics mode 0x105");
		return 1;
	}
//-----------------------------------------------------------------------------

	vg_draw_line(xi, yi, xf, yf,color);

	video_dump_fb();
//-----------------------------------------------------------------------------
	while(scancode != BREAKCODE_ESC) {
		/* Get a request message. */
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & kbd_irq) { /* subscribed interrupt */

						scancode=kbc_read();

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

	//Unsubscribe kbd interrupts
	if(kbc_unsubscribe_int()){
		printf("Unsubscribe failed\n In kbd_test_scan::kbc_unsubscribe_int()\n");
		return 1;
	}

	return 0;

}

int video_test_xpm(char *xpm[], unsigned short xi, unsigned short yi) {

	int kbd_irq=kbc_subscribe_int();
	int ipc_status;
	message msg;
	int r;
	unsigned long scancode;

	if(kbd_irq>=0){
		kbd_irq=BIT(kbd_irq);
	} else {
		printf("Error in video_test_square::kbc_subscribe_int()\n");
		return 1;
	}


	if(vg_init(0x105) == NULL){
		printf("Cannot initialize in graphics mode 0x105");
		return 1;
	}

	vg_draw_xpm(xpm, xi, yi); //verify error
	video_dump_fb();



	while(scancode != BREAKCODE_ESC) {
		/* Get a request message. */
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & kbd_irq) { /* subscribed interrupt */

						scancode=kbc_read();

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

	//Unsubscribe kbd interrupts
	if(kbc_unsubscribe_int()){
		printf("Unsubscribe failed\n In kbd_test_scan::kbc_unsubscribe_int()\n");
		return 1;
	}

	return 0;

}

int video_test_move(char *xpm[], unsigned short xi, unsigned short yi, unsigned short xf, unsigned short yf, short s, unsigned short f) {


	int timer_irq=timer_subscribe_int();
	int kbd_irq=kbc_subscribe_int();
	int ipc_status;
	message msg;
	int r;
	int counter = 0;
	unsigned long scancode;
	int horizontal;

	if(xi != xf && yi == yf)
		horizontal = 1;
	else if(xi == xf && yi != yf)
			horizontal = 0;
		else{
			printf("Invalid x and y values, it must be a vertical or horizontal move\n");
			return 1;
		}

	if(kbd_irq>=0){
		kbd_irq=BIT(kbd_irq);
	} else {
		printf("Error in video_test_square::kbc_subscribe_int()\n");
		return 1;
	}


	if(timer_irq>=0){
		timer_irq=BIT(timer_irq);
	} else {
		printf("Error in timer_test_int::timer_subscribe_int()\n");
		return 1;
	}
	//-----------------------------------------------------------


	if(vg_init(0x105) == NULL){
		printf("Cannot initialize in graphics mode 0x105");
		return 1;
	}

	if(f < 20){
		printf("Frequency cannot be less than 20\n");
		return 1;
	}


	timer_set_frequency(TIMER0_IRQ,f);

	int width, height;
	char *map;
	// get the pix map from the XPM
	map = read_xpm(xpm, &width, &height);


	//--------------------------------------------
	// EXIT ON ESC
	//------------------------------------------------------------

	while( scancode != BREAKCODE_ESC) {
		/* Get a request message. */
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */


				if (msg.NOTIFY_ARG & timer_irq) { /* subscribed interrupt - timer */


					if(s > 0){
						if(xi < xf || yi < yf){
							if(horizontal)
								xi += s;
							else yi += s;
						}else{
							if(horizontal)
								xi -= s;
							else yi -= s;

						}
					}
					else{
						counter++;
						if(counter%abs(s)==0){
							if(xi < xf || yi < yf){
								if(horizontal)
									xi ++;
								else yi ++;
							}
							else{
								if(horizontal)
									xi --;
								else yi --;
							}
						}
					}

					vg_draw(map, height, width, xi, yi);
				}

				if (msg.NOTIFY_ARG & kbd_irq) { /* subscribed interrupt - keyboard */
					scancode=kbc_read();
				}

				break;
			default:
				break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
						/* no standard messages expected: do nothing */
		}
	}

	video_dump_fb();


	vg_exit();

	//Unsubscribe timer 0 interrupts
	if( timer_unsubscribe_int()){
		printf("Unsubscribe failed\n In kbd_test_timed_scan::timer_unsubscribe_int()\n");
		return 1;
	}

	//Unsubscribe kbd interrupts
	if(kbc_unsubscribe_int()){
		printf("Unsubscribe failed\n In kbd_test_scan::kbc_unsubscribe_int()\n");
		return 1;
	}

	return 0;

}

int video_test_controller() {

	/* To be completed */
	return 0;

}
