#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>

#include "read_xpm.h"
#include "vbe.h"

/* Constants for VBE 0x105 mode */

/* The physical address may vary from VM to VM.
 * At one time it was 0xD0000000
 *  #define VRAM_PHYS_ADDR    0xD0000000
 * Currently on lab B107 is 0xF0000000
 * Better run my version of lab5 as follows:
 *     service run `pwd`/lab5 -args "mode 0x105"
 */
#define VRAM_PHYS_ADDR	0xE0000000
#define H_RES           1024
#define V_RES		  	768
#define BITS_PER_PIXEL	8

/* Private global variables */

static char* video_mem;		/* Process (virtual) address to which VRAM is mapped */

static unsigned h_res;		/* Horizontal screen resolution in pixels */
static unsigned v_res;		/* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */

void* vg_init(unsigned short mode) {

  struct reg86u reg;

  reg.u.w.ax = 0x4F02; // VBE call, function 02 -- set VBE mode
  reg.u.w.bx = 1<<14|mode; // Mode 0x105
  reg.u.b.intno = 0x10;

  if( sys_int86(&reg) != OK ) {
    printf("set_vbe_mode: sys_int86() failed \n");
    return NULL;
  }

  h_res = H_RES;
  v_res = V_RES;
  bits_per_pixel = BITS_PER_PIXEL;


  int r;
  struct mem_range mr;
  unsigned int vram_base = VRAM_PHYS_ADDR; /* VRAM’s physical addresss */
  unsigned int vram_size = (h_res * v_res * bits_per_pixel)/8 ;    /* VRAM’s size, but you can use the frame-buffer size, instead */

  /* Allow memory mapping */
  mr.mr_base = (phys_bytes) vram_base;
  mr.mr_limit = mr.mr_base + vram_size;

  if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);

  /* Map memory */
  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

  if(video_mem == MAP_FAILED)
    panic("couldn’t map video memory");

  return video_mem;

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

void vg_draw_centered_square(unsigned short x, unsigned short y, unsigned short size, unsigned long color){
	int row, col;

	for(row = (384+y)-(size/2); row < (384+y)+(size/2); row++){
			for(col= (512+x)-(size/2); col < (512+x)+(size/2); col++){
				vg_draw_pixel(col,row,color);
			}
	}
}


int vg_draw_xpm(char *xpm[], unsigned short xi, unsigned short yi){

	int width, height;
	char *map;
	// get the pix map from the XPM
	map = read_xpm(xpm, &width, &height);

	char *pointer;
	pointer = video_mem;

	int max = h_res * v_res * bits_per_pixel / 8;

	// Paints the screen in black
	int i;
	for(i=0;i < max; i++){
		*pointer = 0; //Black color
		pointer++;
	}

	// Draws each image pixel
	int j, k, pos = 0;

	for (k = yi; k < yi+height; k++)
	{
		for (j = xi; j < xi+width; j++)
		{
			vg_draw_pixel(j,k, map[pos]);
			pos++;
		}
	}

	return 0;
}

void vg_draw(char *map, int height, int width, unsigned short xi, unsigned short yi){

	char *pointer;
	pointer = video_mem;

	int max = h_res * v_res * bits_per_pixel / 8;

	// Paints the screen in black
	int i;
	for(i=0;i < max; i++){
		*pointer = 0; //Black color
		pointer++;
	}

	// Draws each image pixel
	int j, k, pos = 0;

	for (k = yi; k < yi+height; k++)
	{
		for (j = xi; j < xi+width; j++)
		{
			vg_draw_pixel(j,k, map[pos]);
			pos++;
		}
	}
}


void vg_draw_line(unsigned short xi, unsigned short yi, unsigned short xf, unsigned short yf, unsigned long color){

	//Based on Bresenham's line algorithm implementation
	int i;
	int w = xf - xi ;
	int h = yf - yi ;
	int dx1 = 0, dy1 = 0, dx2 = 0, dy2 = 0 ;
	if (w<0) dx1 = -1 ; else if (w>0) dx1 = 1 ;
	if (h<0) dy1 = -1 ; else if (h>0) dy1 = 1 ;
	if (w<0) dx2 = -1 ; else if (w>0) dx2 = 1 ;
	int longest = abs(w) ;
	int shortest = abs(h) ;
	if (!(longest>shortest)) {
		longest = abs(h) ;
		shortest = abs(w) ;
		if (h<0) dy2 = -1 ; else if (h>0) dy2 = 1 ;
		dx2 = 0 ;
	}
	int numerator = longest >> 1 ;
	for (i=0;i<=longest;i++) {
		vg_draw_pixel(xi,yi,color) ;
		numerator += shortest ;
		if (!(numerator<longest)) {
			numerator -= longest ;
			xi += dx1 ;
			yi += dy1 ;
		} else {
			xi += dx2 ;
			yi += dy2 ;
		}
	}
}


