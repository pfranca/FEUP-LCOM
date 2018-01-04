#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming the i8042 KeyBoard Controller.
 */

#define BIT(n) (0x01<<(n))

#define DELAY_US 20000

#define TIMEOUT 4 //Number of times it loops before exiting

#define KBC_IRQ	        1    /**< @brief KBC IRQ line */

/* I/O port addresses */

#define STAT_REG 0x64
#define KBC_CMD_REG 0x64
#define OUT_BUF 0x60
#define IN_BUF 0x60

//---------------------

#define OBF BIT(0)
#define IBF BIT(1)
#define PAR_ERR BIT(7)
#define TO_ERR BIT(6)
#define BREAKCODE BIT(7)
#define BREAKCODE_ESC 0x81
#define TOW_BYTE_CODE 0xE0

/*BBC Commands*/

#define READ_COMMAND_BYTE 0x20
#define WRITE_COMMAND_BYTE 0x60



/**@}*/

#endif /* _LCOM_I8042_H */
