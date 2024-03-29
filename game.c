#include <lcom/lcf.h>

#include "game.h"
#include "video_card.h"
#include "timer.h"
#include "keyboard.h"
#include <math.h>
#include <stdio.h>
#include "RTC.h"
#include "Menu.h"
#include "serial.h"

extern bool MP;
extern bool Host;
extern char* username_2;

Bitmap* bitmaps_to_destroy[TOTAL_BITMAPS];

//layout
Bitmap *background;
Bitmap *Tool_Box;
Bitmap *Loading_Screen;
Bitmap *Main_Page;
Bitmap *Pause_Menu;
Bitmap *Name_Box;
Bitmap *Info_Box;
Bitmap *Victory_Menu;
Bitmap *Defeat_Menu;

//mouse manip
Bitmap *P_Cursor;
Bitmap *R_Cursor;
Sprite *GreenWizard;
Sprite *BlueWizard;
Sprite *RedWizard;
Sprite *YellowWizard;

//load font
Bitmap *Letter_A;
Bitmap *Letter_B;
Bitmap *Letter_C;
Bitmap *Letter_D;
Bitmap *Letter_E;
Bitmap *Letter_F;
Bitmap *Letter_G;
Bitmap *Letter_H;
Bitmap *Letter_I;
Bitmap *Letter_J;
Bitmap *Letter_K;
Bitmap *Letter_L;
Bitmap *Letter_M;
Bitmap *Letter_N;
Bitmap *Letter_O;
Bitmap *Letter_P;
Bitmap *Letter_Q;
Bitmap *Letter_R;
Bitmap *Letter_S;
Bitmap *Letter_T;
Bitmap *Letter_U;
Bitmap *Letter_V;
Bitmap *Letter_W;
Bitmap *Letter_X;
Bitmap *Letter_Y;
Bitmap *Letter_Z;

Bitmap *Number_0;
Bitmap *Number_1;
Bitmap *Number_2;
Bitmap *Number_3;
Bitmap *Number_4;
Bitmap *Number_5;
Bitmap *Number_6;
Bitmap *Number_7;
Bitmap *Number_8;
Bitmap *Number_9;
Bitmap *Double_Dots;

Bitmap *TextBox;
Bitmap *TextPointer;

//timers

Bitmap *Fire_0;
Bitmap *Fire_1;
Bitmap *Fire_2;
Bitmap *Fire_3;
Bitmap *Fire_4;
Bitmap *Fire_5;

Bitmap *Water_0;
Bitmap *Water_1;
Bitmap *Water_2;
Bitmap *Water_3;
Bitmap *Water_4;
Bitmap *Water_5;

Bitmap *Wind_0;
Bitmap *Wind_1;
Bitmap *Wind_2;
Bitmap *Wind_3;
Bitmap *Wind_4;
Bitmap *Wind_5;

Bitmap *Earth_0;
Bitmap *Earth_1;
Bitmap *Earth_2;
Bitmap *Earth_3;
Bitmap *Earth_4;
Bitmap *Earth_5;

Bitmap *Leap_0;
Bitmap *Leap_1;
Bitmap *Leap_2;
Bitmap *Leap_3;

//elements
Sprite *FireBall;
Sprite *WaterBall;
Sprite *EarthBall;
Sprite *AirBall;

//animations
Animation *Explosion;
Animation *Fire_Cast;
Animation *Wind_Cast;
Animation *Earth_Cast;
Animation *Water_Cast;

extern Cursor *cursor;
extern SpellCast SpellsRdy;

//menu
extern GameUtils GameMenus;
extern enum player_name name_status_single;
extern enum player_name name_status_multi;
enum game_result gameStatus;
extern bool Gamerules;

//List of all Sprites

