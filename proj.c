// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
// Any header files included below this line should have been created by you

#include "bitmap.h"
#include "game.h"
#include "video_card.h"
#include "keyboard.h"
#include "timer.h"
#include "RTC.h"
#include "Menu.h"
#include <math.h>

extern Bitmap *background;
Cursor *cursor;
Wizard *player;
Bot *bot1;
Bot *bot2;
Bot *bot3;
 
extern uint8_t pack;
extern uint8_t packets[3];
//keyboard
extern uint16_t key;
//utils
extern GameUtils GameMenus; 

//globat temporary vars

int main(int argc, char *argv[])
{
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/proj/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/proj/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int Arena()
{
  cursor = CreateCursor(512, 500);
  player = CreateWizard(Green, 512, 600, 0, "ALEX");
  bot1 = CreateBot(Blue, 200, 384, "Blue Bobs");
  bot2 = CreateBot(Red, 900, 384, "Commy");
  bot3 = CreateBot(Yellow, 512, 100, "Bumbble Bee");

  Update_Game_State();
  UpdateVideo();

  int counter = 0;
  uint16_t key = 0;
  struct packet *mouse;

  int r;
  message msg;
  int ipc_status;
  //unsigned int freq = 1; //frequency of updates(bcs there are 60 ticks/sec)
  //int frame_n = 0;

  uint8_t bit_no, bit_no_t, bit_no_m, bit_no_rtc;

  if (enable_stream() == 1)
  {
    if (enable_stream() == -1)
      return 1;
  }

  if (subscribe_kbd(&bit_no) != 0)
  {
    return 1;
  }

  if (timer_subscribe_int(&bit_no_t) != 0)
  {
    return 1;
  }

  if (subscribe_mouse(&bit_no_m) != 0)
  {
    return 1;
  }

  //subscribe RTC
  if (subscribe_rtc(&bit_no_rtc) != 0)
  {
    return 1;
  }

  //....

  uint32_t irq_kbd = BIT(bit_no), irq_timer0 = BIT(bit_no_t), irq_mouse = BIT(bit_no_m), irq_rtc = BIT(bit_no_rtc);

   while (GameMenus.game_onoff == true)
  {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0)
    {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status))
    { /* received notification */
      switch (_ENDPOINT_P(msg.m_source))
      {
      case HARDWARE:
        if (msg.m_notify.interrupts & irq_timer0)
        { //TIMER
          counter = timer_ih();
          if (counter == 60)
          {
            spell_utilities();
          }
          Update_Game_State();
          UpdateVideo();
        }

        if (msg.m_notify.interrupts & irq_kbd)
        { //KEYBOARD
          key = kbd_ih();
          keyboard_utilities(key);
        }

        if (msg.m_notify.interrupts & irq_mouse)
        { //MOUSE
          mouse = mouse_int_h();
          if (mouse != NULL)
          { //if mouse recieved something useful
            cursor->lb = mouse->lb;
            cursor->rb = mouse->rb;
            cursor->x += mouse->delta_x;
            cursor->y -= mouse->delta_y; //it's - becuase y coordinates are counted downwards


          }
        }
        //RTC
        if (msg.m_notify.interrupts & irq_rtc){
            rtc_ih(); 
        }
        

        break;
      default:
        break;
      }
    }
  }

  if (unsubscribe_mouse() != 0)
  {
    return 1;
  }

  if (timer_unsubscribe_int() != OK)
  {
    return 1;
  }
  if (unsubscribe_keyboard() != OK)
  {
    return 1;
  }
  //RTC
  if( unsubscribe_rtc() != OK){
    return 1; 
  }

  if (disable_stream() == 1)
  { //if it returns 1 then we should try again
    if (disable_stream() == -1)
      return 1; //if it returns -1 then there was a fatal error
  }
  return 0;
}


int(proj_main_loop)()
{ 
  vg_init(0x144);
  
  DrawLoadingScreen(); 

  if (!LoadAssets())
  {
    vg_exit();
    return 1;
  }

  //sleep(10);

  time_t t;
  srand((unsigned)time(&t));
  Arena();

  vg_exit();
  return 0;
}

//argc numero de argumentos entre ""
//argv array cujos elementos sao os argumentos transatos
