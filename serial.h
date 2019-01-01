#ifndef __SERIAL_H
#define __SERIAL_H

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

#define DLL         0x00 //WHEN DLAB is SET (LCR)
#define DLM         0x01 //WHEN DLAB is SET (LCR)

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


int subscribe_serial(uint8_t *bit_no);
int unsubscribe_serial();

bool send_char(unsigned char value);
void read_char(uint8_t* values);

uint8_t serial_ih();

void Check_Recieve();
void Recieve_Name(uint8_t* data);
bool Send_Name(char* name);

#endif
