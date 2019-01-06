#include <lcom/lcf.h>
#include "serial.h"
#include "video_card.h"

int serial_hook_id = 3;
extern Element* elements[ELEMS_SIZE];
extern Wizard* wizards[WIZARDS_SIZE];
extern bool MP;

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
    printf("\n Subscribed Serial");
  Clear_UART();
  printf("\n Subscribed Serial");

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
    //printf("\n TRYING TO SEND: %c", value); //debug
  } while(!(st & UART_READY));

  sys_outb(COM1 + THR, value); //write to THR when ready
  //printf("\nSENT");
  return true;
}

void read_char(uint8_t* values){
    uint32_t st, data;
	sys_inb(COM1 + LSR, &st);	
    unsigned int i = 0;
	while (st & DATA_RECIEVED && i < 351){ //Only read if there's anything there
		sys_inb(COM1 + RBR, &data);
        
        *values = (uint8_t) data;
        values++;
        i++;
        //printf("\n READ THIS: %c", data);
        sys_inb(COM1 + LSR, &st);
	}
}

uint8_t serial_ih(){
    printf("\n INTERRUPT GENERATED"); //debug
    uint32_t id = 0, data = 0;
	
    sys_inb(COM1 + IIR, &id);
	if (id & NO_INTERRUPT) return 1;
    else if(id & BIT(1) && id & BIT(2) && id & ~BIT(0))
    {
		sys_inb(COM1 + LSR, &data);
		printf("\nERROR WITH DATA: 0x%x", (uint8_t) data);
    }
    else if(id & DATA_AVAILABLE)
    {
        Check_Recieve();
    }

    return data;
}

extern char* username_2;
bool name = false, wizard = false, element = false;
uint8_t* data_end = NULL;
void Check_Recieve(){
    uint8_t* data = (uint8_t*) malloc(352); //enough for a lot of BYTES
    data_end = data + 352;
    read_char(data);
    //printf("\n DATA: %c", *data); //debug
    while(*data != '\0' && data != NULL){
        if(!name && !wizard && !element)
        {
            switch(*data)
            {
                case 'N':
                    if(MP)
                    {
                        free(data);
                        return;
                    }
                    name = true;
                    break;
                case 'W':
                    if(!MP)
                    {
                        Clear_UART();
                        free(data);
                        return;
                    }
                    wizard = true;
                    break;
                case 'E':
                    if(!MP)
                    {
                        Clear_UART();
                        free(data);
                        return;
                    }
                    element = true;
                    break;
                default:
                    free(data);
                    return;
            }
        }

        if(name) data = Recieve_Name(data);
        else if(wizard) data = Recieve_Wizard(data);
        else if(element) data = Recieve_Element(data);
    }

    free(data);
}

char recieved_name[15];
bool first_letter = true;
int actual_it = 0; //So if we only read until mid of string we can come back to proper place
uint8_t* Recieve_Name(uint8_t* data){
    printf("\n GOT NAME"); //debug
    if(*data == 'N' && first_letter) data++; //To skip N
    for(int i = actual_it; i < 15 && *data != '\0'; i++){
        if((*data) == '/')
        {
            name = false;
            //printf("\nRECIEVED NAME: %s", recieved_name); //debug
            data++;
            first_letter = true;
            username_2 = &recieved_name[0];
            //printf("\n USERNAME_2: %s", username_2); //debug
            actual_it = 0;
            return data;
        }
        else
        {
            if(data == NULL) return data;
            recieved_name[i] = *data;
            data++;
            first_letter = false;
            //printf("\nRECIEVED NAME: %s", recieved_name);
            actual_it++;
        }
    }
    //printf("\nINCOMPLETE NAME: %s", recieved_name); //debug
    return NULL;
}

bool Send_Name(char* name){
    unsigned int size = strlen(name);
    //printf("\n GOING TO SEND: %s", name); //debug
    send_char('N');
    for(unsigned int i = 0; i < size; i++){
        if(!send_char(*name)) return false;
        name++;
    }
    send_char('/');
    //printf("\n NAME SENT: %s", name); //debug
    return true;
}