bool LoadAssets()
{
    if ((Pause_Menu = loadBitmap("Pause_Menu.bmp")) == NULL)
        return false;
    if ((Main_Page = loadBitmap("Main_Page.bmp")) == NULL)
        return false;
    if ((Name_Box = loadBitmap("Name_Box.bmp")) == NULL)
        return false;
    if ((Info_Box = loadBitmap("Info_Box.bmp")) == NULL)
        return false;
    if ((Victory_Menu = loadBitmap("Victory_Menu.bmp")) == NULL)
        return false;
    if ((Defeat_Menu = loadBitmap("Defeat_Menu.bmp")) == NULL)
        return false;

    if ((background = loadBitmap("Background.bmp")) == NULL)
        return false;
    if ((Tool_Box = loadBitmap("Tool_Box.bmp")) == NULL)
        return false;
    if ((GreenWizard = CreateSprite("Green_Hat.bmp")) == NULL)
        return false;
    if ((BlueWizard = CreateSprite("Blue_Hat.bmp")) == NULL)
        return false;
    if ((YellowWizard = CreateSprite("Yellow_Hat.bmp")) == NULL)
        return false;
    if ((RedWizard = CreateSprite("Red_Hat.bmp")) == NULL)
        return false;
    if ((Explosion = CreateAnimation("Explosion", 9, 2, true)) == NULL)
        return false;
    if ((Fire_Cast = CreateAnimation("Fire_Cast", 8, 2, false)) == NULL)
        return false;
    if ((Wind_Cast = CreateAnimation("Wind_Cast", 8, 2, false)) == NULL)
        return false;
    if ((Earth_Cast = CreateAnimation("Earth_Cast", 8, 2, false)) == NULL)
        return false;
    if ((Water_Cast = CreateAnimation("Water_Cast", 8, 2, false)) == NULL)
        return false;

    //mouse
    if ((P_Cursor = loadBitmap("Cursor_Pressed.bmp")) == NULL)
        return false;
    if ((R_Cursor = loadBitmap("Cursor_Released.bmp")) == NULL)
        return false;

    //load font
    if ((Letter_A = loadBitmap("A.bmp")) == NULL)
        return false;
    if ((Letter_B = loadBitmap("B.bmp")) == NULL)
        return false;
    if ((Letter_C = loadBitmap("C.bmp")) == NULL)
        return false;
    if ((Letter_D = loadBitmap("D.bmp")) == NULL)
        return false;
    if ((Letter_E = loadBitmap("E.bmp")) == NULL)
        return false;
    if ((Letter_F = loadBitmap("F.bmp")) == NULL)
        return false;
    if ((Letter_G = loadBitmap("G.bmp")) == NULL)
        return false;
    if ((Letter_H = loadBitmap("H.bmp")) == NULL)
        return false;
    if ((Letter_I = loadBitmap("I.bmp")) == NULL)
        return false;
    if ((Letter_J = loadBitmap("J.bmp")) == NULL)
        return false;
    if ((Letter_K = loadBitmap("K.bmp")) == NULL)
        return false;
    if ((Letter_L = loadBitmap("L.bmp")) == NULL)
        return false;
    if ((Letter_M = loadBitmap("M.bmp")) == NULL)
        return false;
    if ((Letter_N = loadBitmap("N.bmp")) == NULL)
        return false;
    if ((Letter_O = loadBitmap("O.bmp")) == NULL)
        return false;
    if ((Letter_P = loadBitmap("P.bmp")) == NULL)
        return false;
    if ((Letter_Q = loadBitmap("Q.bmp")) == NULL)
        return false;
    if ((Letter_R = loadBitmap("R.bmp")) == NULL)
        return false;
    if ((Letter_S = loadBitmap("S.bmp")) == NULL)
        return false;
    if ((Letter_T = loadBitmap("T.bmp")) == NULL)
        return false;
    if ((Letter_U = loadBitmap("U.bmp")) == NULL)
        return false;
    if ((Letter_V = loadBitmap("V.bmp")) == NULL)
        return false;
    if ((Letter_W = loadBitmap("W.bmp")) == NULL)
        return false;
    if ((Letter_X = loadBitmap("X.bmp")) == NULL)
        return false;
    if ((Letter_Y = loadBitmap("Y.bmp")) == NULL)
        return false;
    if ((Letter_Z = loadBitmap("Z.bmp")) == NULL)
        return false;
    if ((Number_0 = loadBitmap("0.bmp")) == NULL)
        return false;
    if ((Number_1 = loadBitmap("1.bmp")) == NULL)
        return false;
    if ((Number_2 = loadBitmap("2.bmp")) == NULL)
        return false;
    if ((Number_3 = loadBitmap("3.bmp")) == NULL)
        return false;
    if ((Number_4 = loadBitmap("4.bmp")) == NULL)
        return false;
    if ((Number_5 = loadBitmap("5.bmp")) == NULL)
        return false;
    if ((Number_6 = loadBitmap("6.bmp")) == NULL)
        return false;
    if ((Number_7 = loadBitmap("7.bmp")) == NULL)
        return false;
    if ((Number_8 = loadBitmap("8.bmp")) == NULL)
        return false;
    if ((Number_9 = loadBitmap("9.bmp")) == NULL)
        return false;
    if ((Double_Dots = loadBitmap("Double_Dots.bmp")) == NULL)
        return false;

    if ((TextBox = loadBitmap("Text_Box.bmp")) == NULL)
        return false;
    if ((TextPointer = loadBitmap("Text_Pointer.bmp")) == NULL)
        return false;

    //load timer displays
    if ((Fire_0 = loadBitmap("Fire_0.bmp")) == NULL)
        return false;
    if ((Fire_1 = loadBitmap("Fire_1.bmp")) == NULL)
        return false;
    if ((Fire_2 = loadBitmap("Fire_2.bmp")) == NULL)
        return false;
    if ((Fire_3 = loadBitmap("Fire_3.bmp")) == NULL)
        return false;
    if ((Fire_4 = loadBitmap("Fire_4.bmp")) == NULL)
        return false;
    if ((Fire_5 = loadBitmap("Fire_5.bmp")) == NULL)
        return false;

    if ((Water_0 = loadBitmap("Water_0.bmp")) == NULL)
        return false;
    if ((Water_1 = loadBitmap("Water_1.bmp")) == NULL)
        return false;
    if ((Water_2 = loadBitmap("Water_2.bmp")) == NULL)
        return false;
    if ((Water_3 = loadBitmap("Water_3.bmp")) == NULL)
        return false;
    if ((Water_4 = loadBitmap("Water_4.bmp")) == NULL)
        return false;
    if ((Water_5 = loadBitmap("Water_5.bmp")) == NULL)
        return false;

    if ((Wind_0 = loadBitmap("Wind_0.bmp")) == NULL)
        return false;
    if ((Wind_1 = loadBitmap("Wind_1.bmp")) == NULL)
        return false;
    if ((Wind_2 = loadBitmap("Wind_2.bmp")) == NULL)
        return false;
    if ((Wind_3 = loadBitmap("Wind_3.bmp")) == NULL)
        return false;
    if ((Wind_4 = loadBitmap("Wind_4.bmp")) == NULL)
        return false;
    if ((Wind_5 = loadBitmap("Wind_5.bmp")) == NULL)
        return false;

    if ((Earth_0 = loadBitmap("Earth_0.bmp")) == NULL)
        return false;
    if ((Earth_1 = loadBitmap("Earth_1.bmp")) == NULL)
        return false;
    if ((Earth_2 = loadBitmap("Earth_2.bmp")) == NULL)
        return false;
    if ((Earth_3 = loadBitmap("Earth_3.bmp")) == NULL)
        return false;
    if ((Earth_4 = loadBitmap("Earth_4.bmp")) == NULL)
        return false;
    if ((Earth_5 = loadBitmap("Earth_5.bmp")) == NULL)
        return false;

    if ((Leap_0 = loadBitmap("Leap_0.bmp")) == NULL)
        return false;
    if ((Leap_1 = loadBitmap("Leap_1.bmp")) == NULL)
        return false;
    if ((Leap_2 = loadBitmap("Leap_2.bmp")) == NULL)
        return false;
    if ((Leap_3 = loadBitmap("Leap_3.bmp")) == NULL)
        return false;

    //spell sprites
    if ((FireBall = CreateSprite("FireBall.bmp")) == NULL)
        return false;
    if ((WaterBall = CreateSprite("WaterBall.bmp")) == NULL)
        return false;
    if ((EarthBall = CreateSprite("EarthBall.bmp")) == NULL)
        return false;
    if ((AirBall = CreateSprite("AirBall.bmp")) == NULL)
        return false;

    return true;
}

