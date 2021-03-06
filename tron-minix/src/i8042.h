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
#define MOUSE_IRQ		12

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
#define MAKECODE_A 0x1E
#define MAKECODE_W 0x11
#define MAKECODE_S 0x1F
#define MAKECODE_D 0x20
#define MAKECODE_UP_NUMPAD8 0x48
#define MAKECODE_LEFT_NUMPAD4 0x4B
#define MAKECODE_DOWN_NUMPAD2 0x50
#define MAKECODE_RIGHT_NUMPAD6 0x4D

/*KBC Commands*/
#define READ_COMMAND_BYTE 0x20
#define WRITE_COMMAND_BYTE 0x60
#define BYTE_TO_MOUSE 0xD4
#define ENABLE_MOUSE_INT 0x47


//---------------------

/*MOUSE Commands*/

#define ENABLE_DATA_REPORT 0xF4
#define DISABLE_DATA_REPORT 0xF5
#define SET_STREAM_MODE 0xEA
#define SET_REMOTE_MODE 0xF0
#define READ_DATA 0xEB

//--------------------

#define ACK 0xFA
#define NACK 0xFE
#define ERROR 0xFC

//------------------

#define LB BIT(0)
#define RB BIT(1)
#define MB BIT(2)

#define XSIGN BIT(4)
#define YSIGN BIT(5)
#define XOVF BIT(6)
#define YOVF BIT(7)

//-----------

#define BLUE 0x0B
#define WHITE 0x3F

#define UP 1
#define RIGHT 2
#define DOWN 3
#define LEFT 4





/**@}*/

#endif /* _LCOM_I8042_H */
