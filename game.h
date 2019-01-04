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

#define LOWER_ATTENTION         (int) 1.5*60
#define HIGHER_ATTENTION        (int) 4.5*60
#define LOWER_TTF               3*60
#define HIGHER_TTF              5*60
#define MAX_WOOBLE_VAR          270 //10 degrees of variation to either side

#define SPELL_TIMER             60

typedef struct {
    Bitmap* bitmap[360];  //list of all 360 possible rotations
} Sprite;

typedef struct {
    Sprite* sprites[30];  //Up to 30 frames for each animation
    int n_frames;
    int ticks_between_frames;
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

enum Spell_Type
{
    Launch = 1,
    //Control,
    //Circle,
    //Aura,
    None = 0
};

typedef struct
{
    int center_x, center_y; //Current center position
    unsigned int rot;       //current rotation
    Sprite *img;
    int health;   //current health points
    
    bool casting; //True if wizard is casting a spell (and can't do anything else)
    enum Element_Type cast_type;
    enum Spell_Type spell;
    Animation* cast_animation;
    int frame_n;
    int try_n;
    
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
    enum Spell_Type spell_type;  //Type of spell
    bool active; //True if object hasn't hit anything
    int frame_n;
    int try_n;

    bool destroyed;

    int array_position;

} Element;

typedef struct
{
    int x, y; //Current pointing positon
    bool lb;
    bool rb;
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
bool LoadAssets(); //Loads all of the bitmaps and saves them in sprites, bmps or animations
void DrawToolBox();

//animation
Animation* CreateAnimation(char animation_name[], int n_frames, int ticks_per_frame); //Used to load animations
void Get_Animation(Wizard* wizard); //Get correct casting animation for a wizard
void Draw_Animation(Animation* animation, int center_x, int center_y, int frame_n, int *try_n, unsigned int rot);

//players
void Player_Cast(Wizard* player, Cursor* cursor); //Handles spell casting for players
enum Spell_Type Get_Spell_Type(Cursor* cursor); //Gets what type of spell to cast depending on the cursor attributes
bool Check_Cursor(Cursor* cursor, enum Spell_Type spell_type);

Wizard* CreateWizard(enum Wizard_color color, int center_x, int center_y, unsigned int rot, char* name) ;
void DrawWizard(Wizard *wizard);
void Wizard_Colision(Wizard *wizard, Element *element);
void Draw_Cast(Wizard* wizard);

//bots
Bot* CreateBot(enum Wizard_color color, int center_x, int center_y, char* name);
void Change_Target(Bot *bot);
void Bot_Cast(Bot *bot);
void Update_Bot_Rotation(Bot *bot);
void Bot_Transition(Bot *bot);
void Bot_Wobble(Bot *bot);

//elements
Element *CreateElement(Wizard* wizard);
void DrawElement(Element *element);
void Move_Element(Element *element);
void Element_Colision(Element *element1, Element *element2);
void Draw_Destruction(Element* element);
bool Out_Of_Bounds(Element* element);    

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

//Multiplayer
void Send_Game_Info();
Element* Create_Guest_Element(int array_pos, int8_t elem_type, uint8_t spell_type);

#endif
