#include <lcom/lcf.h>
#include <stdlib.h>
#include "RTC.h"
#include "macros.h"
#include "game.h"
#include "video_card.h"

//local variables
int rtc_hook_id = 8;
Time GameClock = {0, 0, 0};

extern Bitmap *Number_0;
extern Bitmap *Number_1;
extern Bitmap *Number_2;
extern Bitmap *Number_3;
extern Bitmap *Number_4;
extern Bitmap *Number_5;
extern Bitmap *Number_6;
extern Bitmap *Number_7;
extern Bitmap *Number_8;
extern Bitmap *Number_9;
extern Bitmap *Double_Dots;

//only interestd on update interupts tho we only want to get current hours

int subscribe_rtc(uint8_t *rtc_bit_no)
{
    *rtc_bit_no = rtc_hook_id;
    if (sys_irqsetpolicy(RTC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &rtc_hook_id) != OK)
    {
        printf("failed subscribing rtc interupts 1\n");
        return 1;
    }

    if (activate_update_interrupts() != 0)
    {
        printf("failed subscribing rtc interupts 3\n");
        return 1;
    }
    printf("RTC subscribed!\n");
    return 0;
}

int unsubscribe_rtc()
{
    if (sys_irqrmpolicy(&rtc_hook_id) != OK)
    {
        return 1;
    }

    if (disable_update_interrupts() != 0)
    {
        return 1;
    }
    return 0;
}

//adapted from LCOM slides
void rtc_ih()
{
    uint32_t regC = 0;

    sys_outb(RTC_ADDR_REG, RTC_REG_C);

    sys_inb(RTC_DATA_REG, &regC);

    if (regC & RTC_UF)
    {

        if (get_hours() != 0)
        {
            printf("error getting hours\n");
            return;
        }
        clock_display(); //comment this to stop outputting 
    }
}

////////
///////

int activate_update_interrupts()
{
    //get the current REG B bits
    uint32_t regB = 0;

    if (sys_outb(RTC_ADDR_REG, RTC_REG_B) != OK)
    {
        return 1;
    }
    if (sys_inb(RTC_DATA_REG, &regB) != OK)
    {
        printf("rtc_enable_interrupts::Error reading register B\n");
        return 1;
    }

    //set the update mode
    regB |= RTC_UF;
    regB &= ~RTC_SET; //see if makes a diference

    //update the register

    if (sys_outb(RTC_ADDR_REG, RTC_REG_B) != OK)
    {
        return 1;
    }

    if (sys_outb(RTC_DATA_REG, regB) != OK)
    {
        return 1;
    }

    return 0; //if success
}

int disable_update_interrupts()
{
    uint32_t regB = 0;

    if (sys_outb(RTC_ADDR_REG, RTC_REG_B) != OK)
    {
        return 1;
    }

    if (sys_inb(RTC_DATA_REG, &regB) != OK)
    {
        return 1;
    }
    //change all bits execept bit4 which is the one os the update
    regB &= ~BIT(4);

    if (sys_outb(RTC_ADDR_REG, RTC_REG_B) != OK)
    {
        return 1;
    }

    if (sys_outb(RTC_DATA_REG, regB) != OK)
    {
        return 1;
    }

    return 0;
}

int get_hours()
{

    uint32_t time = 0;

    if (sys_outb(RTC_ADDR_REG, RTC_HOURS) != OK)
    {
        return 1;
    }
    if (sys_inb(RTC_DATA_REG, &time) != OK)
    {
        return 1;
    }

    if (bcd_to_binary(&time) != 0)
    {
        printf("Error converting cbd to binary");
        return 1;
    }

    GameClock.hours = time;

    if (sys_outb(RTC_ADDR_REG, RTC_MINUTES) != OK)
    {
        return 1;
    }
    if (sys_inb(RTC_DATA_REG, &time) != OK)
    {
        return 1;
    }

    if (bcd_to_binary(&time) != 0)
    {
        printf("Error converting cbd to binary");
        return 1;
    }
    GameClock.minutes = time;

    if (sys_outb(RTC_ADDR_REG, RTC_SECONDS) != OK)
    {
        return 1;
    }
    if (sys_inb(RTC_DATA_REG, &time) != OK)
    {
        return 1;
    }

    if (bcd_to_binary(&time) != 0)
    {
        printf("Error converting cbd to binary");
        return 1;
    }
    GameClock.seconds = time;

    return 0;
}

