#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>

#include "video_gr.h"

#include "vbe.h"

#include "video.h"
#include "lmlib.h"

#include <math.h>
#include <stdint.h>
/* Constants for VBE 0x105 mode */

/* The physical address may vary from VM to VM.
 * At one time it was 0xD0000000
 *  #define VRAM_PHYS_ADDR    0xD0000000
 * Currently on lab B107 is 0xF0000000
 * Better run my version of lab5 as follows:
 *     service run `pwd`/lab5 -args "mode 0x105"
 */

/* Private global variables */



static phys_bytes video_phys;	//Physical address for VRAM
static char* video_mem;			//Virtual address for VRAM
static char* double_buffer;		//Buffer to use for double buffering

static uint16_t h_res;			//Horizontal screen resolution in pixels
static uint16_t v_res;			//Vertical screen resolution in pixels
static uint8_t bits_per_pixel; //Number of bits per pixel to represent color in VRAM





void* vg_init(unsigned short mode) {

	//Struct to store info from calling VBE Function 01h
	vbe_mode_info_t info;

	//Call VBE function 01h
	if(vbe_get_mode_info(mode, &info) != 0) {
		return NULL;
	}

	//Initialize values for the mode specified
	h_res = info.XResolution;
	v_res = info.YResolution;
	bits_per_pixel = info.BitsPerPixel;
	video_phys = info.PhysBasePtr;

	//Call VBE Function 02h to set display mode
	struct reg86u reg86;

	reg86.u.w.ax = SET_VBE_MODE;
	reg86.u.w.bx = LINEAR_FRAME | mode;
	reg86.u.b.intno = BIOS_VIDEO;

	if(sys_int86(&reg86) != OK) {
		printf("vga: vg_init: sys_int86() failed\n");
		return NULL;
	}

	int r;
	struct mem_range mr;

	int vram_size = h_res * v_res * (bits_per_pixel / 8);

	//Allow memory mapping
	mr.mr_base = video_phys;
	mr.mr_limit = mr.mr_base + vram_size;

	if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
		panic("sys_privctl (ADD_MEM) failed: %d\n", r);

	//Map memory
	video_mem = vm_map_phys(SELF, (void*)mr.mr_base, vram_size);

	if(video_mem == MAP_FAILED)
		panic("vga: vg_init: couldn't map video memory\n");

	//Allocate double buffer
	double_buffer = (char*) malloc(h_res * v_res * (bits_per_pixel / 8));

	return video_mem; //Virtual address of VRAM
}

//Draws pixel at (x, y) coords with specified color (24-bit direct color modes only)
void draw_pixel(int x, int y, int color) {

	if(x >= h_res || y >= v_res) return;

	char* vram_t = double_buffer;
	vram_t += (y * h_res + x) * (bits_per_pixel / 8);


	//RGB 8:8:8
	*vram_t = color & 0x0000FFL;
	*(vram_t + 1) = (color & 0x00FF00L) >> 8;
	*(vram_t + 2) = (color & 0xFF0000L) >> 16;
}

//Draws previously loaded png image at (start_x, start_y) coords
void vg_png(unsigned char* image, int width, int height, int start_x, int start_y) {

	size_t x, y;

	for(y = 0; y < height; y++) {
		for(x = 0; x < width; x++) {
			uint32_t r = image[(x + y * width) * 3];
			uint32_t g = image[(x + y * width) * 3 + 1];
			uint32_t b = image[(x + y * width) * 3 + 2];
			if(r!=0 && g!=0 && b!=0){
			uint32_t color = ((r << 16) & 0xFF0000) | ((g << 8) & 0x00FF00) | (b & 0x0000FF);
			draw_pixel(start_x + x, start_y + y, color);
			}
		}
	}
}

//Sets buffer to all black for clearing screen
void vg_clear() {
	memset(double_buffer, 0, h_res * v_res * (bits_per_pixel / 8));
}

//Copies RAM buffer to VRAM for double buffering
void vg_copy() {
	memcpy(video_mem, double_buffer, h_res * v_res * (bits_per_pixel / 8));
}

//Free double buffer
void vg_free() {
	free(double_buffer);
}


int vg_exit() {


  struct reg86u reg86;

  reg86.u.b.intno = 0x10; /* BIOS video services */

  reg86.u.b.ah = 0x00;    /* Set Video Mode function */
  reg86.u.b.al = 0x03;    /* 80x25 text mode*/

  if( sys_int86(&reg86) != OK ) {
      printf("\tvg_exit(): sys_int86() failed \n");
      return 1;
  } else
      return 0;
}


int vg_draw_pixel(unsigned short x, unsigned short y, unsigned long color){

	if( x < 0 || x > h_res-1 || y < 0 || y > v_res-1 ) return 1;

	char* videoptr = video_mem;
	videoptr += (y*h_res)+x;
	*videoptr = color;

	return 0;

}

void vg_draw_bike(unsigned short x, unsigned short y, unsigned long color){
	int row, col;
	int size=10;

	for(row = y-(size/2); row < y+(size/2); row++){
			for(col= x-(size/2); col < x+(size/2); col++){
				vg_draw_pixel(col,row,color);
			}
	}
}

unsigned long vg_getColor(unsigned short x, unsigned short y){

	char* videoptr = video_mem;
	videoptr += (y*h_res)+x;

	return *videoptr;

}


char * getSecondaryBuffer(){
	return double_buffer;
}

int getVerResolution(){
	return v_res;
}


int getHorResolution(){
	return h_res;
}


