#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

/** @defgroup i8254 i8254
 * @{
 *
 * Constants for programming the i8254 Timer. Needs to be completed.
 */

#define KDB_IRQ	        1   /**< @brief keybord 1 IRQ line */
#define BIT(n) (0x01<<(n))

#define OUT_BUF 0x60      //output buffer 
#define ESC_BREAK 0x81    //esc break key
#define STAT_REG 0x64 
#define OBF BIT(0)
#define WAIT_KBC 20000
#define KBC_CMD_REG 0x64
#define READ_CMD_BYTE 0x20 //read comand byte 
#define WRITE_CMD_BYTE 0x60
#define AUX BIT(5)
#define INT_KDB_ENABLE 0x01 

#define TWO_BYTES  0xe0    //if key code has 2 bytes 
#define MAKE_TYPE 0x80 //check if break or make

#define IBF         BIT(1)
#define OBF         BIT(0)
#define PAR_ERR     BIT(7)
#define TO_ERR      BIT(6)

#define MAX_RETRIES 5
#define IN_BUF 0x60       //intput buffer 

#endif 
/* _LCOM_I8042_H */