void clock_display()
{
    correct_hours();
    // to test display with printf
    int Hour, Min, Sec;
    Hour = GameClock.hours;
    Min = GameClock.minutes;
    Sec = GameClock.seconds;

    printf("%d:", Hour);
    printf("%d:", Min);
    printf("%d\n", Sec);

    //later with in game bmp
}

int bcd_to_binary(uint32_t *time)
{
    uint32_t regB = 0;
    if (sys_outb(RTC_ADDR_REG, RTC_REG_B) != OK)
    {
        return 1;
    }
    if (sys_inb(RTC_DATA_REG, &regB) != OK)
    {
        return 1;
    }

    if (!(regB & RTC_DM)) //compare if its in binary
    {
        *time = BCD_TO_BIN(*time);
    }
    return 0; //if success
}

void correct_hours()
{
    if (GameClock.hours == 81)
        GameClock.hours = 13;

    if (GameClock.hours == 82)
        GameClock.hours = 14;

    if (GameClock.hours == 83)
        GameClock.hours = 15;

    if (GameClock.hours == 84)
        GameClock.hours = 16;

    if (GameClock.hours == 85)
        GameClock.hours = 17;

    if (GameClock.hours == 86)
        GameClock.hours = 18;

    if (GameClock.hours == 87)
        GameClock.hours = 19;

    if (GameClock.hours == 88)
        GameClock.hours = 20;

    if (GameClock.hours == 89)
        GameClock.hours = 21;

    if (GameClock.hours == 90)
        GameClock.hours = 22;

    if (GameClock.hours == 91)
        GameClock.hours = 23;

    if (GameClock.hours == 92)
        GameClock.hours = 24;
}