void Destroy_Bitmaps()
{
    unsigned int total = 0;
    for(unsigned int i = 0; i < TOTAL_BITMAPS; i++)
    {
        if(bitmaps_to_destroy[i] == NULL) break;
        free(bitmaps_to_destroy[i]);
        total++;
    }
    printf("\n DESTROYED %d BITMAPS", total);
}

Animation *CreateAnimation(char animation_name[], int n_frames, int ticks_per_frame, bool rotates)
{
    Animation *animation = (Animation *)malloc(sizeof(Animation));
    char name[100] = "/";
    strcat(name, animation_name);

    char path[100] = "\0";
    strcpy(path, animation_name);
    strcat(path, name);

    for (int i = 0; i < n_frames; i++)
    {
        char final[100] = "\0";
        strcpy(final, path);

        char number[2] = "\0";
        sprintf(number, "%d", i);
        strcat(final, number);

        strcat(final, ".bmp");

        if(rotates)
        {
            Sprite *anim = CreateSprite(final);
            if (anim != NULL)
                animation->sprites[i] = anim;
            else
            {
                printf("\nInvalid animation path: %s", final);
                return NULL;
            }
        }
        else
        {
            Bitmap *anim = loadBitmap(final);
            if (anim != NULL)
                animation->bitmaps[i] = anim;
            else
            {
                printf("\nInvalid animation path: %s", final);
                return NULL;
            }
        }
    }

    animation->n_frames = n_frames;
    animation->ticks_between_frames = ticks_per_frame;
    animation->rotates = rotates;

    return animation;
}

//////////OBJECT ARRAYS///////////
Wizard *wizards[WIZARDS_SIZE]; //4 is max nmuber of wizards
Bot *bots[BOTS_SIZE];
Element *elements[ELEMS_SIZE]; //there'll never be more than 30 active spells at the same time so this number is fine

Wizard *CreateWizard(enum Wizard_color color, int center_x, int center_y, unsigned int rot, char *name)
{
    Wizard *wizard = (Wizard *)malloc(sizeof(Wizard));
    wizard->color = color;
    switch (color)
    {
    case Red:
        wizard->img = RedWizard;
        break;
    case Green:
        wizard->img = GreenWizard;
        break;
    case Blue:
        wizard->img = BlueWizard;
        break;
    case Yellow:
        wizard->img = YellowWizard;
        break;
    }

    wizard->center_x = center_x;
    wizard->center_y = center_y;
    wizard->rot = rot;
    wizard->casting = false;
    wizard->cast_type = Null;
    wizard->health = 3;
    wizard->name = name;

    for (unsigned int i = 0; i < WIZARDS_SIZE; i++)
    { //There can only be 4 wizards
        if (wizards[i] == NULL)
        {
            wizards[i] = wizard;
            break;
        }
    }

    return wizard;
}

void Wizard_Colision(Wizard *wizard, Element *element)
{
    if (element->elem_type != Null && element->active)
    {
        element->active = false;
        wizard->health--;
        //printf("\n %s :  %d HEALTH", wizard->name, wizard->health); //debug
        if (MP)
            Send_Element(element, element->array_position);
    }
}

Element *CreateElement(Wizard *wizard)
{
    Element *elem = (Element *)malloc(sizeof(Element));
    elem->elem_type = wizard->cast_type;
    elem->spell_type = wizard->spell;
    switch (elem->elem_type)
    {
    case Air:
        elem->img = AirBall;
        break;
    case Earth:
        elem->img = EarthBall;
        break;
    case Water:
        elem->img = WaterBall;
        break;
    case Fire:
        elem->img = FireBall;
        break;
    case Null:
        return NULL;
    }

    int x = wizard->center_x;
    int y = wizard->center_y;
    double rot = wizard->rot * M_PI / 180.0;
    x -= CAST_DISTANCE * sin(rot);
    y -= CAST_DISTANCE * cos(rot);

    elem->center_x = x;
    elem->center_y = y;
    elem->rot = wizard->rot;
    elem->active = true;
    elem->frame_n = 0;
    elem->destroyed = false;
    elem->try_n = 0;

    for (unsigned int i = 0; i < ELEMS_SIZE; i++)
    {
        if (elements[i] == NULL)
        {
            elements[i] = elem;
            elem->array_position = i;
            if (MP)
                Send_Element(elem, i);
            break;
        }
    }

    return elem;
}

Cursor *CreateCursor(int x, int y)
{
    Cursor *cursor = (Cursor *)malloc(sizeof(Cursor));

    cursor->x = x;
    cursor->y = y;

    cursor->lb = false;
    cursor->rb = false;
    cursor->pressed = P_Cursor;
    cursor->released = R_Cursor;

    return cursor;
}

void DrawToolBox()
{
    DrawBitmap(Tool_Box, 0, 618);
}

void DrawCursor(Cursor *cursor)
{
    if (cursor->x < 0)
        cursor->x = 0;
    else if (cursor->x > H_RES)
        cursor->x = H_RES;

    if (cursor->y < 0)
        cursor->y = 0;
    else if (cursor->y > V_RES)
        cursor->y = V_RES;

    if (cursor->lb || cursor->rb)
        DrawBitmap(cursor->pressed, cursor->x, cursor->y);
    else
        DrawBitmap(cursor->released, cursor->x, cursor->y);

    //BUTTON POS
    // printf( "mouse x:%d", cursor->x);
    //printf( "mouse y:%d\n", cursor->y);
}

