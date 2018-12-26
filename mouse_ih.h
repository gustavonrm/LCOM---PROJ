#ifndef _MOUSE_IH_H_
#define _MOUSE_IH_H_

#include <lcom/lcf.h>
#include "i8042.h"

int subscribe_mouse(uint8_t *bit_no);
int unsubscribe_mouse();
int enable_stream();
int disable_stream();
struct packet* mouse_int_h();
void parse_packet(struct packet *pp, uint8_t packets[3]);

int (disable_mouse_IH)();
int (mouse_write)();
int read_pcktB();
void read_data();

uint32_t write_kbccmd(uint32_t kbccmd);
uint32_t write_cmd(uint32_t cmd);
uint32_t read_cmd(uint32_t *cmd);
int (clear_kbd_outBuf)();

//STATE MACHINE 
typedef enum {  // we must draw an inverted V
    INIT=0, //initial state 
    DRAWUP, //first inv v line 
    MID, //similar to initial state 
    DRAWDOWN,  // second inv v line 
    FINAL_STATE 
} States;

typedef enum {
    LEFT_PRESSED, //press left key    
    LEFT_RELEASED, //release left key 
    RIGHT_PRESSED, //same for right key 
    RIGHT_RELEASED,  //same for right key 
    BUTTON_OTHER, //any other buttons event 
    MOUSE_MOVE //draw
}  mouse_ev_t;
/*
typedef enum {
    LB_PRESSED, //press left key    
    LB_REALEASED, //release left key 
    RB_PRESSED, //same for right key 
    RB_RELEASED,  //same for right key 
    BUTTON_EV, //any other buttons event 
    MOUSE_MOV //draw
}  mouse_ev_t;*/

//state
//implent stryuct if needed 
//event
/*
typedef struct mouse_ev {
    int16_t delta_x;
    int16_t delta_y; 
	mouse_ev_t event;
};*/

void state_machine(struct mouse_ev *evt, uint8_t x_len ,uint8_t tolerance);
struct mouse_ev* update_event_type(struct packet *pp);
int mouse_cursor(/*int* x,int* y,*/ bool* LB); 

#endif
