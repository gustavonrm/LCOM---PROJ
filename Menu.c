#include <lcom/lcf.h>
#include <stdlib.h>

#include "Menu.h"
#include "video_card.h"
#include "game.h"
#include "mouse_ih.h"
#include "keyboard.h"
#include "serial.h"

//bmps
Bitmap *Loading_Screen;
extern Bitmap *Pause_Menu;

//extern vars
extern Cursor *cursor;

//utilities
GameUtils GameMenus = {true, false, 0, true};
extern char username[20];
extern char *username_2;
extern bool Host;
extern bool MP;
extern Wizard *player;
extern Wizard *player2;
extern Bot *bot1;
extern Bot *bot2;
extern Bot *bot3;
enum player_name name_status_single = get;
enum game_result gameStatus = clean;
//enum player_name name_status_multi = empty;
bool Gamerules = false;

void main_menu()
{
     if (name_status_single == done)
     {
          if (GameMenus.main_page && Host && username_2 != NULL)
          { //If Host and got Guest Name
               GameMenus.main_page = false;
               GameMenus.run = 1;

               player = CreateWizard(Green, 512, 600, 0, username);
               bot1 = CreateBot(Blue, 200, 384, "Blue Bobs");
               bot2 = CreateBot(Red, 900, 384, "Commy");
               player2 = CreateWizard(Yellow, 512, 100, 180, username_2);
               MP = true;

               printf("multiplayer rdy\n");
          }
          //condition for single player
          if (cursor->lb == true && cursor->x >= 210 && cursor->x <= 810 && cursor->y >= 240 && cursor->y <= 390)
          {
               GameMenus.main_page = false;
               GameMenus.run = 1;
               MP = false;

               player = CreateWizard(Green, 512, 700, 0, username);
               bot1 = CreateBot(Blue, 100, 384, "Bot: Bob");
               bot2 = CreateBot(Red, 950, 384, "Bot: Commy");
               bot3 = CreateBot(Yellow, 512, 80, "Bot: Pie");
          }

          //condition for multi player
          if (cursor->lb == true && cursor->x >= 210 && cursor->x <= 810 && cursor->y >= 410 && cursor->y <= 560)
          {
               Send_Name(username);
               if (username_2 == NULL && !Host)
               { //If user is the first to send his name(a.k.a HOST)
                    Host = true;
                    //printf("\n USERNAME SENT AS HOST"); //deubg
               }
               else if (username_2 != NULL && !Host)
               { //If other player is Host
                    GameMenus.main_page = false;
                    GameMenus.run = 1;

                    bot3 = CreateBot(Green, 512, 100, "Bumbble Bee");
                    bot1 = CreateBot(Blue, 900, 384, "Blue Bobs");
                    bot2 = CreateBot(Red, 200, 384, "Commy");
                    player = CreateWizard(Yellow, 512, 600, 0, username);
                    MP = true;

                    //printf("multiplayer rdy as GUEST\n"); //deubg
               }
          }
     }
     //always do this

     //conditon to leave game
     if (cursor->lb == true && cursor->x >= 210 && cursor->x <= 810 && cursor->y >= 580 && cursor->y <= 730)
     {
          //Got Here
          GameMenus.main_page = false;
          GameMenus.game_onoff = false;
     }

     //get game rules and infos
     if (Gamerules == false)
     {
          if (cursor->lb == true && cursor->x >= 955 && cursor->x <= 1000 && cursor->y >= 705 && cursor->y <= 750)
          {
               printf("pressed game info\n ");
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

void gameCondition()
{

     if (gameStatus == clean)
     {
          return;
     }

     if (gameStatus == victory)
     {
          //estas condicoes colocar talvez qd se mudar para victory
          GameMenus.main_page = false;
          GameMenus.run = 0;

          DrawVictory();
          DrawCursor(cursor);

          if (cursor->lb == true && cursor->x >= 210 && cursor->x <= 810 && cursor->y >= 270 && cursor->y <= 410)
          {
               //Got Here
               GameMenus.main_page = true;
               GameMenus.run = 0;
               GameMenus.pause = false;
               gameStatus = clean;
          }

          if (cursor->lb == true && cursor->x >= 210 && cursor->x <= 810 && cursor->y >= 505 && cursor->y <= 650)
          {
               GameMenus.main_page = true;
               GameMenus.game_onoff = false;
          }
     }

     if (gameStatus == defeat)
     {
          // o mesmo de cima para defeat
          GameMenus.main_page = false;
          GameMenus.run = 0;

          DrawDefeat();
          DrawCursor(cursor);

          if (cursor->lb == true && cursor->x >= 210 && cursor->x <= 810 && cursor->y >= 270 && cursor->y <= 410)
          {
               //Got Here
               GameMenus.main_page = true;
               GameMenus.run = 0;
               GameMenus.pause = false;
               gameStatus = clean;
          }

           if (cursor->lb == true && cursor->x >= 210 && cursor->x <= 810 && cursor->y >= 505 && cursor->y <= 650)
          {
               GameMenus.main_page = true;
               GameMenus.game_onoff = false;
          }
     }
}
