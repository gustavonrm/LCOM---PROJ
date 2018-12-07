#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

unsigned int ticks=0; 
int timer_hook_id = 0;  //used for timer

extern unsigned int ticks;

int (timer_subscribe_int)(uint8_t *bit_no) {
  *bit_no = timer_hook_id;
  if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &timer_hook_id) != OK) return 1;
  return 0;
}

int (timer_unsubscribe_int)() {
  if(sys_irqrmpolicy(&timer_hook_id) != OK) return 1;
  return 0;
}

int timer_ih() {
  if (ticks == 60) ticks = 0;
  ticks++;
  return ticks;
}
