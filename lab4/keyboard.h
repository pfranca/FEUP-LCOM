#ifndef __KEYBOARD_H
#define __KEYBOARD_H

/** @defgroup Keyboard Keyboard
 * @{
 *
 * Functions for using the i8042 KBC
 */

/**
 * @brief Display to the screen sys_inb_counter and reset's it.
 *
 */
void print_and_reset_sys_counter();

/**
 * @brief Increments the sys_inb counter whenever sys_inb is called
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int sys_inb_cnt(unsigned long porto, unsigned long *reg);

/**
 * @brief Subscribes and enables KBC interrupts
 *
 * @return Returns bit order in interrupt mask; negative value on failure
 */
int kbc_subscribe_int(void);

/**
 * @brief Unsubscribes KBC interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int kbc_unsubscribe_int();

/**
 * @brief Read OUTPUT BUFFER (0x60)
 *
 * @return Return code red from the OUT BUFFER; 0xFFF on failure
 */
int kbc_read();


/**
 * @brief Display to the screen sys_inb_counter and reset's it.
 *
 */
void display_scancode(unsigned long scancode);

/**
 * @brief write command to the kbc
 * First reads status comand byte
 *
 * @param porto Port to write to
 * @param cmd Command to write
 *
 */
int kbc_write(unsigned long porto, unsigned long cmd);


#endif /* __KEYBOARD_H */