void DrawTextBox()
{
    DrawBitmap(TextBox, 25, 640);
}
void DrawTextPointer()
{
    DrawBitmap(TextPointer, 40, 680);
}

void DrawWizard(Wizard *wizard)
{
    DrawSprite(wizard->img, wizard->center_x, wizard->center_y, wizard->rot, true);
}

void DrawElement(Element *element)
{
    DrawSprite(element->img, element->center_x, element->center_y, element->rot, true);
}

void Move_Element(Element *element)
{
    double rot;
    rot = (element->rot * M_PI / 180.0);

    int x_des = round(FAST_SPEED * sin(rot));
    int y_des = round(FAST_SPEED * cos(rot));

    element->center_x -= x_des;
    element->center_y -= y_des;
}

void Element_Colision(Element *element1, Element *element2)
{
    int e1 = (int)element1->elem_type;
    int e2 = (int)element2->elem_type;
    if (e1 == e2)
        return; //Do nothing for elements of same type
    else if (e1 * e2 > 0)
    { //Destroy both for elements of opposing types
        element1->active = false;
        element2->active = false;
        if (MP)
            Send_Element(element1, element1->array_position);
        if (MP)
            Send_Element(element2, element2->array_position);
    }
    else
    { //If they're consecutive elements decie which to destroy
        int res = e1 + e2;
        if (res == -3)
        { //If colision happens between earth and air (Special case)
            if (element1->elem_type == Air)
            {
                element1->active = false;
                if (MP)
                    Send_Element(element1, element1->array_position);
            }
            else
            {
                element2->active = false;
                if (MP)
                    Send_Element(element2, element2->array_position);
            }
            return;
        }
        if (abs(e1) > abs(e2))
        {
            element1->active = false; //Highest element gets destroyed
            if (MP)
                Send_Element(element1, element1->array_position);
        }
        else
        {
            element2->active = false;
            if (MP)
                Send_Element(element2, element2->array_position);
        }
    }
}

Bot *CreateBot(enum Wizard_color color, int center_x, int center_y, char *name)
{
    Bot *bot = (Bot *)malloc(sizeof(Bot));

    bot->attention_span = rand() % HIGHER_ATTENTION + LOWER_ATTENTION;
    bot->time_to_fire = rand() % HIGHER_TTF + LOWER_TTF;

    //First they'll be pointing to center
    int x = 512;
    int y = 384;
    int angle = atan2(center_y - y, x - center_x) * 180 / M_PI - 90;
    if (angle < 0)
        angle = 360 + angle;
    bot->target = angle;
    bot->transitioning = true;
    bot->init_diff = 0;

    bot->wizard = CreateWizard(color, center_x, center_y, angle, name);

    for (int i = 0; i < BOTS_SIZE; i++)
    {
        if (bots[i] == NULL)
        {
            bots[i] = bot;
            break;
        }
    }

    return bot;
}

void Change_Target(Bot *bot)
{
    int target_wizard;
    Wizard *wiz;
    unsigned int fail = 0;

    do
    { //Gets a valid wizard to look at
        target_wizard = rand() % WIZARDS_SIZE;
        wiz = wizards[target_wizard];
        fail++;
        if (fail == 20)
            return; //So there isn't an endless loop
    } while (wiz == NULL || wiz->health <= 0 || wiz == bot->wizard);

    int angle = atan2(bot->wizard->center_y - wiz->center_y, wiz->center_x - bot->wizard->center_x) * 180 / M_PI - 90;
    if (angle < 0)
        angle = 360 + angle;

    if (angle != bot->target)
    {
        bot->target = angle;
        bot->transitioning = true;
    }
    bot->attention_span = rand() % HIGHER_ATTENTION + LOWER_ATTENTION;

    //printf("\n TARGET_WIZ: %d  RAND: %d", rand() % WIZARDS_SIZE, rand());
    //printf("\n %s Targeting %s", bot->wizard->name, wiz->name);
    //printf("\n TARGET: %d", bot->target);

    //sleep(3);
}

int Transition_Function(int diff_percentage)
{ //This values give a very simple semi-human like mouse movement
    if (diff_percentage <= 20)
        return (pow(diff_percentage, 2) / 100) + 5 + (rand() % 2);
    else if (diff_percentage < 80)
        return -pow(40 - diff_percentage, 2) / 530 + 10 + (rand() % 5);
    else
        return -pow(80 - diff_percentage, 2) / 80 + 8 + (rand() % 3);
}

void Bot_Transition(Bot *bot)
{
    //Trasition movement algorithm

    //Find which direction it's easiset to rotate (Increment or decrement rot)
    int distance = abs(bot->wizard->rot - bot->target);
    if (distance > 180)
    {
        distance = 360 - distance;
    }

    bool sign; //false if minus, true if plus
    int direction = bot->wizard->rot + distance;
    if (direction > 360)
        direction = 360 - direction;

    if (direction == bot->target)
        sign = true;
    else
        sign = false;

    if (distance < 3)
    { //If transition in complete
        bot->transitioning = false;
        bot->wizard->rot = bot->target;
        bot->var_target = bot->wizard->rot;
        bot->init_diff = 0;
        //printf("\nEND Transition");
        //sleep(3);
        return;
    }

    if (bot->init_diff == 0)
        bot->init_diff = distance;

    int diff_percentage = 100 - (distance / bot->init_diff) * 100;
    int val = Transition_Function(diff_percentage); //Get value to increment/decrement

    int final_rotation;
    if (sign)
        final_rotation = bot->wizard->rot + val;
    else
        final_rotation = bot->wizard->rot - val;

    if (final_rotation > 360)
        final_rotation -= 360;
    else if (final_rotation < 0)
        final_rotation += 360;

    //printf("\nBOT: %s FINAL ROTATION: %d",bot->wizard->name, final_rotation);
    //printf("\nDIFF: %d",distance);
    //printf("\nDIRECTION: %d   ROT: %d   TARGET: %d",direction,bot->wizard->rot,bot->target);
    //printf("\nVAL: %d   DIFF_PERCENT: %d",val, diff_percentage);
    bot->wizard->rot = final_rotation;
}

