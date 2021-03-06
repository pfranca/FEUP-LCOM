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
#include "bike.h"

extern int winner;


int checkScreenBorder(Bike bike){

	if( (getHeadY(bike) < 15) || (getHeadY(bike) > V_RES-15) ){
		return 1;
	}

	if( (getHeadX(bike) < 9) || (getHeadX(bike) > H_RES-7) ){
		return 1;
	}

	return 0;
}

int checkColision(Bike bike){
	int direction = getDirection(bike);
	unsigned long color;

	switch(direction){

	case (UP):
		color = vg_getColor(getHeadX(bike),getHeadY(bike)-7);
		if(color != 0){
			return 1;
		}else{
			return 0;
		}
		break;
	case (RIGHT):
		color = vg_getColor(getHeadX(bike)+7,getHeadY(bike));
		if(color != 0){
			return 1;
		}else{
			return 0;
		}
		break;
	case (DOWN):
		color = vg_getColor(getHeadX(bike),getHeadY(bike)+7);
		if(color != 0){
			return 1;
		}else{
			return 0;
		}
		break;
	case (LEFT):
		color = vg_getColor(getHeadX(bike)-7,getHeadY(bike));
		if(color != 0){
			return 1;
		}else{
			return 0;
		}
		break;
	}
	return 0;
}

int playTron() {


	int timer_irq=timer_subscribe_int();
	int kbd_irq=kbc_subscribe_int();
	int ipc_status;
	message msg;
	int r;
	unsigned long scancode;
	int over = 0;

	Bike player1={10,V_RES/2,RIGHT,10,V_RES/2,BLUE};
	Bike player2={H_RES-10,V_RES/2,LEFT,H_RES-10,V_RES/2, WHITE};

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


	if(vg_init(0x105) == NULL){
		printf("Cannot initialize in graphics mode 0x105");
		return 1;
	}

	while( over == 0 ) {
		/* Get a request message. */
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */

				if (msg.NOTIFY_ARG & timer_irq) { /* subscribed interrupt - timer */

						moveHead(&player1,5);
						moveHead(&player2,5);

						if( checkScreenBorder(player1) ){
							winner = 2;
							over = 1;
						}

						if(checkScreenBorder(player2)){
							winner = 1;
							over = 1;
						}

						if( checkColision(player1) ){
							winner = 2;
							over = 1;
						}

						if( checkColision(player2) ){
							winner = 1;
							over = 1;
						}

						vg_draw_bike(getHeadX(player1), getHeadY(player1), BLUE);
						vg_draw_bike(getHeadX(player2), getHeadY(player2), WHITE);

				}

				if (msg.NOTIFY_ARG & kbd_irq) { /* subscribed interrupt - keyboard */
					scancode=kbc_read();

						if((scancode == MAKECODE_W) && (getDirection(player1) != DOWN)){
							setDirection(&player1,UP);
						}
						if((scancode == MAKECODE_A) && (getDirection(player1) != RIGHT)){
							setDirection(&player1,LEFT);
						}
						if((scancode == MAKECODE_S) && (getDirection(player1) != UP)){
							setDirection(&player1,DOWN);
						}
						if((scancode == MAKECODE_D) && (getDirection(player1) != LEFT)){
							setDirection(&player1,RIGHT);
						}
						if((scancode == MAKECODE_UP_NUMPAD8) && (getDirection(player2) != DOWN)){
							setDirection(&player2,UP);
						}
						if((scancode == MAKECODE_LEFT_NUMPAD4) && (getDirection(player2) != RIGHT)){
							setDirection(&player2,LEFT);
						}
						if((scancode == MAKECODE_DOWN_NUMPAD2) && (getDirection(player2) != UP)){
							setDirection(&player2,DOWN);
						}
						if((scancode == MAKECODE_RIGHT_NUMPAD6) && (getDirection(player2) != LEFT)){
							setDirection(&player2,RIGHT);
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
	vg_exit();

	//Unsubscribe timer 0 interrupts
	if( timer_unsubscribe_int()){
		printf("timer Unsubscribe failed\n");
		return 1;
	}

	//Clear OUT_BUF in case something was left in there...
	kbc_read();


	//Unsubscribe kbd interrupts
	if(kbc_unsubscribe_int()){
		printf("keyboard Unsubscribe failed\n ");
		return 1;
	}

	return 0;
}





