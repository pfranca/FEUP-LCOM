#include <stdio.h>
#include <string.h>
#include "bike.h"
#include "i8042.h"


int getInitialX(Bike bike){
	return bike.initial_x_pos;
}

int getInitialY(Bike bike){
	return bike.initial_y_pos;
}

int getDirection(Bike bike){
	return bike.direction;
}

int getHeadX(Bike bike){
	return bike.head_x_pos;
}

int getHeadY(Bike bike){
	return bike.head_y_pos;
}

unsigned long getcolor(Bike bike){
	return bike.color;
}

void setDirection(Bike *bike, int dir){
	bike->direction = dir;
}

void moveHead(Bike *bike, int speed){
	if(getDirection(*bike) == UP){
		bike->head_y_pos = bike->head_y_pos - speed;
	}
	if(getDirection(*bike) == RIGHT){
		bike->head_x_pos = bike->head_x_pos + speed;
	}
	if(getDirection(*bike) == DOWN){
		bike->head_y_pos = bike->head_y_pos + speed;
	}
	if(getDirection(*bike) == LEFT){
		bike->head_x_pos = bike->head_x_pos - speed;
	}
}