void Bot_Wobble(Bot *bot)
{
    //Wobble Algorithm

    if (bot->var_target == bot->wizard->rot)
    {                                            //if we're there already let's get a new target
        int var = rand() % MAX_WOOBLE_VAR - 135; //so it goes from -10 to 10 and every value below or above that is 0
        if (var > 10 || var < -10)
            var = 0; //So zero is more common

        if (var == 0)
            return; //if it's zero then there's no need to do anything
        int new_target = bot->target + var;

        if (new_target > 360)
            new_target -= 360;
        else if (new_target < 0)
            new_target += 360;

        bot->var_target = new_target;
    }

    int distance = abs(bot->wizard->rot - bot->var_target);
    if (distance > 180)
    {
        distance = 360 - distance;
    }
    unsigned int direction = bot->wizard->rot + distance;
    if (direction > 360)
        direction = 360 - direction;
    bool sign;
    if (direction == bot->var_target)
        sign = true;
    else
        sign = false;

    int val = rand() % 2;
    if (val > 2)
        val = 0; //So 0 has more chance of appearing

    int final_rotation;
    if (sign)
        final_rotation = bot->wizard->rot + val;
    else
        final_rotation = bot->wizard->rot - val;

    if (final_rotation > 360)
        final_rotation -= 360;
    else if (final_rotation < 0)
        final_rotation += 360;

    if (abs(final_rotation - bot->var_target) < 2)
        bot->wizard->rot = bot->var_target;
    else
        bot->wizard->rot = final_rotation;
}

void Update_Bot_Rotation(Bot *bot)
{
    if (bot->transitioning)
    {
        Bot_Transition(bot);
    }
    else
    {
        Bot_Wobble(bot);
    }
}

void Bot_Cast(Bot *bot)
{
    enum Element_Type type;
    do
    {
        int type_n = rand() % 3;
        switch (type_n)
        {
        case 0:
            type = Air;
            break;

        case 1:
            type = Fire;
            break;

        case 2:
            type = Earth;
            break;

        case 3:
            type = Water;
            break;
        }
    } while (type == bot->last_used);

    int type_n = rand() % 3;
    enum Spell_Type spell_type;
    switch (type_n)
    {
    case 0:
        spell_type = Launch;
        break;

    case 1:
        spell_type = Launch;
        break;

    case 2:
        spell_type = Launch;
        break;

    case 3:
        spell_type = Launch;
        break;
    }

    bot->last_used = type;
    bot->wizard->casting = true;
    bot->wizard->cast_type = type;
    bot->wizard->spell = spell_type;
    Get_Animation(bot->wizard);

    bot->time_to_fire = rand() % HIGHER_TTF + LOWER_TTF;
}

///////////////////////////////////////////////
extern SpellCast SpellsRdy;
extern Cursor *cursor;
extern Wizard *player;

enum Spell_Type Get_Spell_Type(Cursor *cursor)
{
    if (cursor->lb && !cursor->rb)
        return Launch;
    else
        return None;
}

bool Check_Cursor(Cursor *cursor, enum Spell_Type spell_type)
{
    switch (spell_type)
    {
    case Launch:
        return cursor->lb;

    case None:
        return false;
    case Leap:
        return false;
    }

    return false;
}

void Draw_Animation(Animation *animation, int center_x, int center_y, int frame_n, int *try_n, unsigned int rot)
{
    if (frame_n >= animation->n_frames)
        return;
    if (*try_n < animation->ticks_between_frames && frame_n != 0)
    {
        (*try_n)++;
    }
    else
    {
        (*try_n) = 0;
    }
    if(animation->rotates) DrawSprite(animation->sprites[frame_n], center_x, center_y, rot, true);
    else{
        int x = center_x - animation->bitmaps[frame_n]->bitmapInfoHeader.width/2;
        int y = center_y - animation->bitmaps[frame_n]->bitmapInfoHeader.height/2;
        DrawBitmap(animation->bitmaps[frame_n],x,y);
    }
}

void Draw_Cast(Wizard *wizard)
{
    if (wizard->cast_animation == NULL || wizard->frame_n > wizard->cast_animation->n_frames)
        return;

    int x = wizard->center_x;
    int y = wizard->center_y;
    double rot = wizard->rot * M_PI / 180.0;
    x -= CAST_DISTANCE * sin(rot);
    y -= CAST_DISTANCE * cos(rot);
    if (wizard->frame_n == wizard->cast_animation->n_frames)
    { //So it stops on last frame
        wizard->frame_n--;
        wizard->try_n = wizard->cast_animation->ticks_between_frames;
    }

    Draw_Animation(wizard->cast_animation, x, y, wizard->frame_n, &wizard->try_n, wizard->rot);

    if (wizard->try_n == 0)
        wizard->frame_n++;
}

void Get_Animation(Wizard *wizard)
{
    switch (wizard->spell)
    {
    case Leap:
    case None:
        wizard->cast_animation = NULL;
        break;

    case Launch:
        if (wizard->cast_type == Fire)
            wizard->cast_animation = Fire_Cast;
        else if (wizard->cast_type == Water)
            wizard->cast_animation = Water_Cast;
        else if (wizard->cast_type == Air)
            wizard->cast_animation = Wind_Cast;
        else if (wizard->cast_type == Earth)
            wizard->cast_animation = Earth_Cast;
        else
            wizard->cast_animation = NULL;
        break;
    }

    wizard->frame_n = 0;
}

void Wizard_Leap(Wizard* wizard){
    int x_dist = player->leap_xf - player->center_x;
    int y_dist = player->leap_yf - player->center_y;
    int dist = round(sqrt(pow(x_dist,2) + pow(y_dist,2)));
    //printf("\n DIST: %d", dist); //debug
    if(dist <= 4)
    {
        wizard->center_x = wizard->leap_xf;
        wizard->center_y = wizard->leap_yf;
        wizard->spell = None;
        wizard->casting = false;
        return;
    }
    int x_des = round(LEAP_SPEED*x_dist/dist);
    int y_des = round(LEAP_SPEED*y_dist/dist);
    wizard->center_x += x_des;
    wizard->center_y += y_des;
}