void DrawClock()
{
    //draw hour

    if (GameClock.hours / 10 == 0)
    {
        DrawBitmap(Number_0, 10, 10);
        switch (GameClock.hours % 10)
        {
        case 0:
            DrawBitmap(Number_0, 20, 10);
            break;

        case 1:
            DrawBitmap(Number_1, 20, 10);
            break;

        case 2:
            DrawBitmap(Number_2, 20, 10);
            break;

        case 3:
            DrawBitmap(Number_3, 20, 10);
            break;

        case 4:
            DrawBitmap(Number_4, 20, 10);
            break;

        case 5:
            DrawBitmap(Number_5, 20, 10);
            break;

        case 6:
            DrawBitmap(Number_6, 20, 10);
            break;

        case 7:
            DrawBitmap(Number_7, 20, 10);
            break;

        case 8:
            DrawBitmap(Number_8, 20, 10);
            break;

        case 9:
            DrawBitmap(Number_9, 20, 10);
            break;
        }
    }

    if (GameClock.hours / 10 == 1)
    {
        DrawBitmap(Number_1, 10, 10);
        switch (GameClock.hours % 10)
        {
        case 0:
            DrawBitmap(Number_0, 20, 10);
            break;

        case 1:
            DrawBitmap(Number_1, 20, 10);
            break;

        case 2:
            DrawBitmap(Number_2, 20, 10);
            break;

        case 3:
            DrawBitmap(Number_3, 20, 10);
            break;

        case 4:
            DrawBitmap(Number_4, 20, 10);
            break;

        case 5:
            DrawBitmap(Number_5, 20, 10);
            break;

        case 6:
            DrawBitmap(Number_6, 20, 10);
            break;

        case 7:
            DrawBitmap(Number_7, 20, 10);
            break;

        case 8:
            DrawBitmap(Number_8, 20, 10);
            break;

        case 9:
            DrawBitmap(Number_9, 20, 10);
            break;
        }
    }

    if (GameClock.hours / 10 == 2)
    {
        DrawBitmap(Number_2, 10, 10);
        switch (GameClock.hours % 10)
        {
        case 0:
            DrawBitmap(Number_0, 20, 10);
            break;

        case 1:
            DrawBitmap(Number_1, 20, 10);
            break;

        case 2:
            DrawBitmap(Number_2, 20, 10);
            break;

        case 3:
            DrawBitmap(Number_3, 20, 10);
            break;

        case 4:
            DrawBitmap(Number_4, 20, 10);
            break;

        }
    }

    DrawBitmap(Double_Dots, 30, 12);

    //draw min
    if (GameClock.minutes / 10 == 0)
    {
        DrawBitmap(Number_0, 35, 10);
        switch (GameClock.minutes % 10)
        {
        case 0:
            DrawBitmap(Number_0, 45, 10);
            break;
        case 1:
            DrawBitmap(Number_1, 45, 10);
            break;
        case 2:
            DrawBitmap(Number_2, 45, 10);
            break;
        case 3:
            DrawBitmap(Number_3, 45, 10);
            break;
        case 4:
            DrawBitmap(Number_4, 45, 10);
            break;
        case 5:
            DrawBitmap(Number_5, 45, 10);
            break;
        case 6:
            DrawBitmap(Number_6, 45, 10);
            break;
        case 7:
            DrawBitmap(Number_7, 45, 10);
            break;
        case 8:
            DrawBitmap(Number_8, 45, 10);
            break;
        case 9:
            DrawBitmap(Number_9, 45, 10);
            break;
        }
    }

    if (GameClock.minutes / 10 == 1)
    {
        DrawBitmap(Number_1, 35, 10);
        switch (GameClock.minutes % 10)
        {
        case 0:
            DrawBitmap(Number_0, 45, 10);
            break;
        case 1:
            DrawBitmap(Number_1, 45, 10);
            break;
        case 2:
            DrawBitmap(Number_2, 45, 10);
            break;
        case 3:
            DrawBitmap(Number_3, 45, 10);
            break;
        case 4:
            DrawBitmap(Number_4, 45, 10);
            break;
        case 5:
            DrawBitmap(Number_5, 45, 10);
            break;
        case 6:
            DrawBitmap(Number_6, 45, 10);
            break;
        case 7:
            DrawBitmap(Number_7, 45, 10);
            break;
        case 8:
            DrawBitmap(Number_8, 45, 10);
            break;
        case 9:
            DrawBitmap(Number_9, 45, 10);
            break;
        }
    }

    if (GameClock.minutes / 10 == 2)
    {
        DrawBitmap(Number_2, 35, 10);
        switch (GameClock.minutes % 10)
        {
        case 0:
            DrawBitmap(Number_0, 45, 10);
            break;
        case 1:
            DrawBitmap(Number_1, 45, 10);
            break;
        case 2:
            DrawBitmap(Number_2, 45, 10);
            break;
        case 3:
            DrawBitmap(Number_3, 45, 10);
            break;
        case 4:
            DrawBitmap(Number_4, 45, 10);
            break;
        case 5:
            DrawBitmap(Number_5, 45, 10);
            break;
        case 6:
            DrawBitmap(Number_6, 45, 10);
            break;
        case 7:
            DrawBitmap(Number_7, 45, 10);
            break;
        case 8:
            DrawBitmap(Number_8, 45, 10);
            break;
        case 9:
            DrawBitmap(Number_9, 45, 10);
            break;
        }
    }

    if (GameClock.minutes / 10 == 3)
    {
        DrawBitmap(Number_3, 35, 10);
        switch (GameClock.minutes % 10)
        {
        case 0:
            DrawBitmap(Number_0, 45, 10);
            break;
        case 1:
            DrawBitmap(Number_1, 45, 10);
            break;
        case 2:
            DrawBitmap(Number_2, 45, 10);
            break;
        case 3:
            DrawBitmap(Number_3, 45, 10);
            break;
        case 4:
            DrawBitmap(Number_4, 45, 10);
            break;
        case 5:
            DrawBitmap(Number_5, 45, 10);
            break;
        case 6:
            DrawBitmap(Number_6, 45, 10);
            break;
        case 7:
            DrawBitmap(Number_7, 45, 10);
            break;
        case 8:
            DrawBitmap(Number_8, 45, 10);
            break;
        case 9:
            DrawBitmap(Number_9, 45, 10);
            break;
        }
    }

    if (GameClock.minutes / 10 == 4)
    {
        DrawBitmap(Number_4, 35, 10);
        switch (GameClock.minutes % 10)
        {
        case 0:
            DrawBitmap(Number_0, 45, 10);
            break;
        case 1:
            DrawBitmap(Number_1, 45, 10);
            break;
        case 2:
            DrawBitmap(Number_2, 45, 10);
            break;
        case 3:
            DrawBitmap(Number_3, 45, 10);
            break;
        case 4:
            DrawBitmap(Number_4, 45, 10);
            break;
        case 5:
            DrawBitmap(Number_5, 45, 10);
            break;
        case 6:
            DrawBitmap(Number_6, 45, 10);
            break;
        case 7:
            DrawBitmap(Number_7, 45, 10);
            break;
        case 8:
            DrawBitmap(Number_8, 45, 10);
            break;
        case 9:
            DrawBitmap(Number_9, 45, 10);
            break;
        }
    }

    if (GameClock.minutes / 10 == 5)
    {
        DrawBitmap(Number_5, 35, 10);
        switch (GameClock.minutes % 10)
        {
        case 0:
            DrawBitmap(Number_0, 45, 10);
            break;
        case 1:
            DrawBitmap(Number_1, 45, 10);
            break;
        case 2:
            DrawBitmap(Number_2, 45, 10);
            break;
        case 3:
            DrawBitmap(Number_3, 45, 10);
            break;
        case 4:
            DrawBitmap(Number_4, 45, 10);
            break;
        case 5:
            DrawBitmap(Number_5, 45, 10);
            break;
        case 6:
            DrawBitmap(Number_6, 45, 10);
            break;
        case 7:
            DrawBitmap(Number_7, 45, 10);
            break;
        case 8:
            DrawBitmap(Number_8, 45, 10);
            break;
        case 9:
            DrawBitmap(Number_9, 45, 10);
            break;
        }
    }

    if (GameClock.minutes == 60)
    {
        DrawBitmap(Number_6, 35, 10);
        DrawBitmap(Number_0, 45, 10);
    }

    DrawBitmap(Double_Dots, 55, 12);

    //draw sec

    if ((GameClock.seconds / 10) == 0)
    {
        DrawBitmap(Number_0, 60, 10);
        switch (GameClock.seconds % 10)
        {
        case 0:
            DrawBitmap(Number_0, 70, 10);
            break;
        case 1:
            DrawBitmap(Number_1, 70, 10);
            break;
        case 2:
            DrawBitmap(Number_2, 70, 10);
            break;
        case 3:
            DrawBitmap(Number_3, 70, 10);
            break;
        case 4:
            DrawBitmap(Number_4, 70, 10);
            break;
        case 5:
            DrawBitmap(Number_5, 70, 10);
            break;
        case 6:
            DrawBitmap(Number_6, 70, 10);
            break;
        case 7:
            DrawBitmap(Number_7, 70, 10);
            break;
        case 8:
            DrawBitmap(Number_8, 70, 10);
            break;
        case 9:
            DrawBitmap(Number_9, 70, 10);
            break;
        }
    }
    if ((GameClock.seconds / 10) == 1)
    {
        DrawBitmap(Number_1, 60, 10);
        switch (GameClock.seconds % 10)
        {
        case 0:
            DrawBitmap(Number_0, 70, 10);
            break;
        case 1:
            DrawBitmap(Number_1, 70, 10);
            break;
        case 2:
            DrawBitmap(Number_2, 70, 10);
            break;
        case 3:
            DrawBitmap(Number_3, 70, 10);
            break;
        case 4:
            DrawBitmap(Number_4, 70, 10);
            break;
        case 5:
            DrawBitmap(Number_5, 70, 10);
            break;
        case 6:
            DrawBitmap(Number_6, 70, 10);
            break;
        case 7:
            DrawBitmap(Number_7, 70, 10);
            break;
        case 8:
            DrawBitmap(Number_8, 70, 10);
            break;
        case 9:
            DrawBitmap(Number_9, 70, 10);
            break;
        }
    }
    if ((GameClock.seconds / 10) == 2)
    {
        DrawBitmap(Number_2, 60, 10);
        switch (GameClock.seconds % 10)
        {
        case 0:
            DrawBitmap(Number_0, 70, 10);
            break;
        case 1:
            DrawBitmap(Number_1, 70, 10);
            break;
        case 2:
            DrawBitmap(Number_2, 70, 10);
            break;
        case 3:
            DrawBitmap(Number_3, 70, 10);
            break;
        case 4:
            DrawBitmap(Number_4, 70, 10);
            break;
        case 5:
            DrawBitmap(Number_5, 70, 10);
            break;
        case 6:
            DrawBitmap(Number_6, 70, 10);
            break;
        case 7:
            DrawBitmap(Number_7, 70, 10);
            break;
        case 8:
            DrawBitmap(Number_8, 70, 10);
            break;
        case 9:
            DrawBitmap(Number_9, 70, 10);
            break;
        }
    }
    if ((GameClock.seconds / 10) == 3)
    {
        DrawBitmap(Number_3, 60, 10);
        switch (GameClock.seconds % 10)
        {
        case 0:
            DrawBitmap(Number_0, 70, 10);
            break;
        case 1:
            DrawBitmap(Number_1, 70, 10);
            break;
        case 2:
            DrawBitmap(Number_2, 70, 10);
            break;
        case 3:
            DrawBitmap(Number_3, 70, 10);
            break;
        case 4:
            DrawBitmap(Number_4, 70, 10);
            break;
        case 5:
            DrawBitmap(Number_5, 70, 10);
            break;
        case 6:
            DrawBitmap(Number_6, 70, 10);
            break;
        case 7:
            DrawBitmap(Number_7, 70, 10);
            break;
        case 8:
            DrawBitmap(Number_8, 70, 10);
            break;
        case 9:
            DrawBitmap(Number_9, 70, 10);
            break;
        }
    }
    if ((GameClock.seconds / 10) == 4)
    {
        DrawBitmap(Number_4, 60, 10);
        switch (GameClock.seconds % 10)
        {
        case 0:
            DrawBitmap(Number_0, 70, 10);
            break;
        case 1:
            DrawBitmap(Number_1, 70, 10);
            break;
        case 2:
            DrawBitmap(Number_2, 70, 10);
            break;
        case 3:
            DrawBitmap(Number_3, 70, 10);
            break;
        case 4:
            DrawBitmap(Number_4, 70, 10);
            break;
        case 5:
            DrawBitmap(Number_5, 70, 10);
            break;
        case 6:
            DrawBitmap(Number_6, 70, 10);
            break;
        case 7:
            DrawBitmap(Number_7, 70, 10);
            break;
        case 8:
            DrawBitmap(Number_8, 70, 10);
            break;
        case 9:
            DrawBitmap(Number_9, 70, 10);
            break;
        }
    }
    if ((GameClock.seconds / 10) == 5)
    {
        DrawBitmap(Number_5, 60, 10);
        switch (GameClock.seconds % 10)
        {
        case 0:
            DrawBitmap(Number_0, 70, 10);
            break;
        case 1:
            DrawBitmap(Number_1, 70, 10);
            break;
        case 2:
            DrawBitmap(Number_2, 70, 10);
            break;
        case 3:
            DrawBitmap(Number_3, 70, 10);
            break;
        case 4:
            DrawBitmap(Number_4, 70, 10);
            break;
        case 5:
            DrawBitmap(Number_5, 70, 10);
            break;
        case 6:
            DrawBitmap(Number_6, 70, 10);
            break;
        case 7:
            DrawBitmap(Number_7, 70, 10);
            break;
        case 8:
            DrawBitmap(Number_8, 70, 10);
            break;
        case 9:
            DrawBitmap(Number_9, 70, 10);
            break;
        }
        if (GameClock.seconds == 60)
        {
            DrawBitmap(Number_6, 60, 10);
            DrawBitmap(Number_0, 70, 10);
        }
    }
}
