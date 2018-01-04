#ifndef __MOUSE_H
#define __MOUSE_H

extern unsigned char packet[];

/** @defgroup Mouse Mouse
 * @{
 *
 * Functions for using the i8042 KBC INTERFACING WITH THE MOUSE
 */


typedef struct {
    int x;						/**< mouse x coordinate */
	int y;						/**< mouse y coordinate */

    int packet_idx;				/**< index of the packet being currently read*/
    int complete_packet;		/**< 1 if packet is ready to be read, 0 otherwise*/
    unsigned long packet[3]; 	/**< array containing the 3 mouse packets*/

    int leftButtonDown;			/**< 1 if mouse's left button is being pressed, 0 otherwise*/
    int leftButtonReleased;		/**< 1 if mouse's left button has just been released, 0 otherwise*/


    int draw;					/**< 1 if mouse must be displayed on the screen, 0 otherwise*/
} Mouse;



/**
 * @brief new mouse instantiation
 *
 * @return mouse object
 */
Mouse* newMouse();

/**
 * @brief gets the mouse object
 *
 * @return mouse object
 */
Mouse* getMouse();

/*
 * @brief updates the mouse
 *
 * 	Gets mouse packets
 * 	Checks whether the buttons where clicked or not
 * */
void updateMouse();

/**
 * @brief new mouse instantiation
 *
 * @param cursor the mouse cursor image
 *
 * @param width the mouse cursor image width
 *
 * @param width the mouse cursor image height
 */
void drawMouse(unsigned char * cursor, int width, int height);

/*
 * @brief deletes the mouse
 *
 * */
void deleteMouse();

/*
 * @brief gets the mouse packets
 *
 * */
void packet_handler();

/**
 * @brief check if the mouse is inside the window
 *
 * @param xi initial X coord
 *
 * @param xf final X coord
 *
 * @param yi initial Y coord
 *
 * @param yf final Y coord
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int checkInside(unsigned xi, unsigned xf, unsigned yi, unsigned yf);


/**
 * @brief Subscribes and enables KBC interrupts
 *
 * @return Returns bit order in interrupt mask; negative value on failure
 */
int mouse_subscribe_int(void);

/**
 * @brief Unsubscribes KBC interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int mouse_unsubscribe_int();

/**
 * @brief Enable mouse data report in stream mode
 *
 * @return Returns ACK, NACK or ERROR
 */
int mouse_enable_data_report();

/**
 * @brief Disable mouse data report in stream mode
 *
 * @return Returns ACK, NACK or ERROR
 */
int mouse_disable_data_report();

/**
 * @brief Puts mouse working in stream mode
 *
 * @return Returns ACK, NACK or ERROR
 */
int mouse_set_stream_mode();

/**
 * @brief Gets the mouse packets and prints them on the screen
 *
 * @return 1 in case of success, 0 otherwise
 */
int mouse_get_packet();


#endif /* __KEYBOARD_H */
