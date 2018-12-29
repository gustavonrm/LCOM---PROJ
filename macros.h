#ifndef _MACROS_H_
#define _MACROS_H_

#include <lcom/lcf.h>

#define BIOS_VIDEO 0x10
#define VIDEO_MODE_FUNC 0x00
#define SET_VBE_MODE 0x4F02
#define GET_VBE_MODE_INFO 0x4F01
#define GET_VBE_CTRL_INFO 0x4F00
#define TEXT_MODE 0x03
#define BIT(n) (0x01<<(n))
#define LINEAR_MODE BIT(14)
#define XPE_MODE 0x105
#define SEGMENT(n)((n) >> 16 & 0x0FFFF)
#define OFFSET(n) ((n) & 0x0FFFF)
#define BCD_TO_BIN(n) ((n & 0xF0) >> 4) * 10 + (n & 0x0F)
////////////////////////////////
//RTC stuff
////////////////////////////////
#define RTC_IRQ 8

//write to the addr_reg and then red/write from the data_reg, always
#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71

//registers
#define RTC_REG_A 10
#define RTC_REG_B 11
#define RTC_REG_C 12
#define RTC_REG_D 13

//adresses
#define RTC_SECONDS       0
#define RTC_SECONDS_ALARM 1
#define RTC_MINUTES       2
#define RTC_MINUTES_ALARM 3
#define RTC_HOURS         4
#define RTC_HOURS_ALARM   5
#define RTC_WEEK_DAY      6
#define RTC_DAY           7
#define RTC_MONTH         8
#define RTC_YEAR          9


//space to define needed bitsets on registers
    
    //REG A
#define RTC_UIP BIT(7)
    //REG B
#define RTC_SET BIT(7)
#define RTC_UIE BIT(4)
#define RTC_DM BIT(2) //to solve bcd problems and set in bin
#define RTC_TWENTY_FOUR BIT(1) //tho changing can interfere with minix
    //REG C
#define RTC_UF BIT(4)
#define RTC_AF BIT(5) //the one i need 
#define RTC_PF BIT(6)
    //REG D
//dont need anything, it just sees the LiBaterry quality 


////////////////////////////////
////////////////////////////////


#endif
