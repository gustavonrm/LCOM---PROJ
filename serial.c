#include <lcom/lcf.h>
#include "serial.h"

int serial_hook_id = 3;

int subscribe_serial(uint8_t *bit_no){
  *bit_no = serial_hook_id;

  if(sys_irqsetpolicy(COM1_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &serial_hook_id) != OK)
    return 1;

  uint32_t lcr_cmd, enable_cmd;
	if(sys_inb(COM1 + LCR, &lcr_cmd) != OK) return 1;

  lcr_cmd &= ~DLAB; //Clearing DLAB in case it's on
  lcr_cmd |= EIGHT_BITS_PER_CHAR;

  if(sys_outb(COM1 + LCR, lcr_cmd) != OK) return 1;
  //NEED to set Bitrate

  if(sys_inb(COM1 + IER, &enable_cmd) != OK) return 1;

  enable_cmd = ENABLE_DATA | ENABLE_LS;

  if(sys_outb(COM1 + IER, enable_cmd) != OK) return 1;

  sys_outb(COM1 + FCR, ENABLE_FIFO); // Enable FIFOs

  return 0;
}

int unsubscribe_serial(){
	if(sys_irqrmpolicy(&serial_hook_id) != OK) return 1;
	if (sys_outb(COM1 + IER, 0) != OK) return 1;

	return 0;
}

bool send_char(unsigned char value){
  uint32_t st;
  unsigned int fail = 0;
  //Check if UART is empty

  do{
    if(fail == 50) return false; //If it's stuck here
    sys_inb(COM1 + LSR, &st);
    fail++;
  } while(!(st & UART_READY));

  sys_outb(COM1 + THR, value); //write to THR when ready
  //printf("\nSENT");

  return true;
}

void read_char(uint8_t* values){
    uint32_t st, data;
	sys_inb(COM1 + LSR, &st);	

	while (st & DATA_RECIEVED){ //Only read if there's anything there
		sys_inb(COM1 + RBR, &data);
        //printf("\nGOT SOMETHING: %c", data);
        
        *values = (uint8_t) data;
        values++;
        sys_inb(COM1 + LSR, &st);
	}
}

uint8_t serial_ih(){
    uint32_t id = 0, data = 0;
	
    sys_inb(COM1 + IIR, &id);
	if (id & NO_INTERRUPT) return 1;
    else if(id & BIT(1) && id & BIT(2) && id & ~BIT(0))
    {
		sys_inb(COM1 + LSR, &data);
		printf("\nerror -> 0x%x", (uint8_t) data);
    }
    else if(id & DATA_AVAILABLE)
    {
        Check_Recieve();
    }

    return data;
}

bool name = false, wizard = false, element = false;
void Check_Recieve(){
    uint8_t* data = (uint8_t*) malloc(64); //enough for 64 Bytes
    read_char(data);

    if(!name && !wizard && !element)
    {
        switch(*data)
        {
            case 'N':
                name = true;
                break;
            case 'W':
                wizard = true;
                break;
            case 'E':
                element = true;
                break;
            default:
                break;
        }
    }

    if(name) Recieve_Name(data);
    //else if(wizard) Recieve_Wizard();
    //else if(element) Recieve_Element();

    free(data);
}

char recieved_name[15];
void Recieve_Name(uint8_t* data){
    data++; //To skip N
    for(int i = 0; i < 15 && data != NULL; i++){
        if((*data) == '/')
        {
            name = false;
            printf("\nRECIEVED NAME: %s", recieved_name);
            return;
        }
        else
        {
            recieved_name[i] = *data;
            data++;
            //printf("\nRECIEVED NAME: %s", recieved_name);
        }
    }
}

bool Send_Name(char* name){
    unsigned int size = strlen(name);
    send_char('N');
    for(unsigned int i = 0; i < size; i++){
        if(!send_char(*name)) return false;
        name++;
    }
    send_char('/');
    return true;
}

