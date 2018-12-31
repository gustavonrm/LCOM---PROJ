#include <lcom/lcf.h>
#include <stdlib.h>

#include "Menu.h"
#include "video_card.h"
#include "game.h"
#include "mouse_ih.h"

Bitmap *Loading_Screen;
extern Bitmap *Pause_Menu;

extern Cursor *cursor;

GameUtils GameMenus = {true, false, false, true};
//GameUtils GameMenus = {false, false, true, false};

void main_menu()
{
     if(cursor->lb == true 
     && cursor->x >= 210 && cursor->x <= 810 
     && cursor->y >= 280 &&  cursor->y <=430 ){
          GameMenus.main_page = false; 
          GameMenus.game_run = true; 
     }
     
     if(cursor->lb == true 
     && cursor->x >= 210 && cursor->x <= 810 
     && cursor->y >= 460 &&  cursor->y <=610 ){
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
