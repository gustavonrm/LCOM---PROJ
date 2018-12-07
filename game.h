#ifndef _GAME_H_
#define _GAME_H_

#include "bitmap.h"
#include "video_card.h"

enum Element_Type { Air, Earth, Water, Fire };
enum Wizard_color { Red, Green, Blue, Yellow };

typedef struct {
    Bitmap* bitmap[360];  //list of all 360 possible rotations
} Sprite;

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

bool LoadAssets();
Sprite* CreateSprite(char img_name[]);
void DrawSprite(Sprite* img, int center_x, int center_y, unsigned int rot);
Wizard* CreateWizard(enum Wizard_color color, int center_x, int center_y, unsigned int rot);


#endif
