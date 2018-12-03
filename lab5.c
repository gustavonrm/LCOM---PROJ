// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>
#include <machine/int86.h> // /usr/src/include/arch/i386

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include "video_card.h"
#include "keyboard.h"
//#include "sprite.h"
#include "timer.c"
//KEYBOARD//
int hook_id = 1; //used for keyboard

uint8_t scancode;
uint8_t bit_no = 0; //kbc
uint8_t bit_no_t = 1;  //timer
extern unsigned int counter;
////////////

int (video_test_init)(uint16_t mode, uint8_t delay) {
  if(vg_init(mode) == NULL) return 1;
  sleep(delay);
  if(vg_exit() != 0) return 1;
  return 0;
}

int (video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  
  vg_init(mode); //graphics_mode(mode); <-- this should be the one to use, init just for early tests
  
  if (draw_rectangle(x, y, width, height,color) != 0) return 1; 


  //keyboard read keys
  uint8_t scan_bytes[2];
  int ipc_status;
  bool make;
  int r;
  message msg;

  subscribe_kbd(&bit_no);

  while(scancode != ESC_BREAK){

    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
        printf("driver_receive failed with: %d", r);
        continue;
      }

      if (is_ipc_notify(ipc_status)) { /* received notification */
        switch (_ENDPOINT_P(msg.m_source)) {
          case HARDWARE: /* hardware interrupt notification */
          if (msg.m_notify.interrupts & BIT(bit_no)) { /* subscribed interrupt */
             kbc_ih();
          if(scancode == TWO_BYTES){
            scan_bytes[0] = scancode;
          }
          else if(scan_bytes[0] == TWO_BYTES){
            scan_bytes[1] = scancode;
            make = !((scancode & BIT(7)) >> 7); //because it's make if it's set to 0
          }
          else{
          scan_bytes[0] = scancode;
          make = !((scancode & BIT(7)) >> 7);
          }
        }
        break;
      default:
        break; /* no other notifications expected: do nothing */
      }
    }
  }
  //closure functions
  if(unsubscribe_keyboard() !=OK) return 1;

  vg_exit();

  return 0;//if succeeded
}

int (video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  if(vg_init(mode) == NULL) return 1;

  uint16_t right_bar_width, bottom_bar_width;
  right_bar_width = (getXRes() % no_rectangles);
  bottom_bar_width = (getYRes() % no_rectangles);

  uint16_t rec_width, rec_height;
  rec_width = (getXRes() / no_rectangles);
  rec_height = (getYRes() / no_rectangles);
  
  bool index_mode;

  if(getMemoryModel() == 4){  //indexed mode
    index_mode = true;
  }
  else if(getMemoryModel() == 6){  //direct mode
    index_mode = false;
  }
  uint32_t color;
  for(uint16_t row = 0; row < no_rectangles; row++){ //Paint for x nº of lines
    for(uint16_t col = 0; col < no_rectangles; col++){ //Paint for x nº of columns
      if(index_mode) color = (first + (row * no_rectangles + col) * step) % (1 << (getByts_per_pixel()*8));
      else color = direct_color(row, col, first, step);
      if(draw_rectangle(col*(rec_width),row*(rec_height),rec_width,rec_height,color)!= 0) return 1;
    }
  }

  //KEYBOARD
  uint8_t scan_bytes[2];
  bool make;
	int r;
  
  message msg;
  
  int ipc_status;
  
  subscribe_kbd(&bit_no);
  
  while(scancode != ESC_BREAK){
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) { 
        printf("driver_receive failed with: %d", r);
        continue;
    }

    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: /* hardware interrupt notification */				
          if (msg.m_notify.interrupts & BIT(bit_no)) { /* subscribed interrupt */
            kbc_ih();

            if(scancode == TWO_BYTES){
              scan_bytes[0] = scancode;
            }
            else if(scan_bytes[0] == TWO_BYTES){
              scan_bytes[1] = scancode;
              make = !((scancode & BIT(7)) >> 7); //because it's make if it's set to 0
            }
            else{
            scan_bytes[0] = scancode;
            make = !((scancode & BIT(7)) >> 7);
            }
          }
          break;
        default:
          break; /* no other notifications expected: do nothing */	
      }
    }
  }
  vg_exit();
  if(unsubscribe_keyboard() !=OK) return 1;
  return 0;
}

//////////XPM//////////

