#ifndef _MENU_H_ 
#define _MENU_H_

#include <lcom/lcf.h>
#include <stdlib.h>

typedef struct
{
    bool main_page; //main page aberta 
    bool pause;  //jogo em pausa
    int run; //if 0 nothing, if 1 single if 2 multi 
    bool game_onoff;

} GameUtils;

enum player_name
{
    empty,
    get, 
    done 
};


void main_menu(); 

void DrawLoadingScreen();
void DrawPauseMenu(); 


#endif
