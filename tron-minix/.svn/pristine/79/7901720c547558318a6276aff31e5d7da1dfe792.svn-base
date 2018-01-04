#include <stdlib.h>
#include "menu.h"
#include "video_gr.h"
#include "mouse.h"
#include "i8042.h"
#include "stbi_wrapper.h"

int winner = 0;


Menu* newMenu() {
	Menu* state = (Menu*) malloc(sizeof(Menu));

	state->done = 0;

	state->background = stbi_png_load(&state->background_width, &state->background_height, "/home/lcom/tron-minix/images/background.png");
	state->play = stbi_png_load(&state->play_width, &state->play_height, "/home/lcom/tron-minix/images/play.png");
	state->bwinner = stbi_png_load(&state->bwinner_width, &state->bwinner_height, "/home/lcom/tron-minix/images/bluewins.png");
	state->wwinner = stbi_png_load(&state->wwinner_width, &state->wwinner_height, "/home/lcom/tron-minix/images/whitewins.png");
	state->exit = stbi_png_load(&state->exit_width, &state->exit_height, "/home/lcom/tron-minix/images/exit.png");

	return state;
}

void updateMenu(Menu* state, unsigned long scan_code){

	if(scan_code == BREAKCODE_ESC){
		state->done =1;
		state->action =EXIT_GAME;
	}
	unsigned x1 = 400;
	unsigned x2= 600;
	unsigned y1 = 400;
	unsigned y2= 480;

	// checks if mouse inside Play Button
	if(checkInside(x1,x2,y1,y2) && getMouse()->leftButtonReleased){
		state->action = PLAY_GAME;
		state->done = 1;
		getMouse()->leftButtonReleased = 0;

	}

	x1 = 400;
	x2 = 600;
	y1= 600;
	y2= 680;

	// checks if mouse inside Exit Button
	if(checkInside(x1,x2,y1,y2) && getMouse()->leftButtonReleased) {
		state->action = EXIT_GAME;
		state->done =2;
		getMouse()->leftButtonReleased =0;
	}
}

void drawMenu(Menu* state) {

	vg_png(state->background, state->background_width, state->background_height,0 , 100);
	vg_png(state->play, state->play_width, state->play_height, 400, 400);
	if(winner==1){
	vg_png(state->bwinner, state->bwinner_width, state->bwinner_height, 400, 500);
	}
	if(winner==2){
	vg_png(state->wwinner, state->wwinner_width, state->wwinner_height, 400, 500);
	}
	vg_png(state->exit, state->exit_width, state->exit_height, 400, 600);
}

void deleteMenu(Menu* state) {

	vg_free(); //Free buffer used for double buffering
	stbi_free(state->background);
	stbi_free(state->play); //Free image, lib has its own free
	stbi_free(state->bwinner);
	stbi_free(state->wwinner);
	stbi_free(state->exit);

	free(state);
}
