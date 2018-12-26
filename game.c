#include <lcom/lcf.h>

#include "game.h"
#include "video_card.h"
#include "timer.h"
#include "keyboard.h"
#include <math.h>

//layout
Bitmap *background;
Bitmap *Tool_Box;

//mouse manip
Bitmap *P_Cursor;
Bitmap *R_Cursor;
Sprite *GreenWizard;
Sprite *BlueWizard;
Sprite *RedWizard;
Sprite *YellowWizard;
//keyboard manip
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

//elements
Sprite *FireBall;

extern Cursor *cursor;

//List of all Sprites

bool LoadAssets()
{
    if ((background = loadBitmap("Background.bmp")) == NULL)
        return false;
   /* if ((Tool_Box = loadBitmap("Tool_Box.bmp")) == NULL)
        return false;*/
    if ((GreenWizard = CreateSprite("Green_Hat.bmp")) == NULL)
        return false;
    if ((BlueWizard = CreateSprite("Blue_Hat.bmp")) == NULL)
        return false;
    if ((YellowWizard = CreateSprite("Yellow_Hat.bmp")) == NULL)
        return false;
    if ((RedWizard = CreateSprite("Red_Hat.bmp")) == NULL)
        return false;

    //mouse
    if ((P_Cursor = loadBitmap("Cursor_Pressed.bmp")) == NULL)
        return false;
    if ((R_Cursor = loadBitmap("Cursor_Released.bmp")) == NULL)
        return false;

    //load letters
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
    //spell sprites
    if ((FireBall = CreateSprite("Fireball.bmp")) == NULL)
        return false;

    return true;
}

//////////OBJECT ARRAYS///////////
Wizard *wizards[WIZARDS_SIZE];    //4 is max nuber of wizards
Bot *bots[BOTS_SIZE];
Element *elements[ELEMS_SIZE]; //there'll never be more than 30 active spells at the same time so this number is fine

Wizard *CreateWizard(enum Wizard_color color, int center_x, int center_y, unsigned int rot, char* name)
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

void Wizard_Colision(Wizard *wizard, Element *element){
    if(element->elem_type != Null){
        element->active = false;
        wizard->health--;
    }
}

