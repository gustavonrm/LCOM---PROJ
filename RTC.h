#ifndef _RTC_H_
#define _RTC_H_

#include <lcom/lcf.h>
#include <stdlib.h>

typedef struct{
    int seconds; 
    int minutes; 
    int hours; 
}Time;

typedef struct{
    int hour1_x; 
    int hour1_y; 
    int hour2_x; 
    int hour2_y; 
    int dd1_x;
    int dd1_y;
    int minute1_x; 
    int minute1_y; 
    int minute2_x; 
    int minute2_y; 
    int dd2_x;
    int dd2_y;
    int second1_x;
    int second1_y;
    int second2_x;
    int second2_y;
}ClockPos; //struct to change clock positioning 

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
