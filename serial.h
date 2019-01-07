#ifndef __SERIAL_H
#define __SERIAL_H

#include "game.h"

#define BIT(n) (0x01<<(n))

#define COM1        0x3F8
#define COM1_IRQ    4

#define RBR         0x00 //Reciever Buffer Register (READ)
#define THR         0x00 //Transmitter Holding Register (WRITE)
#define IER         0x01 //Interrupt enable register
#define IIR         0x02 //Interrupt Id Reg (READ)
#define FCR         0x02 //FIFO Register (WRITE)
#define LCR         0x03 //Line Control Register
#define LSR         0x05 //Line Status Register

////////LCR////////
#define DLAB        BIT(7)
#define EIGHT_BITS_PER_CHAR		BIT(1) | BIT(0)

////////LSR////////
#define DATA_RECIEVED		BIT(0)
#define OVERRUN_ERROR		BIT(1)
#define PARITY_ERROR		BIT(2)
#define FRAMING_ERROR		BIT(3)
#define BREAK_INTERRUPT		BIT(4)
#define UART_READY		    BIT(5) //UART is ready to send
#define BOTH_EMPTY		    BIT(6)
#define FIFO_ERROR		    BIT(7)

////////IER////////
#define ENABLE_DATA		    BIT(0)
#define ENABLE_LS		    BIT(2)

////////IIR////////
#define NO_INTERRUPT		BIT(0)
#define DATA_AVAILABLE		BIT(2)
#define ERROR_INT		    BIT(2) | BIT(1)

////////FIFO///////
#define ENABLE_FIFO         BIT(0) | BIT(1) | BIT(2)

/**
 * @brief Subscribes Serial port interrupts
 * @param bit_no
 * @return 0 if OK
 */
int subscribe_serial(uint8_t *bit_no);
/**
 * @brief unsubscribes serial interrupts
 * @return 0 of OK
 */
int unsubscribe_serial();

/**
 * @brief Sends a char to the THR
 * @param value char to send
 * @return true if successful
 */
bool send_char(unsigned char value);
/**
 * @brief Reads from the RBR everything in the FIFO and stores it in a unit8_t pointer
 * @param values pointer where information is stored
 */
void read_char(uint8_t* values);

/**
 * @brief Checks for errors and Reads data
 * @return data read from the status register
 */
uint8_t serial_ih();

/**
 * @brief Checks which information was recvied from the FIFO and depending on whether there is already a reading in course sends it to the apropriate place
 */
void Check_Recieve();

/**
 * @brief Stored Name recieved to the appropriate place
 * @param data information recieved
 * @return returns the updated pointer location
 */
uint8_t* Recieve_Name(uint8_t* data);
/**
 * @brief Sends a string to other player
 * @param name string to send
 * @return true if successful
 */
bool Send_Name(char* name);
/**
 * @brief Sends Wizard information
 * @param wizard pointer to wizard to send
 * @param array_pos position of wizard in array
 * @return returns true upon success
 */
bool Send_Wizard(Wizard* wizard, int array_pos);
/**
 * @brief Appropriately stores Recieved wizard data
 * @param data data recieved
 * @return returns updated data pointer position
 */
uint8_t* Recieve_Wizard(uint8_t* data);
/**
 * @brief Sends Element information
 * @param element element to send
 * @param array_pos position of element in array
 * @return true if successful
 */
bool Send_Element(Element* element, int array_pos);
/**
 * @brief Stores Recieved element info
 * @param data recieved data
 * @return updated data pointer
 */
uint8_t* Recieve_Element(uint8_t* data);

/**
 * @brief Clears the FIFO if there is any unwanted information there
 */
void Clear_UART();

#endif
