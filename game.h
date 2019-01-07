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
#define LEAP_DISTANCE           70
#define LEAP_SPEED              8 //distance per frame

#define TOTAL_BITMAPS           10000

/**
 * @brief A Sprite is a strcut that contains 360 bitmaps (one for each angle degree)
 * @param bitmap[360] array of bitmaps
 */
typedef struct {
    Bitmap* bitmap[360];  //list of all 360 possible rotations
} Sprite;

/**
 * @brief An animation is a struct that may have up to 30 frames (bitmaps or sprites) and also stores the information needed for timing the animation
 * @param sprites[30] Array of up to 30 frames of sprites
 * @param ticks_between_frames Used to time the animation
 * @param n_frames total number of frames the animation has
 * @param bitmaps[30] Array of up to 30 frames of bitmaps
 * @param rotates boolean that indicates whether this animation has rotation or not
 */
typedef struct {
    Sprite* sprites[30];  //Up to 30 frames for each animation
    int n_frames;
    int ticks_between_frames;
    bool rotates;
    Bitmap* bitmaps[30]; //In case the Animation doesn't rotate
} Animation;

/**
 * @brief enum that indicates the type of each element(used for collisions, animations and casting)
 */
enum Element_Type
{
    Air = 1,
    Earth = -4,
    Water = -2,
    Fire = 3,
    Null = 0
};

/**
 * @brief enum that indicates the color of a wizard (used for Finding the right sprite)
 */
enum Wizard_color
{
    Red,
    Green,
    Blue,
    Yellow
};

/**
 * @brief enum that indicates what type of spell the wizard is casting
 */
enum Spell_Type
{
    Launch = 1,
    Leap = 2,
    None = 0
};

/**
 * @brief Struct that stores all the important information for the Wizard to be managed
 */
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
    int leap_xf, leap_yf;
} Wizard;

/**
 * @brief Struct that stores all the information for the Bot to control it's wizard
 */
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
/**
 * @brief Struct that stores all information needed to update the Element movement and check it's destruction state
 */
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

/**
 * @brief Struct that stores the cursor position, buttons pressed and bmps
 */
typedef struct
{
    int x, y; //Current pointing positon
    bool lb;
    bool rb;
    Bitmap *pressed;
    Bitmap *released;
} Cursor;

/**
 * @brief Struct that stores timer information and indicates which spells are bieng casted
 */
typedef struct
{

    bool Fire_Cast;
    bool Water_Cast;
    bool Earth_Cast;
    bool Air_Cast;
    bool Leap_Cast; 

    unsigned fire_timer;
    unsigned water_timer;
    unsigned earth_timer;
    unsigned air_timer;
    unsigned leap_timer; 

}SpellCast;

//main layout
/**
 * @brief Allocates memory for all Assets used in our project (Bmps, Sprites and animations)
 * @return returns whether the operation was successful or not
 */
bool LoadAssets(); //Loads all of the bitmaps and saves them in sprites, bmps or animations
void DrawToolBox();

//animation
/**
 * @param animation_name Name of animation folder
 * @param n_frames Number of frames
 * @param ticks_per_frame Number of ticks in between frames
 * @param rotates Indicates whether the animation rotatates or not
 * @return Returns a pointer to the Animation struct
 * @brief Open the files inside the folder indicated and creates an Animation struct with the proper values (Bmps inside folder should have the same name as the folder followed by the frame number)
 */
Animation* CreateAnimation(char animation_name[], int n_frames, int ticks_per_frame, bool rotates); //Used to load animations
/**
 * @brief Depending on the wizard spell and cast_type it+s animation parameter will be updated
 * @param wizard Pointer to a Wizard object
 */
void Get_Animation(Wizard* wizard); //Get correct casting animation for a wizard
/**
 * @brief Draws the correct frame at the correct location on de Video buffer
 * @param animation pointer to animation to draw
 * @param center_x x coordinate of the center of animation
 * @param center_y y coordinate of the center of the animation
 * @param frame_n number of the frame to draw
 * @param try_n used to calculate how many ticks until the next frame can be drawn
 * @param rot used to draw the animation in it's correct rotation
 */
void Draw_Animation(Animation* animation, int center_x, int center_y, int frame_n, int *try_n, unsigned int rot);

//players
/**
 * @brief This function Checks if the mouse input is correct in relation to the type of spell being cast (Also checks for new spells being casted by the player)
 * @param player pointer to player's wizard
 * @param cursor pointer to cursor object
 */
void Player_Cast(Wizard* player, Cursor* cursor); //Handles spell casting for players
/**
 * @brief Depending on which buttons are being pressed a different spell type will be return
 * @param cursor pointer to cursor
 * @return return the Spell_Type (used mostly for casting)
 */
enum Spell_Type Get_Spell_Type(Cursor* cursor); //Gets what type of spell to cast depending on the cursor attributes
/**
 * @brief Checks if the Cursor attributes are correct to cast the specified spell_type
 * @param cursor pointer to cursor
 * @param spell_type type of spell bieng casted
 * @return return whether the casting can continue or not
 */
bool Check_Cursor(Cursor* cursor, enum Spell_Type spell_type);

