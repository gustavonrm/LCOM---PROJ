// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
// Any header files included below this line should have been created by you

#include "bitmap.h"
#include "game.h"
#include "video_card.h"
#include "keyboard.h"
#include "timer.h"
#include <math.h>

extern Bitmap *background;
Cursor *cursor;
Wizard *player;
Wizard *bot;
extern uint8_t pack;
extern uint8_t packets[3];
//keyboard
extern uint16_t key;

//globat temporary vars
extern bool Fire_Cast; 


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
  cursor = CreateCursor(500, 500);
  player = CreateWizard(Green, 560, 400, 0);
  bot = CreateWizard(Green, 200, 300, 0);

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

  uint8_t bit_no, bit_no_t, bit_no_m;

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

  uint32_t irq_kbd = BIT(bit_no), irq_timer0 = BIT(bit_no_t), irq_mouse = BIT(bit_no_m);

  while ((key != ESC_BREAK))
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
        if (msg.m_notify.interrupts & irq_timer0) //TIMER
        {
          counter = timer_ih();
          if (counter == 60)
          {
            spell_utilities();
          }
          Update_Game_State();
          UpdateVideo();
        }

        if (msg.m_notify.interrupts & irq_kbd) //KEYBOARD
        {
          key = kbd_ih();
          keyboard_utilities(key);
        }

        if (msg.m_notify.interrupts & irq_mouse) //MOUSE
        {
          mouse = mouse_int_h();
          if (mouse != NULL)
          { //if mouse recieved something useful
            cursor->press = mouse->lb;
            cursor->x += mouse->delta_x;
            cursor->y -= mouse->delta_y; //it's - becuase y coordinates are counted downawrds
            int angle = atan2(player->center_y - cursor->y, cursor->x - player->center_x) * 180 / M_PI - 90;
            if (angle < 0)
              angle = 360 + angle;

//check line 136 in the future if problems 
               player->rot = angle;


            if (!player->casting && Fire_Cast == true)
            {
              player->rot = angle;
              player->casting = cursor->press; //THIS IS ONLY TEMPORARY(So user casts when LB is pressed)
              if (player->casting )
              Fire_Cast = false; 
              player->cast_type = Fire; //TEMPORARY
              
            }
          }
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

  if (disable_stream() == 1)
  { //if it returns 1 then we should try again
    if (disable_stream() == -1)
      return 1; //if it returns -1 then there was a fatal error
  }
  return 0;
}

int(proj_main_loop)()
{
  if (!LoadAssets())
  {
    vg_exit();
    return 1;
  }

  //sleep(10);

  vg_init(0x144);

  Arena();

  vg_exit();
  return 0;
}

//argc numero de argumentos entre ""
//argv array cujos elementos sao os argumentos transatos
