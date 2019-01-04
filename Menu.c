#include <lcom/lcf.h>
#include <stdlib.h>

#include "Menu.h"
#include "video_card.h"
#include "game.h"
#include "mouse_ih.h"
#include "keyboard.h"

//bmps
Bitmap *Loading_Screen;
extern Bitmap *Pause_Menu;

//extern vars
extern Cursor *cursor;

//utilities
GameUtils GameMenus = {true, false, 0, true};
char name[20] = "";
enum player_name name_status_single = empty;
enum player_name name_status_multi = empty;
bool Gamerules = false;

void main_menu()
{

     //condition for single player
     if (cursor->lb == true && cursor->x >= 210 && cursor->x <= 810 && cursor->y >= 240 && cursor->y <= 390)
     {
          name_status_single = get;
     }

     if (name_status_single == done)
     {
          name_status_single = empty;
          GameMenus.main_page = false;
          GameMenus.run = 1;
     }

     //condition for multi player
     if (cursor->lb == true && cursor->x >= 210 && cursor->x <= 810 && cursor->y >= 410 && cursor->y <= 560)
     {
          name_status_multi = get;
     }

     if (name_status_multi == done)
     {
          name_status_multi = empty;
          GameMenus.main_page = false;
          GameMenus.run = 2;
          printf("multiplayer rdy\n");
     }

     //conditon to leave game
     if (cursor->lb == true && cursor->x >= 210 && cursor->x <= 810 && cursor->y >= 580 && cursor->y <= 730)
     {
          GameMenus.main_page = false;
          GameMenus.game_onoff = false;
     }

     //get game rules and infos
     if (Gamerules == false)
     {
          if (cursor->lb == true && cursor->x >= 955 && cursor->x <= 1000 && cursor->y >= 705 && cursor->y <= 750)
          {
               printf( "pressed game info\n "); 
               Gamerules = true;
          }
     }

      if (Gamerules == true)
     {
          if (cursor->lb == true && cursor->x >= 820 && cursor->x <= 850 && cursor->y >= 115 && cursor->y <= 150)
          {
               Gamerules = false;
          }
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

void DrawPauseMenu()
{
     DrawBitmap(Pause_Menu, 170, 200);
}
