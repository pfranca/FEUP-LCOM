#ifndef __TEST5_H
#define __TEST5_H

/** @defgroup lab5 lab5
 * @{
 *
 * Functions for outputing data to screen in graphics mode
 */

/**
 * @brief Tests initialization of video card in graphics mode
 * 
 * Uses the VBE INT 0x10 interface to set the desired
 *  graphics mode, and resets Minix's default text mode after 
 *  a short delay. Before exiting, displays on the console the
 *  physical address of VRAM in the input graphics mode.
 * 
 * @param mode 16-bit VBE mode to set
 * @param delay delay in seconds after which returns to text mode
 * @return Virtual address VRAM was mapped to. NULL, upon failure.
 */
void *video_test_init(unsigned short mode, unsigned short delay);

/**
 * @brief Tests drawing a square with a given color
 * 
 * Tests drawing a square with the specified size and color, at the specified
 *  coordinates (which specify the square center with respect to the center 
 *  of the screen) in video mode 0x105
 * 
 * @param x horizontal coordinate of square center, wrt the center of the screen
 * @param y vertical coordinate of square center, wrt the center of the screen
 * @param size of each side in pixels
 * @param color color to set the pixel
 * @return 0 on success, non-zero otherwise
 */
int video_test_square(unsigned short x, unsigned short y, unsigned short size, unsigned long color);

/**
 * @brief Tests drawing a line segment with specifed end points and color
 * 
 * Tests drawing a line segment with the specified end points and the input color, 
 *  by writing to VRAM in video mode 0x105
 * 
 * @param xi horizontal coordinate of "first" endpoint, starts at 0 (leftmost pixel)  
 * @param yi vertical coordinate of "first" endpoint, starts at 0 (top pixel)
 * @param xf horizontal coordinate of "last" endpoint, starts at 0 (leftmost pixel)  
 * @param yf vertical coordinate of "last" endpoint, starts at 0 (top pixel)  
 * @param color color of the line segment to draw
 * @return 0 upon success, non-zero upon failure
 */
int video_test_line(unsigned short xi, unsigned short yi, 
		           unsigned short xf, unsigned short yf, unsigned long color);

/** @} end of lab5 */
 
 
#endif /* __TEST5_H */
