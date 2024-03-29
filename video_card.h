#ifndef _VIDEO_CARD_H_
#define _VIDEO_CARD_H_

#include <lcom/lcf.h>
#include "macros.h"
#include "game.h"

#define H_RES				1024
#define V_RES				767

uint16_t getXRes();
uint16_t getYRes();
uint8_t getByts_per_pixel();
uint8_t getMemoryModel();
uint32_t direct_color(uint16_t row, uint16_t col, uint32_t first, uint8_t step);
uint8_t getRedMaskSize();
uint8_t getGreenMaskSize();
uint8_t getBlueMaskSize();

int draw_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
int vg_draw_line( uint16_t x, uint16_t y,uint16_t len,uint32_t color );
int draw_pixel(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Draws the Background BMP
 */
void DrawBackground();
/**
 * @brief Draws the Main page bmp
 */
void DrawMainPage();
/**
 * @brief Draws the information box
 */
void DrawInfoBox();
/**
 * @brief Draws the Pause menu
 */
void DrawPauseMenu();
/**
 * @Draws the Victory BMP
 */
void DrawVictory();
/**
 * @brief Draws the Defeat BMP
 */
void DrawDefeat();
/**
 * @brief Draws the bitmap to the buffer
 * @param bitmap bitmap to draw
 * @param x top left x coordinate
 * @param y top left y coordinate
 */
void DrawBitmap(Bitmap* bitmap, int x, int y);
/**
 * @brief Copies buffer to video memory
 */
void UpdateVideo();



////SPRITE////
/**
 * @brief Creates a Sprite with the given name
 * @param img_name name of the sprite
 * @return Pointer to Sprite
 */
Sprite* CreateSprite(char img_name[]);
/**
 * @brief Rotates the Image given by the specified number of angles
 * @param image bitmap to rotate
 * @param angle number of angles to rotate
 * @return returns the new bmp pointer
 */
Bitmap* RotateImage(Bitmap* image, float angle);
/**
 * @brief frees sprites
 * @param sp sprite to be freed
 */
void destroy_sprite(Sprite *sp);
/**
 * @brief Draws the sprite to the buffer
 * @param sp sprite to draw
 * @param center_x x center of sprite
 * @param center_y y center of sprite
 * @param rot rotatio nof sprite
 * @param centered true if teh x and y are the center, false if they are the top left corners
 */
void DrawSprite(Sprite *sp, int center_x, int center_y, unsigned int rot, bool centered);

int new_vbe_get_mode_info(uint16_t mode,vbe_mode_info_t * vmi_p );	
int vbe_get_ctrl_info(); 

typedef struct __attribute__((packed)){
  char VBESignature[4]; 
  BCD VBEVersion[2];
  uint32_t OEMString;
  uint8_t Capabilities[4];
  uint32_t VideoModeList;
  uint16_t TotalMemory; 
  uint16_t OEMSoftwareRevision; 
  uint32_t OEMVendorNamePtr;
  uint32_t OEMProductNamePtr;
  uint32_t OEMProductRevPtr;
  uint8_t Reserved[222];
  uint8_t OEMData[256];
} aux_ctrl_info_t;


#endif
