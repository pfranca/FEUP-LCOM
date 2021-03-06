#ifndef __MOUSE_H
#define __MOUSE_H

extern unsigned char packet[];
extern unsigned long dist;

typedef enum {
	INIT,
	DRAW,
	COMP

}state_t;

typedef enum {
	EV_RDOW,
	EV_RUP,
	EV_DIAGONALMOVE,
	EV_RESETLEN,
	EV_EXIT

}ev_type_t;


/** @defgroup Mouse Mouse
 * @{
 *
 * Functions for using the i8042 KBC INTERFACING WITH THE MOUSE
 */



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
 * @brief Puts mouse working in remote mode
 *
 * @return Returns ACK, NACK or ERROR
 */
int mouse_set_remote_mode();

/**
 * @brief Display mouse data packet in a human friendly way
 *
 * @param packet array of 3 bytes containing mouse config
 *
 */
void mouse_display_packet(unsigned char packet[]);

/**
 * @brief Gets the mouse packets and prints them on the screen
 *
 * @return 1 in case of success, 0 otherwise
 */
int mouse_get_packet();

/**
 * @brief State machine. It is responsible to take the events and reach to COMP state.
 */
void check_for_line(state_t *state, ev_type_t evt);



#endif /* __KEYBOARD_H */