unsigned int spell_timer = SPELL_TIMER; //30 ticks = 0.5 secs

void Player_Cast(Wizard *player, Cursor *cursor)
{
    enum Spell_Type type;
    if (player->casting)
    { //If there's already something being casted
        if(player->spell == Leap)
        {
            Wizard_Leap(player);
            return;
        }
        if (player->frame_n >= player->cast_animation->n_frames)
        {
            spell_timer--;
            if (!Check_Cursor(cursor, player->spell))
            {
                CreateElement(player);
                spell_timer = 0;
            }
            if (spell_timer == 0)
            {
                spell_timer = SPELL_TIMER;
                player->casting = false;
            }
        }

        if (!Check_Cursor(cursor, player->spell))
        {
            player->casting = false;
            player->spell = None;
            player->cast_type = Null;
        }
    }
    else
    { //If nothing is being casted check if there's anything ready for casting
        if (SpellsRdy.Fire_Cast)
        {
            if ((type = Get_Spell_Type(cursor)) != None)
            {
                player->casting = true;
                player->cast_type = Fire;
                player->spell = type;
                SpellsRdy.Fire_Cast = false;
                spell_timer = SPELL_TIMER;
                Get_Animation(player);
            }
            else
            {
                spell_timer--;
                if (spell_timer == 0)
                {
                    spell_timer = SPELL_TIMER;
                    SpellsRdy.Fire_Cast = false;
                }
            }
        }
        else if (SpellsRdy.Water_Cast)
        {
            if ((type = Get_Spell_Type(cursor)) != None)
            {
                player->casting = true;
                player->cast_type = Water;
                player->spell = type;
                SpellsRdy.Water_Cast = false;
                spell_timer = SPELL_TIMER;
                Get_Animation(player);
            }
            else
            {
                spell_timer--;
                if (spell_timer == 0)
                {
                    spell_timer = SPELL_TIMER;
                    SpellsRdy.Water_Cast = false;
                }
            }
        }
        else if (SpellsRdy.Air_Cast)
        {
            if ((type = Get_Spell_Type(cursor)) != None)
            {
                player->casting = true;
                player->cast_type = Air;
                player->spell = type;
                SpellsRdy.Air_Cast = false;
                spell_timer = SPELL_TIMER;
                Get_Animation(player);
            }
            else
            {
                spell_timer--;
                if (spell_timer == 0)
                {
                    spell_timer = SPELL_TIMER;
                    SpellsRdy.Air_Cast = false;
                }
            }
        }
        else if (SpellsRdy.Earth_Cast)
        {
            if ((type = Get_Spell_Type(cursor)) != None)
            {
                player->casting = true;
                player->cast_type = Earth;
                player->spell = type;
                SpellsRdy.Earth_Cast = false;
                spell_timer = SPELL_TIMER;
                Get_Animation(player);
            }
            else
            {
                spell_timer--;
                if (spell_timer == 0)
                {
                    spell_timer = SPELL_TIMER;
                    SpellsRdy.Earth_Cast = false;
                }
            }
        }
        else if (SpellsRdy.leap_timer == 3 && cursor->rb)
        {
            SpellsRdy.leap_timer = 0;
            player->spell = Leap;
            double rad_rot = player->rot * M_PI /180;
            int x_dis = round(sin(rad_rot)*LEAP_DISTANCE);
            int y_dis = round(cos(rad_rot)*LEAP_DISTANCE);

            player->leap_xf = player->center_x - x_dis;
            player->leap_yf = player->center_y - y_dis;

            if(player->leap_xf > H_RES - WIZARD_HITBOX_RADIUS) player->leap_xf = H_RES - WIZARD_HITBOX_RADIUS;
            else if(player->leap_xf < WIZARD_HITBOX_RADIUS) player->leap_xf = WIZARD_HITBOX_RADIUS;

            if(player->leap_yf > V_RES - WIZARD_HITBOX_RADIUS) player->leap_yf = V_RES - WIZARD_HITBOX_RADIUS;
            else if(player->leap_yf < WIZARD_HITBOX_RADIUS) player->leap_yf = WIZARD_HITBOX_RADIUS;
            player->casting = true;
            player->cast_animation = NULL;
        }
    }
}

void Draw_Destruction(Element *element)
{
    if (element->frame_n >= Explosion->n_frames)
    {
        element->destroyed = true;
        return;
    }
    Draw_Animation(Explosion, element->center_x, element->center_y, element->frame_n, &element->try_n, element->rot);
    if (element->try_n == 0)
        element->frame_n++;
}

bool Out_Of_Bounds(Element *element)
{
    int x = element->center_x + element->img->bitmap[element->rot]->bitmapInfoHeader.width / 2;
    int y = element->center_y + element->img->bitmap[element->rot]->bitmapInfoHeader.height / 2;

    if ((x < 0 || x > H_RES) && (y < 0 || y > V_RES))
    {
        element->destroyed = true;
        element->active = false;
        return true;
    }

    return false;
}

///////////////////////////////////////////////

