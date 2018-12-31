#ifndef _MENU_H_ 
#define _MENU_H_

#include <lcom/lcf.h>
#include <stdlib.h>

typedef struct
{
    bool main_page; //main page aberta 
    bool pause;  //jogo em pausa
    bool game_run;  //jogo esta a correr 
    bool game_onoff;

} GameUtils;


void main_menu(); 

void DrawLoadingScreen();
void DrawPauseMenu(); 


#endif
