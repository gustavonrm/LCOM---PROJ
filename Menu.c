#include <lcom/lcf.h>
#include <stdlib.h>

#include "Menu.h"
#include "video_card.h"
#include "game.h"
#include "mouse_ih.h"

Bitmap *Loading_Screen;
extern Bitmap *Pause_Menu;

extern Cursor *cursor;

GameUtils GameMenus = {true, false,0, true};
//GameUtils GameMenus = {false, false, true, false};

void main_menu()
{    

     //condition for single player 
     if(cursor->lb == true 
     && cursor->x >= 210 && cursor->x <= 810 
     && cursor->y >= 240 &&  cursor->y <=390 ){
          GameMenus.main_page = false; 
          GameMenus.run = 1; 
     }

     
     //condition for multi player 
     if(cursor->lb == true 
     && cursor->x >= 210 && cursor->x <= 810 
     && cursor->y >= 410 &&  cursor->y <=560 ){
          GameMenus.main_page = false; 
          GameMenus.run = 2; 
     }

     //conditon to leave game 
     if(cursor->lb == true 
     && cursor->x >= 210 && cursor->x <= 810 
     && cursor->y >= 580 &&  cursor->y <=730 ){
          GameMenus.main_page = false; 
          GameMenus.game_onoff = false; 
     }

}

//Draw stuff
void DrawLoadingScreen()
{
     Loading_Screen = loadBitmap("Loading_Screen.bmp");
     DrawBitmap(Loading_Screen, 0, 0);
     UpdateVideo(); 
     printf("loading screen drawn\n");
}

void DrawPauseMenu(){
     DrawBitmap(Pause_Menu, 170, 200);
}