/**
 * @brief This function Creates a wizard object
 * @param color color of the wizard
 * @param center_x center position
 * @param center_y center position
 * @param rot rotation of the wiard
 * @param name wizard's name
 * @return returns a pointer to the created object
 */
Wizard* CreateWizard(enum Wizard_color color, int center_x, int center_y, unsigned int rot, char* name);
/**
 * @brief Draws the wizard with the correct parameters to the buffer
 * @param wizard pointer to wizard being drawn
 */
void DrawWizard(Wizard *wizard);
/**
 * @brief Checks if there is any collision between elements and wizards
 * @param wizard pointer to wizard
 * @param element pointer to element
 */
void Wizard_Colision(Wizard *wizard, Element *element);
/**
 * @brief Draws the casting animation of the wizard with th correct parameters
 * @param wizard pointer to wizard
 */
void Draw_Cast(Wizard* wizard);
/**
 * @brief Executes the Leap spell for the specified wizard
 * @param wizard poitner to wizard
 */
void Wizard_Leap(Wizard* wizard);

//bots
/**
 * @brief Creates a Bot object with the specified argument
 * @param color color of Bot's WIzard
 * @param center_x x position of Bot's Wizard
 * @param center_y y position of Bot's wizard
 * @param name Bot's name
 * @return Returns pointer to Bot Created
 */
Bot* CreateBot(enum Wizard_color color, int center_x, int center_y, char* name);
/**
 * @brief This fuction changed the tharget the bot is lookng at to another wizard
 * @param bot pointer to bot who's target will change
 */
void Change_Target(Bot *bot);
/**
 * @brief Casts a new spell for the specified bot
 * @param bot pointer to bot
 */
void Bot_Cast(Bot *bot);
/**
 * @brief Checks if it's time to change the target or cast a spell or wobble
 * @param bot
 */
void Update_Bot_Rotation(Bot *bot);
/**
 * @brief This function controls the speed at which the Bot will transition from one target to another
 * @param bot
 */
void Bot_Transition(Bot *bot);
/**
 * @brief This function will make the Bot wobble when it's not transitioning to give human like feel to it
 * @param bot
 */
void Bot_Wobble(Bot *bot);

//elements
/**
 * @brief Creates a new element depending on the type of spell the wizard is casting
 * @param wizard
 * @return returns pointer to element created
 */
Element *CreateElement(Wizard* wizard);
/**
 * @brief Draws element on correct position and with correct rotation
 * @param element pointer to element to draw
 */
void DrawElement(Element *element);
/**
 * @brief Moves the element to a new correct location depending on his rotation
 * @param element
 */
void Move_Element(Element *element);
/**
 * @brief Checks for what to do when there is a collision between elements (Depedning on the elements Element_Type)
 * @param element1
 * @param element2
 */
void Element_Colision(Element *element1, Element *element2);
/**
 * @brief Draws the correct frame of the destrution of an element
 * @param element
 */
void Draw_Destruction(Element* element);
/**
 * @brief Checks if the Element is out of bounds (out of screen)
 * @param element
 * @return Returns whether or nit it is out of bounds
 */
bool Out_Of_Bounds(Element* element);    

//cursors
/**
 * @brief Creates a cursor object on the specified locations
 * @param x x coordinate
 * @param y y coordinate
 * @return Returns pointer to cursor object
 */
Cursor *CreateCursor(int x, int y); //Creates Cursor obj
/**
 * @brief Draws the Cursor on correct location (and with correct BMP depending if it's pressed or not)
 * @param cursor
 */
void DrawCursor(Cursor *cursor);    //Tests for out of bounds and draws appropriate Bitmap on screen

//keayboard layout
/**
 * @brief Draws the Text box to write spells
 */
void DrawTextBox();
void DrawTextPointer();

//timers
/**
 * @brief Draws the correct timer bmp depending of how much time has passed since last Fire spell castes
 */
void DrawFireTimer();
/**
 * @brief Draws the correct timer bmp depending of how much time has passed since last Water spell castes
 */
void DrawWaterTimer();
/**
 * @brief Draws the correct timer bmp depending of how much time has passed since last earth spell castes
 */
void DrawEarthTimer();
/**
 * @brief Draws the correct timer bmp depending of how much time has passed since last wind spell castes
 */
void DrawWindTimer();
/**
 * @brief Draws the correct timer bmp depending of how much time has passed since last Leap spell castes
 */
void DrawLeapTimer();
/**
 * @brief Draws the timers
 */
void DrawTimers();

//video
/**
 * @brief This function runs 60 times per second and is responsible for managing the Drawing on the buffer, updating bot behavioour, sending Multiplayer information and collision checking
 */
void Update_Game_State();

//Multiplayer
/**
 * @brief Sends the Game info from Host to Guest in Multiplayer
 */
void Send_Game_Info();
/**
 * @brief Creates a new element in the Guest computer when it is casted in Host
 * @param array_pos positiion in element array
 * @param elem_type type of element
 * @param spell_type type of spell
 * @return pointer to element created
 */
Element* Create_Guest_Element(int array_pos, int8_t elem_type, uint8_t spell_type);

/**
 * @brief destroys all the bitmaps when the game closes
 */
void Destroy_Bitmaps();

#endif
