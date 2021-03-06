#ifndef __VIDEO_GR_H
#define __VIDEO_GR_H

#define VRAM_PHYS_ADDR	0xE0000000
#define H_RES           1024
#define V_RES		  	768
#define BITS_PER_PIXEL	8

#define ENABLE_DEBUG 1

#if ENABLE_DEBUG == 1

#define LOG_MSG(FILE, TEXT) fprintf(FILE, TEXT)
#else
#define LOG_MSG(...)
#endif

/** @defgroup video_gr video_gr
 * @{
 *
 * Functions for outputing data to screen in graphics mode
 */

/**
 * @brief Initializes the video module in graphics mode
 * 
 * Uses the VBE INT 0x10 interface to set the desired
 *  graphics mode, maps VRAM to the process' address space and
 *  initializes static global variables with the resolution of the screen, 
 *  and the number of colors
 * 
 * @param mode 16-bit VBE mode to set
 * @return Virtual address VRAM was mapped to. NULL, upon failure.
 */
void* vg_init(unsigned short mode);

 /**
 * @brief Returns to default Minix 3 text mode (0x03: 25 x 80, 16 colors)
 * 
 * @return 0 upon success, non-zero upon failure
 */
int vg_exit(void);

/**
* @brief draws one pixel on the screen
*
* @param x pixel x coord
*
* @param y pixel y coord
*
* @param color pixel color
*
* @return 0 upon success, non-zero upon failure
*/
int vg_draw_pixel(unsigned short x, unsigned short y, unsigned long color);

/**
* @brief gets a pixel color
*
* @param x pixel x coord
*
* @param y pixel y coord
*
* @return 0 upon success, non-zero upon failure
*/
unsigned long vg_getColor(unsigned short x, unsigned short y);

/**
* @brief draws the bike
*
* @param x pixel x coord
*
* @param y pixel y coord
*
* @param color pixel color
*
* @return 0 upon success, non-zero upon failure
*/
void vg_draw_bike(unsigned short x, unsigned short y, unsigned long color);


/**
 * @brief return the secondary buffer address
 *
 * @return secondary buffer address
 */

char * getSecondaryBuffer();


/**
 * @brief gets vertical resolution
 *
 * @return vertical resolution
 */
int getVerResolution();


/**
 * @brief gets horizontal resolution
 *
 * @return horizontal resolution
 */
int getHorResolution();

/*
 * @brief Draws pixel at (x, y) coords with specified color (24-bit direct color modes only)
 *
 * @param x coord x
 *
 * @param y coord y
 *
 * @param color pixel color
 *
 * */
void draw_pixel(int x, int y, int color);



/*
 * @brief Draws previously loaded png image at (start_x, start_y) coords
 *
 * @param image image addresss
 *
 * @param width image width
 *
 * @param height image height
 *
 * @param start_x initial x coord
 *
 * @param start_y initial y coord
 *
 * */
void vg_png(unsigned char* image, int width, int height, int start_x, int start_y);


/*
 * @brief Sets buffer to all black for clearing screen
 *
 * */
void vg_clear();

/*
 * @brief Copies RAM buffer to VRAM for double buffering
 * */
void vg_copy();


/*
 * @brief Free double buffer
 * */
void vg_free();


 /** @} end of video_gr */
 
#endif /* __VIDEO_GR_H */
