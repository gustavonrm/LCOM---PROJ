#ifndef _RTC_H_
#define _RTC_H_

#include <lcom/lcf.h>
#include <stdlib.h>

typedef struct{
    int seconds; 
    int minutes; 
    int hours; 
}Time;

int subscribe_rtc(uint8_t *rtc_bit_no); 
int unsubscribe_rtc(); 
void rtc_ih(); 

//update mode
int activate_update_interrupts();
int disable_update_interrupts(); //try if it makes a diference 
void handle_update_int();


void wait_valid_rtc();
int get_hours(); 


//utilitie functions 
int bcd_to_binary(uint32_t *time); 
void correct_hours(); 

//functions for the game 
void clock_display(); 
void adjust_backgorund(); //this could be diferent 
void DrawClock(); 


#endif
