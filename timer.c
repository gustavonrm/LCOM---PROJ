#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"
#include "game.h"

unsigned int ticks = 0;
int timer_hook_id = 0; //used for timer

SpellCast SpellsRdy = {0, 0, 0, 0, 0, 5, 5, 5, 5, 3}; 

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

void fire_ih()
{
  if (SpellsRdy.fire_timer < 5)
  {
    SpellsRdy.fire_timer++;
  }
}

void water_ih()
{
  if (SpellsRdy.water_timer < 5)
  {
    SpellsRdy.water_timer++;
  }
}

void earth_ih()
{
  if (SpellsRdy.earth_timer < 5)
  {
    SpellsRdy.earth_timer++;
  }
}

void wind_ih()
{
  if (SpellsRdy.air_timer < 5)
  {
    SpellsRdy.air_timer++;
  }
}

void leap_ih()
{
  if (SpellsRdy.leap_timer < 3)
  {
    SpellsRdy.leap_timer++;
  }
}

void spell_utilities()
{
  fire_ih();
  water_ih();
  earth_ih();
  wind_ih();

  leap_ih(); 
}

