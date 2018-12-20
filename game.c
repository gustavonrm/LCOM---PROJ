#include <lcom/lcf.h>

#include "game.h"
#include "video_card.h"
#include "keyboard.h"
#include <math.h>

Bitmap *background;
Bitmap *P_Cursor;
//mouse manip
Bitmap *R_Cursor;
Sprite *GreenWizard;
Sprite *FireBall;
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

extern Cursor *cursor;

bool LoadAssets()
{
    if ((background = loadBitmap("Background.bmp")) == NULL)
        return false;
    if ((GreenWizard = CreateSprite("Green_Hat.bmp")) == NULL)
        return false;
    if((FireBall = CreateSprite("Fireball.bmp")) == NULL)
        return false;
    if ((P_Cursor = loadBitmap("Cursor_Pressed.bmp")) == NULL)
        return false;
    if ((R_Cursor = loadBitmap("Cursor_Released.bmp")) == NULL)
        return false;
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
    return true;

    return true;
}

//////////OBJECT ARRAYS///////////
Wizard* wizards[4];  //4 is max nuber of wizards
Element* elements[30]; //there'll never be more than 30 active spells at the same time so this number is fine

Wizard *CreateWizard(enum Wizard_color color, int center_x, int center_y, unsigned int rot)
{
    Wizard *wizard = (Wizard *)malloc(sizeof(Wizard));
    wizard->color = color;
    switch (color)
    {
    case Red:
        break;
    case Green:
        wizard->img = GreenWizard;
        break;
    case Blue:
        break;
    case Yellow:
        break;
    }

    wizard->center_x = center_x;
    wizard->center_y = center_y;
    wizard->rot = rot;
    wizard->casting = false;
    wizard->cast_type = Null;

    for(unsigned int i = 0; i < WIZARDS_SIZE; i++){ //There can only be 4 wizards
        if(wizards[i] == NULL){
            wizards[i] = wizard;
            break;
        }
    }    

    return wizard;
}

Element* CreateElement(enum Element_Type type, int center_x, int center_y, unsigned int rot){
    Element *elem = (Element *) malloc(sizeof(Element));
    elem->elem_type = type;
    switch(type){
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

    for(unsigned int i = 0; i < ELEMS_SIZE; i++){ //There can only be 30 spells
        if(elements[i] == NULL || !elements[i]->active){
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
    DrawBitmap(TextBox, 20, 595);
}

void DrawWizard(Wizard *wizard){
    DrawSprite(wizard->img, wizard->center_x, wizard->center_y, wizard->rot, true);
}

void DrawElement(Element *element){
    DrawSprite(element->img, element->center_x, element->center_y, element->rot, true);
}

void Move_Element(Element *element){
    double rot = element->rot * M_PI / 180.0;
    element->center_x -= FAST_SPEED*sin(rot);
    element->center_y -= FAST_SPEED*cos(rot);
}

bool openTextBox = false;

void Update_Game_State(){
    ////CHECKING FOR ACTIONS////
    for(unsigned int i = 0; i < WIZARDS_SIZE; i++){  //Cast spells if needed
        if(wizards[i] != NULL && wizards[i]->casting){
            int x = wizards[i]->center_x;
            int y = wizards[i]->center_y;
            double rot = wizards[i]->rot * M_PI / 180.0;
            x -= CAST_DISTANCE*sin(rot);
            y -= CAST_DISTANCE*cos(rot); 
            CreateElement(wizards[i]->cast_type, x, y, wizards[i]->rot);
            printf("\nROT: %d", wizards[i]->rot);
            wizards[i]->casting = false;
        }
    }

    ////DRAWING TO BUFFER////
    DrawBackground();
    for(unsigned int i = 0; i < ELEMS_SIZE; i++){
        if(elements[i] != NULL && elements[i]->active){
            Move_Element(elements[i]);
            DrawElement(elements[i]);
        }
    }

    for(unsigned int i = 0; i < WIZARDS_SIZE; i++){  //Cast spells if needed
        if(wizards[i] != NULL && wizards[i]->heatlh > 0){
            DrawWizard(wizards[i]);
        }
    }

    if (openTextBox == true)
    {
    //para desenhar as letras, com a string q guarda a palavra, vai comparar
    // cada letra uma a uma e por cada letra vai dando display no ecra a letra respetiva uma a uma
    DrawTextBox();
    Draw_string(); 
    }
    DrawCursor(cursor);
}
