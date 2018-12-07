#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

/** @defgroup i8254 i8254
 * @{
 *
 * Constants for programming the i8254 Timer. Needs to be completed.
 */

#define KDB_IRQ	        1   /**< @brief keybord 1 IRQ line */
#define MOUSE_IRQ       12   /**< @brief mouse 12 IRQ line */
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

#define ENABLE_STREAM_CMD 0xf4
#define SET_STREAM 0xea
#define DISABLE_STREAM_CMD 0xf5
#define SET_REMOTE_MODE 0xF0 
#define MOUSE_CMD 0xd4 //command to write commands to mouse
#define DISABLE_DATA_REPORT 0xf5

#define ACK 0xfa  //Command recieved successfully
#define NACK 0xfe  //Invalid byte recieved
#define ERROR 0xfc  //second invalid byte recieved

#define READ_DATA 0xEB //mouse packets readings 

#define MAX_RETRIES 5
#define IN_BUF 0x60       //intput buffer 

//Keycodes definitions 
#define A   0x1e     
#define B   0x30     
#define C   0x2E    
#define D   0x20     
#define E   0x12     
#define F   0x21     
#define G   0x22     
#define H   0x23     
#define I   0x17     
#define J   0x24     
#define K   0x25     
#define L   0x26     
#define M   0x32     
#define N   0x31     
#define O   0x18     
#define P   0x19    
#define Q   0x10     
#define R   0x13     
#define S   0x1F     
#define T   0x14  
#define U   0x16     
#define V   0x2F     
#define W   0x11  
#define X   0x2D     
#define Y   0x15     
#define Z   0x2C     

#define ENTER 0X1C
#define BACKSPACE 0X0E
#define SPACEBAR 39 


#endif 
/* _LCOM_I8042_H */