bool Send_Wizard(Wizard* wizard, int array_pos){ //Sending 9 Bytes per wizard
    if(!send_char('W')) return false;
    uint16_t rot = wizard->rot; //2B for 9b

    int16_t x_pos = wizard->center_x; //2B
    int16_t y_pos = wizard->center_y; //2B

    uint8_t health = wizard->health; //1B for 2b
    uint8_t casting = wizard->casting; //1B for 1b
    uint8_t spell_type = wizard->spell; //1B for 3b

    int8_t cast_type = wizard->cast_type; //1B for 1B

    uint8_t frame_n = wizard->frame_n; //1B for 6b
    uint8_t try_n = wizard->try_n; //1B for 4b

    //printf("\n SENDING WIZARD! \n");

    //First send positions:
    int8_t x1 = (x_pos & 0xFF00) >> 8;
    int8_t x2 = (x_pos & 0x00FF);
    if(!send_char(x1) || !send_char(x2)) return false;

    int8_t y1 = (y_pos & 0xFF00) >> 8;
    int8_t y2 = (y_pos & 0x00FF);
    if(!send_char(y1) || !send_char(y2)) return false;
    ////////

    //Now send miscelaneous + first bit of rotation:
    uint8_t misc = ((health & 0x03) << 6) | (casting << 5) | (spell_type << 2) | ((rot & 0x0100) >> 8);
    uint8_t rot_last = rot & 0x00FF;
    if(!send_char(misc) || !send_char(rot_last)) return false; //last Byte of rotation
    ////////

    //Then send in order cast_type, frame_n and try_n:
    uint8_t pos = (((uint8_t) array_pos) & 0x03) << 6 | try_n;
    if(!send_char(cast_type) || !send_char(frame_n) || !send_char(pos)) return false;

    send_char('/');
    
    /*printf("\nX_POS: %d", x_pos);
    printf("\nY_POS: %d", y_pos);

    printf("\nHEALTH: %d", health);
    printf("\nCASTING: %d", casting);
    printf("\nSPELL_TYPE: %d", spell_type);

    printf("\nROT: %d", rot);

    printf("\nCAST TYPE: %d", cast_type);
    printf("\nframe_n: %d", frame_n);
    printf("\nTRY_N: %d", try_n);*/

    //printf("\n\n SENT WIZARD");
    return true;
}

uint8_t data[15];
size_t data_it = 0;
bool wiz_first = true;
uint8_t* Recieve_Wizard(uint8_t* values){
    //printf("\n Recieving Wizard"); //debug
    if(wiz_first) values++; //To skip W header
    wiz_first = false;
    //printf("\n Start Wizard Recieve Procedure");
    while(*values != '/')
    {
        //printf("\n VALUES: %c", *values);
        if(values == data_end){
            //printf("\n BROKE HERE: %d", *values);
            return NULL;
        }
        //printf("\n DATA BEFORE: %c", data[data_it]);
        data[data_it] = (*values);
        //printf("\n DATA: %c", data[data_it]);
        data_it++;
        values++;
    }
    //printf("\n VALUES AFTER BREAKING WHILE: %c", *values);
    data[data_it] = (*values);
    //printf("\n DATA AFTER WHILE: %c", data[data_it]);
    data_it = 0;

    int16_t x_pos;
    int16_t y_pos;

    uint8_t health;
    uint8_t casting;
    uint8_t spell_type;

    uint16_t rot;

    int8_t cast_type;
    uint8_t frame_n;
    uint8_t try_n;
    uint8_t pos;

    x_pos = (data[data_it]) << 8;
    data_it++;
    x_pos |= data[data_it];
    data_it++;

    y_pos = (data[data_it]) << 8;
    data_it++;

    y_pos |= data[data_it];
    data_it++;

    health = ((data[data_it]) & 0xC0) >> 6;
    casting = ((data[data_it]) & 0x20) >> 5;
    spell_type = ((data[data_it]) & 0x1C) >> 2;
    rot = ((data[data_it]) & 0x01) << 8;
    data_it++;

    rot |= (data[data_it]);
    data_it++;

    cast_type = (data[data_it]);
    data_it++;

    frame_n = (data[data_it]);
    data_it++;

    try_n = (data[data_it]) & 0x0F;
    pos = ((data[data_it]) & 0xC0) >> 6;
    data_it++;

    if((data[data_it]) == '/') wizard = false;
    else{
        printf("\nDIDN'T RECIEVE / \n"); //debug
        wiz_first = true;
        wizard = false;
        data_it = 0;
        return NULL;
    }
    wiz_first = true;
    
    //Needs to transfor positions into correct ones
    x_pos = x_pos - (x_pos - H_RES/2)*2;
    y_pos = y_pos - (y_pos - V_RES/2)*2;

    int new_rot = (int) (rot - 180);
    if(new_rot < 0) new_rot+= 360;

    Wizard* wizard = wizards[pos];
    wizard->center_x = (int) x_pos;
    wizard->center_y = (int) y_pos;

    wizard->health = (int) health;
    wizard->casting = (bool) casting;
    wizard->spell = spell_type;

    wizard->rot = (unsigned int) new_rot;

    wizard->cast_type = cast_type;
    wizard->frame_n = (int) frame_n;
    wizard->try_n = (int) try_n;

    //printf("\n RECEIEVED WIZARD \n"); //debug
    /*printf("\n RECEIEVED: \n");

    printf("\nX_POS: %d", x_pos);
    printf("\nY_POS: %d", y_pos);

    printf("\nHEALTH: %d", health);
    printf("\nCASTING: %d", casting);
    printf("\nSPELL_TYPE: %d", spell_type);

    printf("\nROT: %d", rot);

    printf("\nCAST TYPE: %d", cast_type);
    printf("\nframe_n: %d", frame_n);
    printf("\nTRY_N: %d", try_n);

    printf("\nARRAY POS: %d", pos);*/

    return values;
}

