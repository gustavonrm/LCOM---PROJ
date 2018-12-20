#ifndef _GAME_H_
#define _GAME_H_

#include "bitmap.h"
#include "mouse_ih.h"

typedef struct {
    Bitmap* bitmap[360];  //list of all 360 possible rotations
} Sprite;

enum Element_Type { Air, Earth, Water, Fire };
enum Wizard_color { Red, Green, Blue, Yellow };

typedef struct {
    int center_x, center_y;  //Current center position
    unsigned int rot;  //current rotation
    Sprite* img;
    int heatlh;  //current health points
    enum Wizard_color color;
} Wizard;

typedef struct {
    int center_x, center_y;  //Current center position
    unsigned int rot;  //current rotation
    Sprite* img;
    enum Element_Type type;  //element type
} Element;

typedef struct {
    int x, y;  //Current pointing positon
    bool press;
    Bitmap* pressed;
    Bitmap* released;
} Cursor;

bool LoadAssets();
Wizard* CreateWizard(enum Wizard_color color, int center_x, int center_y, unsigned int rot);
Cursor* CreateCursor(int x, int y);  //Creates Cursor obj
void DrawCursor(Cursor *cursor); //Tests for out of bounds and draws appropriate Bitmap on screen
//keayboard layout
void DrawTextBox();
void DrawTextPointer();


//timers
void DrawFireTimer(); 
void DrawWaterTimer(); 
void DrawEarthTimer(); 
void DrawWindTimer(); 
void DrawTimers(); 
#endif