int (video_test_xpm)(const char *xpm[], uint16_t x, uint16_t y) {

  vg_init(XPE_MODE); //just to use a macros 
  draw_xpm(xpm,x,y);

  //KEYBOARD
  uint8_t scan_bytes[2];
  bool make;
	int r;
  
  message msg;
  
  int ipc_status;
  
  subscribe_kbd(&bit_no);
  
  while(scancode != ESC_BREAK){
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) { 
        printf("driver_receive failed with: %d", r);
        continue;
    }

    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: /* hardware interrupt notification */				
          if (msg.m_notify.interrupts & BIT(bit_no)) { /* subscribed interrupt */
            kbc_ih();

            if(scancode == TWO_BYTES){
              scan_bytes[0] = scancode;
            }
            else if(scan_bytes[0] == TWO_BYTES){
              scan_bytes[1] = scancode;
              make = !((scancode & BIT(7)) >> 7); //because it's make if it's set to 0
            }
            else{
            scan_bytes[0] = scancode;
            make = !((scancode & BIT(7)) >> 7);
            }
          }
          break;
        default:
          break; /* no other notifications expected: do nothing */	
      }
    }
  }
  if(unsubscribe_keyboard() !=OK) return 1;
  vg_exit(); 
  return 0;
}

int (video_test_move)(const char *xpm[], uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf, int16_t speed, uint8_t fr_rate) {
  vg_init(XPE_MODE);
  if(xi > getXRes() || xf > getXRes() || yi > getYRes() || yf > getYRes()){
    vg_exit();
    printf("\nINVALID POSITION INPUT!");
    return 1;
  }

  int delta_x = xf - xi;
  int delta_y = yf - yi;
  printf("\nDELTAY:%d",delta_y);
  printf("\nDELTAx:%d",delta_x);
  int x_sign = 0, y_sign = 0;
  if(delta_x < 0) x_sign = -1;
  else if(delta_x > 0) x_sign = 1;
  if(delta_y < 0) y_sign = -1;
  else if(delta_y > 0) y_sign = 1;

  int xspeed = 0, yspeed = 0;
  if(speed < 0){
    xspeed = 1;
    yspeed = 1;
  }
  else if(speed > 0){
    xspeed = speed;
    yspeed = speed;
  }
  xspeed *= x_sign;
  yspeed *= y_sign;
  printf("\nYspeed:%d",yspeed);
  printf("\nXspeed:%d",xspeed);

  Sprite* sp = create_sprite(xpm, xi, yi, xf, yf, xspeed, yspeed);

  uint8_t scan_bytes[2];
  bool make;
  int r;
  message msg;
  int ipc_status;
  unsigned int freq = 60/fr_rate; //frequency of updates(bcs there are 60 ticks/sec)
  int frame_n = 0;

  if (subscribe_kbd(&bit_no) != 0) {
    return 1;
  }

  if (timer_subscribe_int(&bit_no_t) != 0) {
    return 1;
  }

  uint32_t irq_kbd= BIT(bit_no), irq_timer0 = BIT(bit_no_t);

  draw_sprite(sp);
      
  while((scancode != ESC_BREAK)){
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) { 
      printf("driver_receive failed with: %d", r);
      continue;
    }    
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: /* hardware interrupt notification */				
          if (msg.m_notify.interrupts & irq_timer0) { // subscribed interrupt
            timer_int_handler();
            if(counter == freq){
              counter = 0;
              frame_n++;
              if(frame_n == -1*speed || speed > 0){
                frame_n = 0;
                if(sp->moving) draw_sprite(sp);
              }
            }
          }

          if (msg.m_notify.interrupts & irq_kbd){
            kbc_ih();
            counter = 0;

            if(scancode == TWO_BYTES){
              scan_bytes[0] = scancode;
            }
            else if(scan_bytes[0] == TWO_BYTES){
              scan_bytes[1] = scancode;
              make = !((scancode & PAR_ERR ) >> 7); //because it's make if it's set to 0
            }
            else{
              scan_bytes[0] = scancode;
              make = !((scancode & PAR_ERR ) >> 7);
            }
          }
        break;
        default:
        break; 
      }
    }
  }
  destroy_sprite(sp);
  if(timer_unsubscribe_int()!=OK) {
    return 1;
  }
  if(unsubscribe_keyboard() !=OK) return 1; 
  vg_exit();
  return 0;
}


int (video_test_controller)() {
  vbe_get_ctrl_info();  
  return 0;
}  
