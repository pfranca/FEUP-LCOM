#include <minix/drivers.h>
#include <limits.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdint.h>
#include "tron.h"
#include "mainMenu.h"

int main()
{
	/* DO NOT FORGET TO initialize service */
	sef_startup();

	int repeat = 1;

	while(repeat){
		if(menuLoop() == 0)
			playTron(5,30);
		else
			repeat=0;
	}

	return 0;
}


