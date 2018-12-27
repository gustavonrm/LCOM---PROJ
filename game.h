#ifndef _GAME_H_
#define _GAME_H_

#include "bitmap.h"
#include "mouse_ih.h"

#define CAST_DISTANCE 80
#define FAST_SPEED 5

#define ELEMS_SIZE      30
#define WIZARDS_SIZE    4
#define BOTS_SIZE       3

#define WIZARD_HITBOX_RADIUS 50
#define BALL_HITBOX_RADIUS 28

#define LOWER_ATTENTION         1*60
#define HIGHER_ATTENTION        4*60
#define LOWER_TTF               2*60
#define HIGHER_TTF              3*60
#define MAX_WOOBLE_VAR          270 //10 degrees of variation to either side

typedef struct {
    Bitmap* bitmap[360];  //list of all 360 possible rotations
} Sprite;

typedef struct {
    Sprite* sprites[30];  //Up to 30 frames for each animation
} Animation;

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
    char* name;
} Wizard;

typedef struct {
    Wizard* wizard;
    unsigned int attention_span;  //Ticks left until target is changed
    unsigned int time_to_fire;  //Ticks left until bot fires a spell

    int target; //This will be used to determine rotation
    bool transitioning;  //True wheneven a bot is changing target acquisition
    int init_diff;  //value of initial difference when starting the transition movement
    unsigned int var_target; //target while wobbling when looking at another wizard
    enum Element_Type last_used; //This is the last casted spell by the bot
} Bot;

typedef struct {
    int center_x, center_y;  //Current center position
    unsigned int rot;  //current rotation
    Sprite* img;
    enum Element_Type elem_type;  //element type
    bool active; //True if object hasn't hit anything and is still within bounds
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

Animation* CreateAnimation(char animation_name[], int n_frames);

//players
Wizard* CreateWizard(enum Wizard_color color, int center_x, int center_y, unsigned int rot, char* name) ;
void DrawWizard(Wizard *wizard);
void Wizard_Colision(Wizard *wizard, Element *element);

//bots
Bot* CreateBot(enum Wizard_color color, int center_x, int center_y, char* name);
void Change_Target(Bot *bot);
void Bot_Cast(Bot *bot);
void Update_Bot_Rotation(Bot *bot);
void Bot_Transition(Bot *bot);
void Bot_Wobble(Bot *bot);

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
