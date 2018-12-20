#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

unsigned int ticks = 0;
int timer_hook_id = 0; //used for timer

extern unsigned int ticks;

int(timer_subscribe_int)(uint8_t *bit_no)
{
  *bit_no = timer_hook_id;
  if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &timer_hook_id) != OK)
    return 1;
  return 0;
}

int(timer_unsubscribe_int)()
{
  if (sys_irqrmpolicy(&timer_hook_id) != OK)
    return 1;
  return 0;
}

int timer_ih()
{
  if (ticks == 60)
    ticks = 0;
  ticks++;
  return ticks;
}

//spells - local Variables 
unsigned fire_timer = 0;
unsigned water_timer = 0;
unsigned earth_timer = 0;
unsigned wind_timer = 0;



void fire_ih()
{
  if (fire_timer < 5)
  {
    fire_timer++;
  }
}

void water_ih()
{
  if (water_timer < 5)
  {
    water_timer++;
  }
}

void earth_ih()
{
  if (earth_timer < 5)
  {
    earth_timer++;
  }
}

void wind_ih()
{
  if (wind_timer < 5)
  {
    wind_timer++;
  }
}

void spell_utilities()
{
  fire_ih();
  water_ih();
  earth_ih();
  wind_ih();
}

