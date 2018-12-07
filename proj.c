// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
// Any header files included below this line should have been created by you

#include "bitmap.h"
#include "game.h"
#include "video_card.h"
#include "keyboard.h"
#include "timer.h"

extern Bitmap* background;

int main(int argc, char *argv[]) {
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

int Arena(){

  Wizard* player = CreateWizard(Green, 560, 400, 0);
  DrawSprite(player->img, player->center_x, player->center_y, player->rot);
  UpdateVideo();
  
  int counter = 0;
  uint16_t key = 0;

  int r;
  message msg;
  int ipc_status;
  //unsigned int freq = 1; //frequency of updates(bcs there are 60 ticks/sec)
  //int frame_n = 0;

  uint8_t bit_no, bit_no_t;

  if (subscribe_kbd(&bit_no) != 0) {
    return 1;
  }

  if (timer_subscribe_int(&bit_no_t) != 0) {
    return 1;
  }

  uint32_t irq_kbd= BIT(bit_no), irq_timer0 = BIT(bit_no_t);
      
  while((key != ESC_BREAK)){
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) { 
      printf("driver_receive failed with: %d", r);
      continue;
    }    
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: /* hardware interrupt notification */
          if (msg.m_notify.interrupts & irq_timer0) { // subscribed interrupt
            
            counter = timer_ih();
            //UpdateVideo();
            
          }

          if (msg.m_notify.interrupts & irq_kbd){
            
            key = kbd_ih();
          
          }
        break;
        default:
        break; 
      }
    }
  }

  if(timer_unsubscribe_int()!=OK) {
    return 1;
  }
  if(unsubscribe_keyboard() !=OK) return 1; 
  return 0;
}


int (proj_main_loop)() {
  vg_init(0x144);

  if(!LoadAssets()){
    vg_exit();
    return 1;
  }

  DrawBitmap(background,0,0);

  Arena();

  vg_exit(); 
  return 0;
}


//argc numero de argumentos entre ""
//argv array cujos elementos sao os argumentos transatos 
