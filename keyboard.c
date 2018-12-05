#include <lcom/lcf.h>
#include <keyboard.h>

int kbd_hook_id = 1;

int subscribe_kbd(uint8_t *kbd_bit_no) {
  *kbd_bit_no = kbd_hook_id;
  if(sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE|IRQ_EXCLUSIVE, &kbd_hook_id) != OK) return 1; //sets exclusive interrupts to kbd_hook_id
  return 0;
}

int unsubscribe_keyboard() {
	if(sys_irqrmpolicy(&kbd_hook_id )!= OK) return 1;
	else return 0;
}

uint16_t kbd_ih(){
  bool make = false;
  uint8_t scan_bytes[2];
  uint8_t scancode = 0;
  uint32_t stat, data;
  if(sys_inb(STAT_REG, &stat) != OK) return 1;
  uint8_t st = (uint8_t) stat;
  if(stat & OBF) {
    if(sys_inb(OUT_BUF, &data) != OK) return 1;
    if ((st &(PAR_ERR | TO_ERR)) == 0){
      scancode = (uint8_t) data;
    }  
    else{} //in case there was an error simply discard the output buffer value
  }

  if(scancode == TWO_BYTES){
    scan_bytes[0] = scancode;
    make = false;
  }
  else if(scan_bytes[0] == TWO_BYTES){
    scan_bytes[1] = scancode;
    make = !((scancode & MAKE_TYPE ) >> 7); //because it's make if it's set to 0
    return ((scan_bytes[0] << 8) | (scan_bytes[1] & 0xff)); //returns double byte key
  }
  else{
  scan_bytes[0] = scancode;
  make = !((scancode & MAKE_TYPE ) >> 7);
  return scan_bytes[0]; //returns single byte key
  }

  return 1;
}