bool openTextBox = false;
void Update_Game_State()
{

    //win condition
    if (gameStatus != clean)
        gameCondition();

    if (GameMenus.main_page == true)
    {
        DrawMainPage();
        DrawCursor(cursor);
        main_menu();

        if (Gamerules == true)
        {
            DrawInfoBox();
            DrawCursor(cursor);
        }

        if (name_status_single == get)
        {
            DrawBitmap(Name_Box, 210, 240);
            Draw_string(250, 300);
        }
        /*if (name_status_multi == get)
        {
            DrawBitmap(Name_Box, 210, 410);
            Draw_string(250, 470);
        }*/
    }

    if (GameMenus.run == 1)
    {
        if (GameMenus.pause == false)
        {
            if ((MP && Host) || !MP)
            {
                ////BOT UPDATES//// (Only Host or singleplayer)
                for (unsigned int i = 0; i < BOTS_SIZE; i++)
                { //Update Bot Behaviour
                    Bot *bot = bots[i];
                    if (bot != NULL && bot->wizard->health > 0)
                    { //If he exists and is alive
                        bot->attention_span--;
                        if (!bot->wizard->casting)
                            bot->time_to_fire--;
                        else if (bot->wizard->frame_n >= bot->wizard->cast_animation->n_frames)
                        {
                            CreateElement(bot->wizard);
                            bot->wizard->casting = false;
                            bot->wizard->frame_n = 0;
                        }

                        if (bot->attention_span == 0)
                        {
                            Change_Target(bot);
                        }

                        if (bot->time_to_fire == 0)
                        {
                            Bot_Cast(bot);
                        }

                        Update_Bot_Rotation(bot);
                    }
                }
                //////////////////////////
            }

            ////Updating Player rotation////
            int angle = round(atan2(player->center_y - cursor->y, cursor->x - player->center_x) * 180 / M_PI - 90);
            if (angle < 0)
                angle = 360 + angle;
            player->rot = angle;
            ////////////////////////////////

            Player_Cast(player, cursor); //Checks if player has cast anything and if he is casting correctly

            ////GUEST SENDS UPDATED INFORMATION
            if (MP && !Host)
            {
                //printf("\n SHOULD SEND GUEST WIZARD");
                Send_Wizard(player, 1);
            }

            if (!MP || (MP && Host))
            {
                ////MOVE ELEMENTS AND CHECK FOR COLISIONS//// (Only Host)
                for (unsigned int i = 0; i < ELEMS_SIZE; i++)
                {
                    if (elements[i] != NULL && elements[i]->active)
                    {
                        if (Out_Of_Bounds(elements[i]))
                            continue;

                        Move_Element(elements[i]);
                        for (unsigned int x = 0; x < ELEMS_SIZE; x++)
                        { //Colisions between elements
                            if (x != i && elements[x] != NULL && elements[x]->active)
                            {
                                int x_dis = abs(elements[i]->center_x - elements[x]->center_x);
                                int y_dis = abs(elements[i]->center_y - elements[x]->center_y);
                                int distance = sqrt(pow(x_dis, 2) + pow(y_dis, 2));
                                if (distance <= BALL_HITBOX_RADIUS * 2)
                                { //if there has been a colision
                                    Element_Colision(elements[i], elements[x]);
                                }
                            }
                        }

                        for (unsigned int x = 0; x < WIZARDS_SIZE; x++)
                        { //Colisions between elements and wizards
                            if (wizards[x] != NULL && wizards[x]->health > 0)
                            {
                                int x_dis = abs(elements[i]->center_x - wizards[x]->center_x);
                                int y_dis = abs(elements[i]->center_y - wizards[x]->center_y);
                                int distance = sqrt(pow(x_dis, 2) + pow(y_dis, 2));
                                if (distance <= BALL_HITBOX_RADIUS + WIZARD_HITBOX_RADIUS)
                                { //if there has been a colision
                                    Wizard_Colision(wizards[x], elements[i]);
                                }
                            }
                        }

                        for (unsigned int x = 0; x < WIZARDS_SIZE; x++)
                        { //Colisions between elements and wizards
                            if (wizards[x] != NULL && wizards[x]->health > 0)
                            {
                                int x_dis = abs(elements[i]->center_x - wizards[x]->center_x);
                                int y_dis = abs(elements[i]->center_y - wizards[x]->center_y);
                                int distance = sqrt(pow(x_dis, 2) + pow(y_dis, 2));
                                if (distance <= BALL_HITBOX_RADIUS + WIZARD_HITBOX_RADIUS)
                                { //if there has been a colision
                                    Wizard_Colision(wizards[x], elements[i]);
                                }
                            }
                        }
                    }
                }
                /////////////////////////////////////
            }

            ////HOST SENDS UPDATED INFORMATION
            if (MP && Host)
            {
                //printf("\n SHOULD SEND HOST INFO");
                Send_Game_Info();
            }

            ////GUEST MOVES ELEMENTS WITHOU CHECKING FOR COLISIONS
            if (MP && !Host)
            {
                for (unsigned int i = 0; i < ELEMS_SIZE; i++)
                {
                    if (elements[i] != NULL && elements[i]->active)
                    {
                        Move_Element(elements[i]);
                    }
                }
            }
            /////////////

            ////DRAWING TO BUFFER////
            DrawBackground();

            for (unsigned int i = 0; i < ELEMS_SIZE; i++)
            {
                if (elements[i] != NULL && elements[i]->active)
                { //Drawing active elements
                    DrawElement(elements[i]);
                }
                else if (elements[i] != NULL && elements[i]->destroyed)
                {
                    free(elements[i]);
                    elements[i] = NULL;
                }
                else if (elements[i] != NULL && !elements[i]->active)
                { //Drawing inactive elements
                    Draw_Destruction(elements[i]);
                }
            }

            unsigned int alive = 0;
            Wizard* wizard_alive = NULL;
            for (unsigned int i = 0; i < WIZARDS_SIZE; i++)
            {
                if (wizards[i] != NULL && wizards[i]->health > 0)
                {
                    if(wizards[i]->casting) Draw_Cast(wizards[i]);
                    DrawWizard(wizards[i]);
                    alive++;
                    wizard_alive = wizards[i];
                }
            }
            if(alive <= 1)
            {
                tickdelay(1);
                if(Host) Send_Game_Info();
                for (unsigned int i = 0; i < WIZARDS_SIZE; i++)
                {
                    free(wizards[i]);
                    wizards[i] = NULL;
                }
                for (unsigned int i = 0; i < ELEMS_SIZE; i++)
                {
                    free(elements[i]);
                    elements[i] = NULL;
                }
                if(wizard_alive != NULL && wizard_alive == player)
                {
                    gameStatus = victory;
                }
                else gameStatus = defeat;
                cursor->lb = false;
                cursor->x = 50;
                cursor->y = 50;
                MP = false;
                Host = false;
                username_2 = NULL;
                Clear_UART();
            }

            DrawTimers();

            if (openTextBox == true)
            {
                //para desenhar as letras, com a string q guarda a palavra, vai comparar
                // cada letra uma a uma e por cada letra vai dando display no ecra a letra respetiva uma a uma
                DrawTextBox();
                //DrawTextPointer();
                Draw_string(45, 680);
            }
            DrawCursor(cursor);
        }
        DrawClock();

        if (GameMenus.pause == true)
        {
            DrawPauseMenu();
            DrawCursor(cursor);

            //cursor condition
            //to check
            if (cursor->lb == true && cursor->x >= 280 && cursor->x <= 810 && cursor->y >= 280 && cursor->y <= 425)
            {
                GameMenus.main_page = false;
                GameMenus.run = 1;
                GameMenus.pause = false;
            }

            if (cursor->lb == true && cursor->x >= 280 && cursor->x <= 810 && cursor->y >= 465 && cursor->y <= 610)
            {
                GameMenus.main_page = true;
                GameMenus.run = 0;
                GameMenus.pause = false;
                for (unsigned int i = 0; i < WIZARDS_SIZE; i++)
                {
                    free(wizards[i]);
                    wizards[i] = NULL;
                }
                for (unsigned int i = 0; i < ELEMS_SIZE; i++)
                {
                    free(elements[i]);
                    elements[i] = NULL;
                }
                cursor->lb = false;
                cursor->x = 50;
                cursor->y = 50;
                MP = false;
                Host = false;
                username_2 = NULL;
                Clear_UART();
            }
        }
    }
}