bool Send_Element(Element* element, int array_pos){ //Using 10B
    int16_t x_pos = element->center_x;
    int16_t y_pos = element->center_y;

    uint16_t rot = element->rot;
    uint8_t array_p = array_pos;

    uint8_t destroyed = element->destroyed;
    uint8_t active = element->active;
    uint8_t spell_type = element->spell_type;

    int8_t elem_type = element->elem_type;
    uint8_t frame_n = element->frame_n;
    uint8_t try_n = element->try_n;

    //printf("\nSEND ELEMENT"); //debug

    if(!send_char('E')) return false;

    //First send positions:
    int8_t x1 = (x_pos & 0xFF00) >> 8;
    int8_t x2 = (x_pos & 0x00FF);
    if(!send_char(x1) || !send_char(x2)) return false;

    int8_t y1 = (y_pos & 0xFF00) >> 8;
    int8_t y2 = (y_pos & 0x00FF);
    if(!send_char(y1) || !send_char(y2)) return false;
    ////////

    //Now send array_p + first bit of rotation + last B of rotation:
    uint8_t pos = ((array_p & 0x1F) << 3) | ((rot & 0x0100) >> 8);
    uint8_t rot_last = rot & 0x00FF;
    if(!send_char(pos) || !send_char(rot_last)) return false; //last Byte of rotation
    ////////

    //Now send miscelaneous:
    uint8_t misc = ((active & 0x01) << 7) | (destroyed << 6) | (spell_type & 0x07);
    if(!send_char(misc)) return false;
    ////////

    //Then send in order elem_type, frame_n and try_n:
    if(!send_char(elem_type) || !send_char(frame_n) || !send_char(try_n)) return false;

    if(!send_char('/')) return false;

    //printf("\nSENT");
    data_it = 0;
    return true;
}

bool elem_first;
uint8_t* Recieve_Element(uint8_t* values){
    if(elem_first)values++;//To skip E header
    while(*values != '/')
    {   
        printf("\n VALUES: %c", *values);
        printf("\n DATAI_IT: %d", data_it);
        if(values == data_end) return NULL;
        data[data_it] = *values;
        data_it++;
        values++;
    }
    data[data_it] = *values;
    printf("\n VALUES: %c", *values);
    printf("\n DATAI_IT: %d", data_it);
    data_it = 0;

    //printf("\n RECIEVING ELEMENT"); //debug

    int16_t x_pos;
    int16_t y_pos;

    uint16_t rot;
    uint8_t array_p;

    uint8_t destroyed;
    uint8_t active;
    uint8_t spell_type;

    int8_t elem_type;
    uint8_t frame_n;
    uint8_t try_n;

    x_pos = (data[data_it]) << 8;
    data_it++;
    x_pos |= data[data_it];
    data_it++;  

    y_pos = (data[data_it]) << 8;
    data_it++;
    y_pos |= data[data_it];
    data_it++;

    array_p = ((data[data_it]) & 0xF8) >> 3;
    rot = ((data[data_it]) & 0x01) << 8;
    data_it++;  

    rot |= (data[data_it]);
    data_it++;  

    active = ((data[data_it]) & 0x80) >> 7;
    destroyed = ((data[data_it]) & 0x40) >> 6;
    spell_type = (data[data_it]) & 0x07;
    data_it++; 

    elem_type = (data[data_it]);
    data_it++;  

    frame_n = data[data_it];
    data_it++;  

    try_n = data[data_it];
    data_it++;  

    if(data[data_it] != '/'){
        printf("\n Didn't Recieve /"); //debug
        elem_first = true;
        element = false;
        data_it = 0;
        return NULL;
    }
    element = false;
    elem_first = true;

    Element* element = NULL;
    if(elements[array_p] == NULL) element = Create_Guest_Element(array_p, elem_type, spell_type);
    else element = elements[array_p];

    x_pos = x_pos - (x_pos - H_RES/2)*2;
    y_pos = y_pos - (y_pos - V_RES/2)*2;

    int new_rot = (int) (rot - 180);
    if(new_rot < 0) new_rot+= 360;
    
    element->center_x = (int) x_pos;
    element->center_y = (int) y_pos;

    element->rot = (unsigned int) new_rot;
    element->destroyed = (bool) destroyed;
    element->active = (bool) active;

    element->elem_type = elem_type;
    element->spell_type = spell_type;
    element->frame_n = (int) frame_n;
    element->try_n = (int) try_n;

    //printf("\n RECEIEVED ELEMENT: \n"); //debug
    /*
    printf("\nX_POS: %d", x_pos);
    printf("\nY_POS: %d", y_pos);

    printf("\nROT: %d", rot);
    printf("\nARRAY POS: %d", array_p);

    printf("\nDESTROYED: %d", destroyed);
    printf("\nACTIVE: %d", active);
    printf("\nSPELL_TYPE: %d", spell_type);

    printf("\nELEM_TYPE: %d", elem_type);
    printf("\nFRAME_N: %d", frame_n);
    printf("\nTRY_N: %d", try_n);*/

    data_it = 0;
    return values;
}

void Clear_UART(){
     uint32_t st, data;
    sys_inb(COM1 + LSR, &st);
    while (st & DATA_RECIEVED){ //Only read if there's anything there
		sys_inb(COM1 + RBR, &data);
        //printf("\nGOT SOMETHING: %c", data); //debug
        sys_inb(COM1 + LSR, &st);
	}
}
