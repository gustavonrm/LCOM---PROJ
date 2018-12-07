#include <lcom/lcf.h>

#include "game.h"

Bitmap *background;
Bitmap *P_Cursor;
Bitmap *R_Cursor;
Sprite *GreenWizard;
//List of all Sprites

bool LoadAssets()
{

    if ((background = loadBitmap("Background.bmp")) == NULL)
        return false;
    if ((GreenWizard = CreateSprite("Green_Hat.bmp")) == NULL)
        return false;
    if ((P_Cursor = loadBitmap("Cursor_Pressed.bmp")) == NULL)
        return false;
    if ((R_Cursor = loadBitmap("Cursor_Released.bmp")) == NULL)
        return false;

    return true;
}

Sprite *CreateSprite(char img_name[])
{
    Bitmap *bmp;
    if ((bmp = loadBitmap(img_name)) == NULL)
        return NULL;
    Sprite *sprite = (Sprite *)malloc(sizeof(Sprite));

    /*For cicle calling function to rotate the bitmap in 360 directions*/

    sprite->bitmap[0] = bmp; //for now

    return sprite;
}

void DrawSprite(Sprite *img, int center_x, int center_y, unsigned int rot)
{
    int x, y; //drawing position
    x = center_x - img->bitmap[rot]->bitmapInfoHeader.width;
    y = center_y - img->bitmap[rot]->bitmapInfoHeader.height;

    DrawBitmap(img->bitmap[rot], x, y);
}

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

    return wizard;
}

void Cursor( bool LB_Pressed)
{

    if(LB_Pressed == false){
      DrawBitmap(R_Cursor, 50, 50);  
    }
    if(LB_Pressed == true){
        DrawBitmap(P_Cursor, 50, 50); 
    }


}