Element *CreateElement(enum Element_Type type, int center_x, int center_y, unsigned int rot)
{
    Element *elem = (Element *)malloc(sizeof(Element));
    elem->elem_type = type;
    switch (type)
    {
    case Air:
        break;
    case Earth:
        break;
    case Water:
        break;
    case Fire:
        elem->img = FireBall;
    case Null:
        break;
    }

    elem->center_x = center_x;
    elem->center_y = center_y;
    elem->rot = rot;
    elem->active = true;

    for (unsigned int i = 0; i < ELEMS_SIZE; i++)
    { //There can only be 30 spells
        if (elements[i] == NULL || !elements[i]->active)
        {
            elements[i] = elem;
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

    cursor->press = false;
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

    if (cursor->press)
        DrawBitmap(cursor->pressed, cursor->x, cursor->y);
    else
        DrawBitmap(cursor->released, cursor->x, cursor->y);
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
    double rot = element->rot * M_PI / 180.0;
    element->center_x -= FAST_SPEED * sin(rot);
    element->center_y -= FAST_SPEED * cos(rot);
}

void Element_Colision(Element *element1, Element *element2){
    int e1 = (int) element1->elem_type;
    int e2 = (int) element2->elem_type;
    if( e1 == e2) return; //Do nothing for elements of same type
    else if( e1*e2 > 0){ //Destroy both for elements of opposing types
        element1->active = false;
        element2->active = false;
    }
    else{ //If they're consecutive elements decie which to destroy
        int res = e1 + e2;
        if(res == -3){ //If colision happens between earth and air (Special case)
            if(element1->elem_type == Air) element1->active = false;
            else element2->active = false;
            return;
        }
        if(abs(e1) > abs(e2)) element1->active = false; //Highest element gets destroyed
        else element2->active = false;
    }
}

Bot* CreateBot(enum Wizard_color color, int center_x, int center_y, char* name){
    Bot *bot = (Bot *)malloc(sizeof(Bot));

    bot->attention_span = rand() % HIGHER_ATTENTION + LOWER_ATTENTION;
    bot->time_to_fire = rand() % HIGHER_TTF + LOWER_TTF;

    //First they'll be pointing to center
    int x = 512;
    int y = 384;
    int angle = atan2(center_y - y, x - center_x) * 180 / M_PI - 90;
    if (angle < 0) angle = 360 + angle;
    bot->target = angle;
    bot->transitioning = true;
    bot->init_diff = 0;

    bot->wizard = CreateWizard(color, center_x, center_y, angle, name);

    for(int i = 0; i < BOTS_SIZE; i++){
        if(bots[i] == NULL){
            bots[i] = bot;
            break;
        }
    }

    return bot;
}

void Change_Target(Bot *bot){
    int target_wizard;
    Wizard* wiz;
    unsigned int fail = 0;

    do{ //Gets a valid wizard to look at
        target_wizard = rand() % WIZARDS_SIZE;
        wiz = wizards[target_wizard];
        fail++;
        if(fail == 20) return; //So there isn't and endless loop
    } while(wiz == NULL || wiz->health <= 0 || wiz == bot->wizard);

    int angle = atan2(bot->wizard->center_y - wiz->center_y, wiz->center_x - bot->wizard->center_x) * 180 / M_PI - 90;
    if (angle < 0) angle = 360 + angle;
    
    if(angle != bot->target){
        bot->target = angle;
        bot->transitioning = true;
    }
    bot->attention_span = rand() % HIGHER_ATTENTION + LOWER_ATTENTION; //NOT FINAL

    //printf("\n TARGET_WIZ: %d  RAND: %d", rand() % WIZARDS_SIZE, rand());
    //printf("\n %s Targeting %s", bot->wizard->name, wiz->name);
    //printf("\n TARGET: %d", bot->target);

    //sleep(3);
}

int Transition_Function(int diff_percentage){  //This values give a very simple semi-human like mouse movement
    if(diff_percentage <= 20) return (pow(diff_percentage,2)/100) + 5 + (rand() % 2);
    else if(diff_percentage < 80) return -pow(40 - diff_percentage,2)/530 + 10 + (rand() % 5);
    else return -pow(80 - diff_percentage,2)/80 + 8 + (rand() % 3);
}

void Bot_Transition(Bot *bot){
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

void Bot_Wobble(Bot *bot){
    //Wobble Algorithm

    if (bot->var_target == bot->wizard->rot)
    { //if we're there already let's get a new target
        int var = rand() % MAX_WOOBLE_VAR - 90; //so it goes from -20 to 20 and every value below or above that is 0
        if (var > 20 || var < -20)
            var = 0; //So zero is more common
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

    if (abs(final_rotation - bot->var_target) < 2) bot->wizard->rot = bot->var_target;
    else bot->wizard->rot = final_rotation;
}

void Update_Bot_Rotation(Bot *bot){
    if(bot->transitioning){
        Bot_Transition(bot);
    }
    else{
        Bot_Wobble(bot);
    }
}

void Bot_Cast(Bot *bot){
    bot->time_to_fire = 3;
    //printf("\nWIP");
}

///////////////////////////////////////////////
bool openTextBox = false;

void Update_Game_State()
{
    ////CHECKING FOR ACTIONS////
    for(unsigned int i = 0; i < BOTS_SIZE; i++){ //Update Bot Behaviour
        Bot* bot = bots[i];
        if (bot != NULL && bot->wizard->health > 0)
        { //If he exists and is alive
            bot->attention_span--;
            bot->time_to_fire--;

            if(bot->attention_span == 0){
                Change_Target(bot);
            }

            if(bot->time_to_fire == 0){
                Bot_Cast(bot);
            }

            Update_Bot_Rotation(bot);
        }
    }

    for (unsigned int i = 0; i < WIZARDS_SIZE; i++)
    { //Cast spells if needed
        if (wizards[i] != NULL && wizards[i]->casting)
        {
            int x = wizards[i]->center_x;
            int y = wizards[i]->center_y;
            double rot = wizards[i]->rot * M_PI / 180.0;
            x -= CAST_DISTANCE * sin(rot);
            y -= CAST_DISTANCE * cos(rot);
            CreateElement(wizards[i]->cast_type, x, y, wizards[i]->rot);
            wizards[i]->casting = false;
        }
    }

    ////MOVE ELEMENTS AND CHECK FOR COLISIONS////
    for (unsigned int i = 0; i < ELEMS_SIZE; i++)
    {
        if (elements[i] != NULL && elements[i]->active)
        {
            Move_Element(elements[i]);
            for (unsigned int x = 0; x < ELEMS_SIZE; x++){ //Colisions between elements
                if (x != i && elements[x] != NULL && elements[x]->active){
                    int x_dis = abs(elements[i]->center_x - elements[x]->center_x);
                    int y_dis = abs(elements[i]->center_y - elements[x]->center_y);
                    int distance = sqrt(pow(x_dis,2) + pow(y_dis,2));
                    if(distance <= BALL_HITBOX_RADIUS*2){ //if there has been a colision
                        Element_Colision(elements[i],elements[x]);
                    }
                }
            }

            for(unsigned int x = 0; x < WIZARDS_SIZE; x++){ //Colisions between elements and wizards
                if(wizards[x] != NULL && wizards[x]->health > 0){
                    int x_dis = abs(elements[i]->center_x - wizards[x]->center_x);
                    int y_dis = abs(elements[i]->center_y - wizards[x]->center_y);
                    int distance = sqrt(pow(x_dis,2) + pow(y_dis,2));
                    if(distance <= BALL_HITBOX_RADIUS + WIZARD_HITBOX_RADIUS){ //if there has been a colision
                        Wizard_Colision(wizards[x],elements[i]);
                    }
                }
            }
        }
    }

    ////DRAWING TO BUFFER////
    DrawBackground();
    for (unsigned int i = 0; i < ELEMS_SIZE; i++)
    {
        if (elements[i] != NULL && elements[i]->active)
        {
            DrawElement(elements[i]);
        }
    }

    for (unsigned int i = 0; i < WIZARDS_SIZE; i++)
    { //Cast spells if needed
        if (wizards[i] != NULL && wizards[i]->health > 0)
        {
            DrawWizard(wizards[i]);
        }
    }

    //DrawToolBox();
    DrawTextBox();
    DrawTimers();

    if (openTextBox == true)
    {
        //para desenhar as letras, com a string q guarda a palavra, vai comparar
        // cada letra uma a uma e por cada letra vai dando display no ecra a letra respetiva uma a uma
        DrawTextPointer();
        Draw_string();
    }
    DrawCursor(cursor);
}

//timers

extern unsigned fire_timer;
extern unsigned water_timer;
extern unsigned earth_timer;
extern unsigned wind_timer;

void DrawFireTimer()
{
    if (fire_timer == 0)
        DrawBitmap(Fire_0, 400, 655);
    if (fire_timer == 1)
        DrawBitmap(Fire_1, 400, 655);
    if (fire_timer == 2)
        DrawBitmap(Fire_2, 400, 655);
    if (fire_timer == 3)
        DrawBitmap(Fire_3, 400, 655);
    if (fire_timer == 4)
        DrawBitmap(Fire_4, 400, 655);
    if (fire_timer == 5)
        DrawBitmap(Fire_5, 400, 655);
}

void DrawWaterTimer()
{
    if (water_timer == 0)
        DrawBitmap(Water_0, 550, 655);
    if (water_timer == 1)
        DrawBitmap(Water_1, 550, 655);
    if (water_timer == 2)
        DrawBitmap(Water_2, 550, 655);
    if (water_timer == 3)
        DrawBitmap(Water_3, 550, 655);
    if (water_timer == 4)
        DrawBitmap(Water_4, 550, 655);
    if (water_timer == 5)
        DrawBitmap(Water_5, 550, 655);
}

void DrawEarthTimer()
{
    if (earth_timer == 0)
        DrawBitmap(Earth_0, 550, 695);
    if (earth_timer == 1)
        DrawBitmap(Earth_1, 550, 695);
    if (earth_timer == 2)
        DrawBitmap(Earth_2, 550, 695);
    if (earth_timer == 3)
        DrawBitmap(Earth_3, 550, 695);
    if (earth_timer == 4)
        DrawBitmap(Earth_4, 550, 695);
    if (earth_timer == 5)
        DrawBitmap(Earth_5, 550, 695);
}

void DrawWindTimer()
{
    if (wind_timer == 0)
        DrawBitmap(Wind_0, 400, 695);
    if (wind_timer == 1)
        DrawBitmap(Wind_1, 400, 695);
    if (wind_timer == 2)
        DrawBitmap(Wind_2, 400, 695);
    if (wind_timer == 3)
        DrawBitmap(Wind_3, 400, 695);
    if (wind_timer == 4)
        DrawBitmap(Wind_4, 400, 695);
    if (wind_timer == 5)
        DrawBitmap(Wind_5, 400, 695);
}

void DrawTimers()
{
    DrawFireTimer();
    DrawWaterTimer();
    DrawEarthTimer();
    DrawWindTimer();
}