void Send_Game_Info()
{
    for (unsigned int i = 0; i < WIZARDS_SIZE; i++)
    {
        if (wizards[i] != NULL) Send_Wizard(wizards[i],i);
        //printf("\n TRIED TO SEND: %s", wizards[i]->name); //debug
    }
}

Element *Create_Guest_Element(int array_pos, int8_t elem_type, uint8_t spell_type)
{
    Element *elem = (Element *)malloc(sizeof(Element));
    elem->elem_type = elem_type;
    elem->spell_type = spell_type;
    switch (elem->elem_type)
    {
    case Air:
        elem->img = AirBall;
        break;
    case Earth:
        elem->img = EarthBall;
        break;
    case Water:
        elem->img = WaterBall;
        break;
    case Fire:
        elem->img = FireBall;
        break;
    case Null:
        return NULL;
    }
    elements[array_pos] = elem;
    return elem;
}

//timers

void DrawFireTimer()
{
    if (SpellsRdy.fire_timer == 0)
        DrawBitmap(Fire_0, 700, 655);
    if (SpellsRdy.fire_timer == 1)
        DrawBitmap(Fire_1, 700, 655);
    if (SpellsRdy.fire_timer == 2)
        DrawBitmap(Fire_2, 700, 655);
    if (SpellsRdy.fire_timer == 3)
        DrawBitmap(Fire_3, 700, 655);
    if (SpellsRdy.fire_timer == 4)
        DrawBitmap(Fire_4, 700, 655);
    if (SpellsRdy.fire_timer == 5)
        DrawBitmap(Fire_5, 700, 655);
}

void DrawWaterTimer()
{
    if (SpellsRdy.water_timer == 0)
        DrawBitmap(Water_0, 850, 655);
    if (SpellsRdy.water_timer == 1)
        DrawBitmap(Water_1, 850, 655);
    if (SpellsRdy.water_timer == 2)
        DrawBitmap(Water_2, 850, 655);
    if (SpellsRdy.water_timer == 3)
        DrawBitmap(Water_3, 850, 655);
    if (SpellsRdy.water_timer == 4)
        DrawBitmap(Water_4, 850, 655);
    if (SpellsRdy.water_timer == 5)
        DrawBitmap(Water_5, 850, 655);
}

void DrawEarthTimer()
{
    if (SpellsRdy.earth_timer == 0)
        DrawBitmap(Earth_0, 850, 695);
    if (SpellsRdy.earth_timer == 1)
        DrawBitmap(Earth_1, 850, 695);
    if (SpellsRdy.earth_timer == 2)
        DrawBitmap(Earth_2, 850, 695);
    if (SpellsRdy.earth_timer == 3)
        DrawBitmap(Earth_3, 850, 695);
    if (SpellsRdy.earth_timer == 4)
        DrawBitmap(Earth_4, 850, 695);
    if (SpellsRdy.earth_timer == 5)
        DrawBitmap(Earth_5, 850, 695);
}

void DrawWindTimer()
{
    if (SpellsRdy.air_timer == 0)
        DrawBitmap(Wind_0, 700, 695);
    if (SpellsRdy.air_timer == 1)
        DrawBitmap(Wind_1, 700, 695);
    if (SpellsRdy.air_timer == 2)
        DrawBitmap(Wind_2, 700, 695);
    if (SpellsRdy.air_timer == 3)
        DrawBitmap(Wind_3, 700, 695);
    if (SpellsRdy.air_timer == 4)
        DrawBitmap(Wind_4, 700, 695);
    if (SpellsRdy.air_timer == 5)
        DrawBitmap(Wind_5, 700, 695);
}

void DrawLeapTimer()
{
    if (SpellsRdy.leap_timer == 0)
        DrawBitmap(Leap_0, 700, 615);
    if (SpellsRdy.leap_timer == 1)
        DrawBitmap(Leap_1, 700, 615);
    if (SpellsRdy.leap_timer == 2)
        DrawBitmap(Leap_2, 700, 615);
    if (SpellsRdy.leap_timer == 3)
        DrawBitmap(Leap_3, 700, 615);
}

void DrawTimers()
{
    DrawFireTimer();
    DrawWaterTimer();
    DrawEarthTimer();
    DrawWindTimer();

    DrawLeapTimer();
}
