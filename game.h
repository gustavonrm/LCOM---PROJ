#ifndef _GAME_H_
#define _GAME_H_

#include "bitmap.h"
#include "mouse_ih.h"

#define CAST_DISTANCE 80
#define FAST_SPEED 5

#define ELEMS_SIZE 30
#define WIZARDS_SIZE 4

#define WIZARD_HITBOX_RADIUS 50
#define BALL_HITBOX_RADIUS 28

typedef struct
{
    Bitmap *bitmap[360]; //list of all 360 possible rotations
} Sprite;

enum Element_Type
{
    Air = 1,
    Earth = -4,
    Water = -2,
    Fire = 3,
    Null = 0
};
enum Wizard_color
{
    Red,
    Green,
    Blue,
    Yellow
};

typedef struct
{
    int center_x, center_y; //Current center position
    unsigned int rot;       //current rotation
    Sprite *img;
    int health;   //current health points
    bool casting; //True if wizard is casting a spell
    enum Element_Type cast_type;
    enum Wizard_color color;
} Wizard;

typedef struct
{
    int center_x, center_y; //Current center position
    unsigned int rot;       //current rotation
    Sprite *img;
    enum Element_Type elem_type; //element type
    bool active;                 //True if object hasn't hit anything and is still within bounds
} Element;

typedef struct
{
    int x, y; //Current pointing positon
    bool press;
    Bitmap *pressed;
    Bitmap *released;
} Cursor;

typedef struct
{

    bool Fire_Cast;
    bool Water_Cast;
    bool Earth_Cast;
    bool Air_Cast;
    unsigned fire_timer;
    unsigned water_timer;
    unsigned earth_timer;
     unsigned air_timer;
}SpellCast;

//main layout
bool
LoadAssets();
void DrawToolBox();

//players
Wizard *CreateWizard(enum Wizard_color color, int center_x, int center_y, unsigned int rot);
void DrawWizard(Wizard *wizard);
void Wizard_Colision(Wizard *wizard, Element *element);

//elements
Element *CreateElement(enum Element_Type type, int center_x, int center_y, unsigned int rot);
void DrawElement(Element *element);
void Move_Element(Element *element);
void Element_Colision(Element *element1, Element *element2);

//cursors
Cursor *CreateCursor(int x, int y); //Creates Cursor obj
void DrawCursor(Cursor *cursor);    //Tests for out of bounds and draws appropriate Bitmap on screen

//keayboard layout
void DrawTextBox();
void DrawTextPointer();

//timers
void DrawFireTimer();
void DrawWaterTimer();
void DrawEarthTimer();
void DrawWindTimer();
void DrawTimers();

//video
void Update_Game_State();

#endif
