#include <lcom/lcf.h>
#include <keyboard.h>

int hook_id;
uint8_t scancode;
uint8_t bit_no;

int subscribe_kbd(uint8_t *bit_no) {
  *bit_no = hook_id;
  if(sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE|IRQ_EXCLUSIVE, &hook_id) != OK) return 1; //sets exclusive interrupts to hook_id
  return 0;
}

int unsubscribe_keyboard() {
	if(sys_irqrmpolicy(&hook_id )!= OK) return 1;
	else return 0;
}

void (kbc_ih)(){
  uint32_t stat, data;
  if(sys_inb(STAT_REG, &stat) != OK) return;
  uint8_t st = (uint8_t) stat;
  if(stat & OBF) {
    if(sys_inb(OUT_BUF, &data) != OK) return;
    if ((st &(PAR_ERR | TO_ERR)) == 0){
      scancode = (uint8_t) data;
    }  
    else{} //in case there was an error simply discard the output buffer value
  }
}
